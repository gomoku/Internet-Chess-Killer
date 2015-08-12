//---------------------------------------------------------------------------

#ifndef SquareViewWindowH
#define SquareViewWindowH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TSquareViewForm : public TForm
{
__published:	// IDE-managed Components
  TStringGrid *gSquarePixels;
  TEdit *edSquare;
private:	// User declarations
public:		// User declarations
  __fastcall TSquareViewForm(TComponent* Owner);
  void PaintSquare(int x, int y, int colour);
};
//---------------------------------------------------------------------------
extern PACKAGE TSquareViewForm *SquareViewForm;
//---------------------------------------------------------------------------
#endif
