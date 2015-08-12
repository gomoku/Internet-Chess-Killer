#pragma hdrstop
#include <windows.h>
#include "TBoardCapture.h"
#include "ScreenScanerForm.h"
#include "square.h"

#define X_COORD(x)  ((x)&7)
#define Y_COORD(y)  ((y)>>3)

void TBoardCapture::MakeMove(mv_t move, int board_reversed)
{
  if (!BoardRecognized)
    return;
  int from = SQUARE_TO_64(MOVE_FROM(move));
  int to = SQUARE_TO_64(MOVE_TO(move));
  from = Y_COORD(from)*8 + (7-X_COORD(from));
  to = Y_COORD(to)*8 + (7-X_COORD(to));
  if (board_reversed) {
     from = 63-from;
     to = 63-to;
  }
  int x = BitmapX + (BoardSize*X_COORD(from))/8 + BoardSize/16;
  int y = BitmapY + (BoardSize*Y_COORD(from))/8 + BoardSize/16;
  SetCursorPos(x,y);
  mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
  Sleep(10);
  //mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
  x = BitmapX + (BoardSize*X_COORD(to))/8 + BoardSize/16;
  y = BitmapY + (BoardSize*Y_COORD(to))/8 + BoardSize/16;
  SetCursorPos(x,y);
  //mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
  Sleep(10);
  mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}

int ClearAlphaCanal (int x)
{
  return x & 0xFFFFFF;
}
TBoardCapture::TBoardCapture()
{
  BoardRecognized = false;
  HDC screendc = GetDC(NULL);
  int BitsPerPixel = GetDeviceCaps(screendc, BITSPIXEL);
  if (BitsPerPixel < 32) {
    ReleaseDC(0, screendc);
    throw "Screen resolution is wrong";
  }
  ScreenSizeX = GetDeviceCaps(screendc, HORZRES);
  ScreenSizeY = GetDeviceCaps(screendc, VERTRES);
  ReleaseDC(0, screendc);
  int count = ScreenSizeX * ScreenSizeY * 4;
  ScreenBuffer = (int *)malloc(count);
  if (ScreenBuffer == NULL)
    throw "Memory not allocated";

  memset(&BitmapInfo,sizeof(BITMAPINFO),0);
  BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  BitmapInfo.bmiHeader.biWidth = ScreenSizeX;
  BitmapInfo.bmiHeader.biHeight = -ScreenSizeY;
  BitmapInfo.bmiHeader.biPlanes = 1;
  BitmapInfo.bmiHeader.biBitCount = 32;
  BitmapInfo.bmiHeader.biCompression = BI_BITFIELDS;
  BitmapInfo.bmiHeader.biSizeImage = count;
  BitmapInfo.bmiHeader.biClrUsed = 0;
  BitmapInfo.bmiHeader.biClrImportant = 0;
}

TBoardCapture::~TBoardCapture()
{
  free(ScreenBuffer);
}

void TBoardCapture::CaptureBoard()
{
  Captured = false;
  if (BoardRecognized) {
    screendc = GetDC(NULL);
    dc = CreateCompatibleDC(screendc);
    BitmapSizeY = BitmapSizeX = BoardSize + 4;
    BitmapInfo.bmiHeader.biWidth = BitmapSizeX;
    BitmapInfo.bmiHeader.biHeight = -BitmapSizeY;
    BitmapInfo.bmiHeader.biSizeImage = BitmapSizeX * BitmapSizeY * 4;
    HBITMAP Bitmap = CreateCompatibleBitmap(screendc, BitmapSizeX, BitmapSizeY);
    Handle = SelectObject(dc, Bitmap);
    BitBlt(dc, 0, 0, BitmapSizeX, BitmapSizeY, screendc, BitmapX, BitmapY, SRCCOPY);
    GetDIBits(dc,Bitmap,0,BitmapSizeY,ScreenBuffer,&BitmapInfo,DIB_RGB_COLORS);
    StartPixel = ScreenBuffer + BitmapSizeX*2 + 2;
    Release();
    if (CheckForBoard(1,1,BitmapSizeX-2)) {
      Captured = true;
      return;
    }
  }
  BoardRecognized = false;
  screendc = GetDC(NULL);
  dc = CreateCompatibleDC(screendc);
  HBITMAP Bitmap;
  BitmapInfo.bmiHeader.biWidth = ScreenSizeX;
  BitmapInfo.bmiHeader.biHeight = -ScreenSizeY;
  BitmapInfo.bmiHeader.biSizeImage = ScreenSizeX * ScreenSizeY * 4;
  Bitmap = CreateCompatibleBitmap(screendc, ScreenSizeX, ScreenSizeY);
  Handle = SelectObject(dc, Bitmap);
  BitBlt(dc, 0, 0, ScreenSizeX, ScreenSizeY, screendc, 0, 0, SRCCOPY);
  GetDIBits(dc,Bitmap,0,ScreenSizeY,ScreenBuffer,&BitmapInfo,DIB_RGB_COLORS);
  Release();
  //Sleep(10);
  BitmapSizeX = ScreenSizeX;
  BitmapSizeY = ScreenSizeY;
  int xFirst;
  const int min_board_size = 100;
  for (int y=1; y<ScreenSizeY-1; y++) {
    int phase = 0;
    for (int x=1; x<ScreenSizeX-1; x++)
      switch (phase) {
        case 0:
          if (ColorIsBorder(ScreenBuffer[y*ScreenSizeX+x])) {
            phase = 1;
            xFirst = x;
          }
          break;
        case 1:
          if (!ColorIsBorder(ScreenBuffer[y*ScreenSizeX+x])) {
            if (x - xFirst >= min_board_size) {
              if (CheckForBoard(xFirst,y,x-xFirst)) {
                BoardRecognized = true;
                BitmapX = xFirst - 1;
                BitmapY = y - 1;
                StartPixel = ScreenBuffer + BitmapSizeX*2*y + xFirst + 1;
                BoardSize = x-xFirst-2;
                Captured = true;
                //Sleep(10);
                CaptureBoard();
                return;
              }
            }
            else
              phase = 0;
          }
      }
  }
}

