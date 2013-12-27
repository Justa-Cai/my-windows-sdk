/*! \file doc_3.cpp 
	\brief 样例程序3 模拟Shell控制台 比样例2更加复杂一些，用串口模拟双向管道
*/

#include <io.h>
#include <fcntl.h>
#include <process.h>
#include "..\Src\CnComm.h"


//! 创建双向管道替换子进程的输入输出，并创建CMD.EXE子进程，形成简单的Shell封装
class ShellWrapper
{
public:
	ShellWrapper()
	{
		int iReturn;
		iReturn = _pipe(iPipeIn_, 512, _O_TEXT | O_NOINHERIT);
		assert(iReturn != -1);
		iReturn = _pipe(iPipeOut_, 512, _O_TEXT | O_NOINHERIT);
		assert(iReturn != -1);

		//save old handle
		int iStdin = _dup(_fileno(stdin));
		int iStdout = _dup(_fileno(stdout));
		int iStderr = _dup(_fileno(stderr));

		iReturn = _dup2(iPipeOut_[0], _fileno(stdin));
		assert(iReturn == 0);
		iReturn = _dup2(iPipeIn_[1], _fileno(stdout));
		assert(iReturn == 0);
		iReturn = _dup2(iPipeIn_[1], _fileno(stderr));
		assert(iReturn == 0);

		char *arg[] = {"cmd.exe", "/Q", "/A", NULL};
		iSubProcess_ = spawnvp(P_NOWAIT, arg[0], arg);

		//restore old handle
		iReturn = _dup2(iStdin, _fileno(stdin));
		iReturn = _dup2(iStdout, _fileno(stdout));
		iReturn = _dup2(iStderr, _fileno(stderr));
	}

	virtual ~ShellWrapper()
	{
		_write(Write(), "exit\n", 5);
		_flushall();
		_cwait(NULL, iSubProcess_, NULL);

		close(iPipeIn_[0]);
		close(iPipeIn_[1]);    
		close(iPipeOut_[0]);
		close(iPipeOut_[1]);
	}

	int Write()
	{
		return iPipeOut_[1];
	}

	int Read()
	{
		return iPipeIn_[0];
	}

	HANDLE Process()
	{
		return (HANDLE)iSubProcess_;
	}

	bool IsAlive()
	{
		DWORD dwExitCode = STILL_ACTIVE;
		if(!GetExitCodeProcess((HANDLE)iSubProcess_, &dwExitCode))
			return false;

		return dwExitCode == STILL_ACTIVE;
	}

protected:

	int iPipeOut_[2], iPipeIn_[2];// Write--> iPipeOut_[1] --> iPipeOut_[0] --> sub_process --> iPipeIn_[1] --> iPipeIn_[0]--> Read
	int iSubProcess_;
};

//! Shell控制台串口类 接收命令: 协议 0x2+ASCII(0～1024)+0x3 及一数据包最小为2字节 最大为1024+2字节 
class ShellComm : public CnComm
{
public:
	//! 启用2个线程 启用读写缓冲 启用重叠IO
	ShellComm() : CnComm(EN_THREAD | EN_TX_BUFFER | EN_RX_BUFFER | EN_OVERLAPPED | EN_TX_THREAD)
	{
		iPacketBegin_ = -1;
	}
	//! 重载接收事件
	void OnReceive()
	{
		for (BlockBuffer::Iterator iter = I_.Begin(); iter; iter += 1)
		{
			if (iPacketBegin_ != -1)
			{
				if (*iter == '\x3')
				{
					OnPacket(iPacketBegin_ + 1, iter.O_);
					iPacketBegin_ = -1;
				}
				else
				{
					if (iter.O_ > iPacketBegin_+254)
						OnOverflow(iter);
				}
			}
			else
			{
				if (*iter == '\x2')
					iPacketBegin_ = iter.O_;
			}
		}

		if (iPacketBegin_ == -1)
			I_.Clear();
	}
	//! 定义溢出
	void OnOverflow(BlockBuffer::Iterator& iter)
	{
		I_.Read(0, iter.O_);//删除
		iter = I_.Begin();
		iPacketBegin_ = (bool)iter && *iter == '\x2' ? iter.O_ : -1;
	}
	//! 相应命令
	void OnPacket(DWORD begin, DWORD end)
	{
		if (end - begin > 1024)
		{
			O_.SafeWrite("Command too long. \n\x3");
			BufferToPort();//辅助线程中必须写入端口
		}
		else
		{
			char temp[1024] = {0};
			I_.Copy(temp, end-begin, begin);

			if (Shell_.IsAlive())
			{
				_write(Shell_.Write(), temp,  end-begin);
				_write(Shell_.Write(), "\n", 1);
			}
		}
	}

	//! 重载写后台线程
	DWORD WriteModel()
	{
		while ( Shell_.IsAlive() && bContinue_)
		{
			if (!_eof(Shell_.Read()))
			{
				while (!_eof(Shell_.Read()) && bContinue_)
				{
					BlockBuffer::InnerLock lock(&O_);
					int len = _read(Shell_.Read(), O_.GetFreePtr(256), 256);
					O_.Release(len);
				}
			}
			else
				Sleep(50);

			if (O_.Size())
			{
				O_.Write("\x3");
				BufferToPort();
			}
		}

		return 0;
	}

protected:
	int				iPacketBegin_;
	ShellWrapper	Shell_;
};

//! 执行结果: 协议 ASCII(0～N)+0x3 
class ClientComm : public CnComm
{
public:
	//! 配置CnComm 监视线程 重叠IO 双缓冲区
	ClientComm(): CnComm(EN_THREAD | EN_TX_BUFFER | EN_RX_BUFFER | EN_OVERLAPPED)
	{
		hEvent_ = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	//! 
	virtual ~ClientComm()
	{
		CloseHandle(hEvent_);
	}
	//! 接收命令响应信息 \sa CnComm::OnReceive()
	void OnReceive()
	{
		for (BlockBuffer::Iterator iter = I_.Begin(); iter; iter++)
		{
			if (*iter == '\x3')
				SetEvent(hEvent_);
			else
				putchar(*iter);
		}

		I_.Clear();
	}
	//! 等待命令响应
	bool WaitReturn(DWORD dwWaitTime = 100)
	{
		 return WaitForSingleObject(hEvent_, dwWaitTime) == WAIT_OBJECT_0;
	}
	//! 发送命令
	void SendPacket(const char *szCmd)
	{
		ResetEvent(hEvent_);
		Write("\x2");
		Write(szCmd);
		Write("\x3");
	}

protected:
	HANDLE hEvent_; //!< 等待输入事件

};

//! 
int main()
{	
	char buf[1024] = {0}, *p;

	
	ClientComm Com1;
	ShellComm Com2;

	Com1.Open(1, 9600);
	Com2.Open(2, 9600);

	while(gets(buf))
	{
		Com1.SendPacket(buf);

		Com1.WaitReturn();

		for(p = buf; *p && isspace(*p); p++)
			;//erase space

		for (char *e = p + strlen(p); *e && isspace(*e); e--)
			*e = 0;//erase space

		if (!_stricmp(buf, "exit"))
			break;
	}

	return 0;
}