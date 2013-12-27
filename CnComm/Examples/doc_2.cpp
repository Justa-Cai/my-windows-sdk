/*! \file doc_2.cpp 
	\brief ��������2 ��ִ�зǽ���ʽ����̨/DOS���� ����ʽͨѶ
*/

#include <io.h>
#include <fcntl.h>
#include <process.h>
#include "..\Src\CnComm.h"

//! ִ�е�һ����������� 
class CmdComm : public CnComm
{
public:

	CmdComm()
	{
		iIndex_ = -1;
		memset(Packet_, 0, 1024);
	}
	//! ����
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
				else//��������������
					continue;
			}
			else
			{
				if (iIndex_ == 1024)
				{
					iIndex_ = -1, i--; //���
					continue;
				}

				if (buffer[i] == '\x3')//���һ������Ľ��գ���Ӧ
					Packet_[iIndex_] = 0, RunCmd(Packet_), iIndex_ = -1;
				else
					Packet_[iIndex_++] = buffer[i];
			}
		}
	}
	//! ����������������н������д��������Ȼ�����������
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
			BufferToPort();//�����߳��б���д��˿�
	}

protected:
	int		iIndex_;		//!< ������
	char	Packet_[1024];	//!< ��������
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

//! Main()  COM1 ---�ַ���----> COM2 (������9600)
int main()
{	
	char buf[1024] = {0};

	ClientComm Com1;
	CmdComm Com2;

	Com1.Open(1, 9600);
	Com2.Open(2, 9600);
	printf("ֻ�ܽ��ܷǽ���ʽ����\n");

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