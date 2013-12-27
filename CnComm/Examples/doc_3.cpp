/*! \file doc_3.cpp 
	\brief ��������3 ģ��Shell����̨ ������2���Ӹ���һЩ���ô���ģ��˫��ܵ�
*/

#include <io.h>
#include <fcntl.h>
#include <process.h>
#include "..\Src\CnComm.h"


//! ����˫��ܵ��滻�ӽ��̵����������������CMD.EXE�ӽ��̣��γɼ򵥵�Shell��װ
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

//! Shell����̨������ ��������: Э�� 0x2+ASCII(0��1024)+0x3 ��һ���ݰ���СΪ2�ֽ� ���Ϊ1024+2�ֽ� 
class ShellComm : public CnComm
{
public:
	//! ����2���߳� ���ö�д���� �����ص�IO
	ShellComm() : CnComm(EN_THREAD | EN_TX_BUFFER | EN_RX_BUFFER | EN_OVERLAPPED | EN_TX_THREAD)
	{
		iPacketBegin_ = -1;
	}
	//! ���ؽ����¼�
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
	//! �������
	void OnOverflow(BlockBuffer::Iterator& iter)
	{
		I_.Read(0, iter.O_);//ɾ��
		iter = I_.Begin();
		iPacketBegin_ = (bool)iter && *iter == '\x2' ? iter.O_ : -1;
	}
	//! ��Ӧ����
	void OnPacket(DWORD begin, DWORD end)
	{
		if (end - begin > 1024)
		{
			O_.SafeWrite("Command too long. \n\x3");
			BufferToPort();//�����߳��б���д��˿�
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

	//! ����д��̨�߳�
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

//! ִ�н��: Э�� ASCII(0��N)+0x3 
class ClientComm : public CnComm
{
public:
	//! ����CnComm �����߳� �ص�IO ˫������
	ClientComm(): CnComm(EN_THREAD | EN_TX_BUFFER | EN_RX_BUFFER | EN_OVERLAPPED)
	{
		hEvent_ = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	//! 
	virtual ~ClientComm()
	{
		CloseHandle(hEvent_);
	}
	//! ����������Ӧ��Ϣ \sa CnComm::OnReceive()
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
	//! �ȴ�������Ӧ
	bool WaitReturn(DWORD dwWaitTime = 100)
	{
		 return WaitForSingleObject(hEvent_, dwWaitTime) == WAIT_OBJECT_0;
	}
	//! ��������
	void SendPacket(const char *szCmd)
	{
		ResetEvent(hEvent_);
		Write("\x2");
		Write(szCmd);
		Write("\x3");
	}

protected:
	HANDLE hEvent_; //!< �ȴ������¼�

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