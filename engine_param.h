//---------------------------------------------------------------------------

#ifndef engine_paramH
#define engine_paramH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TEngineParamForm : public TForm
{
  __published:	// IDE-managed Components
  TGroupBox *GroupBox1;
  TGroupBox *GroupBox2;
  TBitBtn *BitBtn1;
  TBitBtn *BitBtn2;
  TEdit *edEngineFileName;
  TSpeedButton *SelectEngineFile;
  TComboBox *MultiPV;
  TLabel *Label1;
  TComboBox *edHashSize;
  TLabel *lbHashSize;
  TOpenDialog *OpenDialog;
  void __fastcall SelectEngineFileClick(TObject *Sender);
  private:	// User declarations
  public:		// User declarations
  __fastcall TEngineParamForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEngineParamForm *EngineParamForm;
//---------------------------------------------------------------------------
#endif
