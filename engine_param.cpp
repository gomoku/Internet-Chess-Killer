//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "engine_param.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEngineParamForm *EngineParamForm;
//---------------------------------------------------------------------------
__fastcall TEngineParamForm::TEngineParamForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEngineParamForm::SelectEngineFileClick(TObject *Sender)
{
   if (OpenDialog->Execute())
      edEngineFileName->Text = OpenDialog->FileName;
}
//---------------------------------------------------------------------------
