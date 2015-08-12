//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "debug.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBoardScanerWindow *BoardScanerWindow;
//---------------------------------------------------------------------------
__fastcall TBoardScanerWindow::TBoardScanerWindow(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TBoardScanerWindow::PaintScreenSquare(int x, int y, int colour)
{
  TRect r = gScreen->CellRect(x,y);
  gScreen->Canvas->Pen->Color = colour;
  gScreen->Canvas->Brush->Color = colour;
  gScreen->Canvas->Rectangle(r);
}

void TBoardScanerWindow::PaintBoardSquare(int x, int y, int colour)
{
  TRect r = gBoard->CellRect(x,y);
  gBoard->Canvas->Pen->Color = colour;
  gBoard->Canvas->Brush->Color = colour;
  gBoard->Canvas->Rectangle(r);
}

