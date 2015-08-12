//---------------------------------------------------------------------------

#ifndef parametersH
#define parametersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TCommonSettingsForm : public TForm
{
__published:	// IDE-managed Components
  TBitBtn *BitBtn1;
  TPanel *Panel;
  TBitBtn *BitBtn2;
  TComboBox *edProgramType;
  TLabel *Label4;
  TCheckBox *chDebug;
  TCheckBox *chThinkMode;
private:	// User declarations
public:		// User declarations
  __fastcall TCommonSettingsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCommonSettingsForm *CommonSettingsForm;
//---------------------------------------------------------------------------
#endif