void TBoardCapture::Release()
{
   DeleteObject(SelectObject(dc, Handle));
   DeleteDC(dc);
   ReleaseDC(0, screendc);
}

bool TBoardCapture::FindNotBlack(int x, int y, int size)
{
   int *start = ScreenBuffer + y*BitmapSizeX + x;
   int *cur = start;
   for (int i=0; i<size; i++) {
     if (!ColorIsBorder(*cur))
       return true;
     cur++;
   }
   cur += 1;
   if (ColorIsBorder(*cur))
     return true;
   cur = start;
   for (int i=0; i<size; i++) {
     if (!ColorIsBorder(*cur))
       return true;
     cur += BitmapSizeX;
   }
   cur += BitmapSizeX;
   if (ColorIsBorder(*cur))
     return true;
   return false;
}

bool TBoardCapture::FindBlack(int x, int y, int size)
{
   int *start = ScreenBuffer + y*BitmapSizeX + x;
   int *cur = start;
   for (int i=0; i<size; i++) {
     if (ColorIsBorder(*cur))
       return true;
     cur++;
   }
   cur = start;
   for (int i=0; i<size; i++) {
     if (ColorIsBorder(*cur))
       return true;
     cur += BitmapSizeX;
   }
   return false;
}

bool TBoardCapture::CheckForBoard(int x, int y, int size)
{
  if ((y + size > BitmapSizeY - 1) || (x + size > BitmapSizeX))
    return false;
  if (ProgramType == spinchat)
    return !FindBlack(x-1,y-1,size-4) && !FindNotBlack(x,y,size-2) && !ColorIsWhite(x-1,y-1);
  if (ProgramType==kurnik || ProgramType==winboard || ProgramType==bereg)
    return !FindBlack(x-1,y-1,size-4) && !FindNotBlack(x,y,size-2) && !ColorIsWhite(x+1,y+1);
  if (!FindBlack(x-1,y-1,size-4) && !FindNotBlack(x,y,size-2) && !FindBlack(x+1,y+1,size-4))
    return true;
  return false;
}

bool TBoardCapture::ColorIsWhite(int x, int y)
{
   int *cur = ScreenBuffer + y*BitmapSizeX + x;
   int r = *cur & 255;
   int g = (*cur>>8) & 255;
   int b = (*cur>>16) & 255;
   return r>250 && g>250 && b>250;
}

bool TBoardCapture::ColorIsBorder(int color)
{
  int res_color = ClearAlphaCanal(color);
  int r,g,b;
  switch (ProgramType) {
    case chessbase:
      return color == 0;
    case bereg:
      return color == 0 || (color == ((64<<16) + (64<<8) + 64));
    case chessassistant:
      return color == ((113<<16) + (111<<8) + 100) || color == ((133<<16) + (135<<8) + 140) || color == ((64<<16) + (64<<8) + 64);
    case instantchess: case kurnik:
      return ClearAlphaCanal(color) == 0;
    case winboard:
      return color == 0 || color == ((255<<16) + (255<<8));
    case chessgate:
      return ClearAlphaCanal(color) == (155<<16) + (155<<8) + 155;
    case spinchat:
      return (ClearAlphaCanal(color) == (255<<16) + (234<<8) + 157) ||
             (ClearAlphaCanal(color) == (164<<16) + (110<<8) + 48) ||
             (ClearAlphaCanal(color) == (80<<16) + (53<<8) + 23) ||
             (ClearAlphaCanal(color) == (140<<16) + (114<<8) + 77) ||
             (ClearAlphaCanal(color) == (178<<8) ||
             (ClearAlphaCanal(color) == (178<<16)));
    case chessclub_dasher:
      r = (res_color >> 16) & 255;
      g = (res_color >> 8) & 255;
      b = res_color & 255;
      return (r > 55 && r < 60) && (g > 33 && g <39) && (b > 17 && b < 23);
  }
  return false;
}

void TBoardCapture::ShowScreen()
{
  TForm *Form = ScreenScanerWindow;
  if (BoardRecognized) {
    for (int y=0; y<BoardSize; y++)
      for (int x=0; x<BoardSize; x++) {
        if (y < Form->ClientHeight && x < Form->ClientWidth) {
          int xx = StartPixel[y*BitmapSizeX+x];
          int r = xx & 255;
          int g = (xx>>8) & 255;
          int b = (xx>>16) & 255;
          Form->Canvas->Pixels[x][y] = (r<<16)|(g<<8)|b;
        }
      }
  }
  else {
    for (int y=0; y<Form->ClientHeight && y<BitmapSizeY; y++)
      for (int x=0; x<Form->ClientWidth && x<BitmapSizeX; x++) {
        int xx = ScreenBuffer[y*BitmapSizeX+x];
        int r = xx & 255;
        int g = (xx>>8) & 255;
        int b = (xx>>16) & 255;
        Form->Canvas->Pixels[x][y] = (r<<16)|(g<<8)|b;
      }
  }
}



