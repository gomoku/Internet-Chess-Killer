//---------------------------------------------------------------------------

#ifndef RegisterWindowH
#define RegisterWindowH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormRegister : public TForm
{
__published:	// IDE-managed Components
  TEdit *edComputerId;
  TBitBtn *BitBtn1;
  TBitBtn *BitBtn2;
  TLabel *lbComputerId;
  TEdit *edKey;
  TLabel *lbKey;
private:	// User declarations
public:		// User declarations
  __fastcall TFormRegister(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormRegister *FormRegister;
//---------------------------------------------------------------------------
#endif
