//---------------------------------------------------------------------------
#ifndef ScreenScanerFormH
#define ScreenScanerFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TScreenScanerWindow : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
  __fastcall TScreenScanerWindow(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TScreenScanerWindow *ScreenScanerWindow;
//---------------------------------------------------------------------------
#endif
