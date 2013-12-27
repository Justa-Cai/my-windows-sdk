//---------------------------------------------------------------------------

#include <vcl.h>
#define CN_COMM_VCL_EXCEPTION

#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
   Comm_.SetWnd(Handle) ;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    if (!Comm_.IsOpen())
    {
        Comm_.Open(ComboBox1->ItemIndex + 1) ;
        SetState();
        Button1->Caption = "¹Ø±Õ";
    }
    else
    {
        Comm_.Close();
        Button1->Caption = "´ò¿ª";
    }
}
//---------------------------------------------------------------------------
LRESULT __fastcall TForm1::OnReceive(TMessage &msg)
{
    if (msg.WParam == Comm_.GetPort())
    {
        char buffer[1024];
        Comm_.ReadString(buffer, 1024);
        Memo1->Lines->Add(buffer);
    }

    return TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetState()
{
    if (!Comm_.IsOpen())
		return;
	
	struct Parity{
		BYTE dwParity;
		TCHAR sParity[24];
	} Paritys[] = 
	{
		{NOPARITY    ,_T("NO")},
		{ODDPARITY   ,_T("ODD")},
		{EVENPARITY  ,_T("EVEN")},
		{MARKPARITY  ,_T("MARK")},
		{SPACEPARITY ,_T("SPACE")}
	};
	
	BYTE ByteSizes[] = {4, 5, 6, 7, 8};
	
	struct StopBit {
		BYTE dwStopBit;
		TCHAR sStopBit[24];
	} StopBits [] = 
	{
		{ONESTOPBIT   ,_T("ONESTOPBIT")},
		{ONE5STOPBITS ,_T("ONE5STOPBITS")},
		{TWOSTOPBITS  ,_T("TWOSTOPBITS")},
	};
	
	char szBuf[1024];

	DCB* pDcb = Comm_.GetState();
	pDcb->BaudRate = ComboBox2->Text.ToInt();
    pDcb->Parity = Paritys[ComboBox3->ItemIndex].dwParity;
	pDcb->ByteSize = ByteSizes[ComboBox4->ItemIndex];
	pDcb->StopBits = StopBits[ComboBox5->ItemIndex].dwStopBit;

	if (!Comm_.SetState(pDcb))
	{
		wsprintf(szBuf, "SetState(%d, %d, %d, %d), Fail. Code:%d",
			ComboBox2->Text.ToInt(),
			Paritys[ComboBox3->ItemIndex].dwParity,
			ByteSizes[ComboBox4->ItemIndex],
			StopBits[ComboBox5->ItemIndex].dwStopBit,
			GetLastError());
		ShowMessage(szBuf);
		return;
	}
}
void __fastcall TForm1::Button2Click(TObject *Sender)
{
    if (Comm_.IsOpen())
        Comm_.Write(Edit2->Text.c_str());

}

//---------------------------------------------------------------------------

