//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SquareViewWindow.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSquareViewForm *SquareViewForm;
//---------------------------------------------------------------------------
__fastcall TSquareViewForm::TSquareViewForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TSquareViewForm::PaintSquare(int x, int y, int colour)
{
  TRect r = gSquarePixels->CellRect(x,y);
  gSquarePixels->Canvas->Pen->Color = colour;
  gSquarePixels->Canvas->Brush->Color = colour;
  gSquarePixels->Canvas->Rectangle(r);
}

