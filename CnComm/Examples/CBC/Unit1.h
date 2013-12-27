//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "..\..\src\CnComm.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TComboBox *ComboBox1;
        TComboBox *ComboBox2;
        TComboBox *ComboBox3;
        TComboBox *ComboBox4;
        TComboBox *ComboBox5;
        TButton *Button1;
        TEdit *Edit2;
        TButton *Button2;
    TMemo *Memo1;
        void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations

    BEGIN_MESSAGE_MAP
        VCL_MESSAGE_HANDLER(ON_COM_RECEIVE, TMessage, OnReceive)
    END_MESSAGE_MAP(TForm)
    void __fastcall SetState();
    LRESULT __fastcall OnReceive(TMessage &msg);
    CnComm Comm_;
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
