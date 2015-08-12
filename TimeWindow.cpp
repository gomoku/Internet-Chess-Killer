//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TimeWindow.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TTimeForm *TimeForm;
//---------------------------------------------------------------------------
__fastcall TTimeForm::TTimeForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
