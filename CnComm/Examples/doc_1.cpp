/*! \file doc_1.cpp  
	\brief 样例程序1 缓冲区使用代码片段 异常功能使用片段
*/

#define CN_COMM_BUFFER_MIN_BLOCK_SIZE 512 //!<重定义缓冲区大小
#define CN_COMM_STD_EXCEPTION
//#define CN_COMM_MFC_EXCEPTION
//#define CN_COMM_VCL_EXCEPTION

#include "..\Src\CnComm.h"

void TestException()
{
	try
	{
		CnComm Com;
		Com.Open(888);
	}
	catch (exception& e)
	{
		printf("%s", e.what());
	}
}

void TestBlockBuffer()
{
	char buf[1024] = {0};
	CnComm::BlockBuffer bb;
	
	bb.Write("0123456789");//写入10个字节 
	bb.Read(buf, 5);//读出5个字节 并返回实际读出数
	bb.Clear();//清空
	printf("%s\n", buf);

	char *pFree = (char*)bb.GetFreePtr(10);// 获得空闲缓冲区
	strcpy(pFree, "0123456789");// 写入10个字节 
	bb.Release(10); // 反馈实际使用大小

	bb.Copy(buf, 5, 10); //复制数据 偏移10 5个字节 数据还保存在缓冲区
	bb.Read(NULL, 10);//删除10个字节
	printf("%s\n", buf);


	bb.Write("0123456789");
	bb.Read(buf, 10);//等于bb.Copy(buf, 10, 0), bb.Read(0, 10);
	printf("%s\n", buf);
}

int main()
{
	TestException();
	TestBlockBuffer();
	return 0;
}
