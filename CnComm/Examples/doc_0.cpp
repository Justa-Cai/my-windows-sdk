/*! \file doc_0.cpp 
	\brief 样例程序0 最简单的应用
*/

#include <locale.h>
#include "..\Src\CnComm.h"

//! 发送数据
void SayHello(DWORD dwPort)
{
	CnComm Com(0);//! v1.5 阻塞IO 不启动线程模型
	Com.Open(dwPort);
	Com.Write(_T("Hello world. PS:懒人才是最优秀的。"));
}
//! 样例程序0
class HelloComm : public CnComm
{	//! \sa CnComm::OnReceive()
	void OnReceive()
	{
		TCHAR buffer[1024];
		setlocale(LC_ALL, "chs");// Unicode下中文显示不出 配置一下 CBC 不支持 "chs"
		_tprintf(_T("\nRecv:%s\n"), ReadString(buffer, 1024));
	}
};
//! Main()  COM1 ---字符串----> COM2 (波特率9600)
int main()
{	
	HelloComm Com;//! 默认模式 重叠IO 监视线程

	Com.Open(2, 9600);

	system("pause");//暂停

	SayHello(1);

	system("pause");//暂停

	return 0;
}

