/*! \file doc_0.cpp 
	\brief ��������0 ��򵥵�Ӧ��
*/

#include <locale.h>
#include "..\Src\CnComm.h"

//! ��������
void SayHello(DWORD dwPort)
{
	CnComm Com(0);//! v1.5 ����IO �������߳�ģ��
	Com.Open(dwPort);
	Com.Write(_T("Hello world. PS:���˲���������ġ�"));
}
//! ��������0
class HelloComm : public CnComm
{	//! \sa CnComm::OnReceive()
	void OnReceive()
	{
		TCHAR buffer[1024];
		setlocale(LC_ALL, "chs");// Unicode��������ʾ���� ����һ�� CBC ��֧�� "chs"
		_tprintf(_T("\nRecv:%s\n"), ReadString(buffer, 1024));
	}
};
//! Main()  COM1 ---�ַ���----> COM2 (������9600)
int main()
{	
	HelloComm Com;//! Ĭ��ģʽ �ص�IO �����߳�

	Com.Open(2, 9600);

	system("pause");//��ͣ

	SayHello(1);

	system("pause");//��ͣ

	return 0;
}

