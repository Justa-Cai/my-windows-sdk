// CnCommTest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CnCommTest.h"
#include "CnCommTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCnCommTestApp

BEGIN_MESSAGE_MAP(CCnCommTestApp, CWinApp)
END_MESSAGE_MAP()


// CCnCommTestApp ����
CCnCommTestApp::CCnCommTestApp()
	: CWinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCnCommTestApp ����
CCnCommTestApp theApp;

// CCnCommTestApp ��ʼ��

BOOL CCnCommTestApp::InitInstance()
{
    // ��Ӧ�ó����ʼ���ڼ䣬Ӧ����һ�� SHInitExtraControls �Գ�ʼ��
    // ���� Windows Mobile �ض��ؼ����� CAPEDIT �� SIPPREF��
    SHInitExtraControls();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CCnCommTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
