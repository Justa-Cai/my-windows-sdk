/*! \file doc_2.cpp 
	\brief 样例程序2 可执行非交互式控制台/DOS命令 交互式通讯
*/

#include <io.h>
#include <fcntl.h>
#include <process.h>
#include "..\Src\CnComm.h"

//! 执行单一方向命令串口类 
class CmdComm : public CnComm
{
public:

	CmdComm()
	{
		iIndex_ = -1;
		memset(Packet_, 0, 1024);
	}
	//! 接收
	void OnReceive()
	{
		char buffer[1024];
		int len;

		len = Read(buffer, 1024);

		for (int i = 0; i<len ; i++)
		{
			if (iIndex_ == -1)
			{
				if (buffer[i] == '\x2')
					iIndex_ = 0;
				else//抛弃无意义数据
					continue;
			}
			else
			{
				if (iIndex_ == 1024)
				{
					iIndex_ = -1, i--; //溢出
					continue;
				}

				if (buffer[i] == '\x3')//完成一个命令的接收，响应
					Packet_[iIndex_] = 0, RunCmd(Packet_), iIndex_ = -1;
				else
					Packet_[iIndex_++] = buffer[i];
			}
		}
	}
	//! 运行命令，将命令运行结果存入写缓冲区，然后输出到串口
	void RunCmd(const char * cmd)
	{
		FILE *fp = _popen(cmd, "rt");
		if (fp)
		{
			while(!feof(fp))
			{
				int len = fread(O_.GetFreePtr(256), 1, 256, fp);
				O_.Release(len);
			}
			_pclose(fp);
		}
		O_.Write("\x3");

		while(O_.Size())
			BufferToPort();//辅助线程中必须写入端口
	}

protected:
	int		iIndex_;		//!< 包长度
	char	Packet_[1024];	//!< 包缓冲区
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

//! Main()  COM1 ---字符串----> COM2 (波特率9600)
int main()
{	
	char buf[1024] = {0};

	ClientComm Com1;
	CmdComm Com2;

	Com1.Open(1, 9600);
	Com2.Open(2, 9600);
	printf("只能接受非交互式命令\n");

	while(1)
	{
		printf("\nCmd>");

		gets(buf);

		if (!_stricmp(buf, "exit"))
			break;

		Com1.SendPacket(buf);

		while(!Com1.WaitReturn())
			;
	}

	return 0;
}