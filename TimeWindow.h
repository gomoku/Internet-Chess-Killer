//---------------------------------------------------------------------------

#ifndef TimeWindowH
#define TimeWindowH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TTimeForm : public TForm
{
__published:	// IDE-managed Components
  TBitBtn *BitBtn1;
  TBitBtn *BitBtn2;
  TPanel *Panel;
  TCSpinEdit *edTime;
  TCSpinEdit *edIncTime;
  TLabel *lbTime;
  TLabel *lbIncrement;
  TCSpinEdit *edLagTime;
  TLabel *lbLagTime;
  TCSpinEdit *edTimeSec;
  TLabel *lbSec;
private:	// User declarations
public:		// User declarations
  __fastcall TTimeForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTimeForm *TimeForm;
//---------------------------------------------------------------------------
#endif
