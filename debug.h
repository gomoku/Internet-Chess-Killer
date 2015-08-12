//---------------------------------------------------------------------------

#ifndef debugH
#define debugH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TBoardScanerWindow : public TForm
{
__published:	// IDE-managed Components
  TLabel *lbScreenPos;
  TLabel *lbBoardPos;
  TDrawGrid *gScreen;
  TDrawGrid *gBoard;
private:	// User declarations
public:		// User declarations
  __fastcall TBoardScanerWindow(TComponent* Owner);
  void PaintScreenSquare(int x, int y, int colour);
  void PaintBoardSquare(int x, int y, int colour);
};
//---------------------------------------------------------------------------
extern PACKAGE TBoardScanerWindow *BoardScanerWindow;
//---------------------------------------------------------------------------
#endif
