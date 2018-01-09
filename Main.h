//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#define SWAP(x,y) {int t; t = x; x = y; y = t;}
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AxAltairUDrv_OCX.h"
#include "AxOvkBase_OCX.h"
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TImage *Image1;
    TAxAltairU *AxAltairU1;
    TAxImageBW8 *AxImageBW81;
    TButton *Button1;
    TImage *Image2;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TAxImageBW8 *AxImageBW82;
    TImage *Image3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TButton *Button2;
    TAxImageBW8 *AxImageBW83;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall AxAltairU1SurfaceFilled(TObject *Sender,
          long SurfaceHandle);
    void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    void TForm1::PrintArr(byte Arr[]);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
