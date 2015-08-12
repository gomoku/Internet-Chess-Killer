//---------------------------------------------------------------------------
#pragma hdrstop

#include "TBoardRecognize.h"
#include "debug.h"
#include "find_pos.h"
#include "SquareViewWindow.h"

#pragma package(smart_init)

#define X_COORD(x)  ((x)&7)
#define Y_COORD(y)  ((y)>>3)

TBoardRecognize::TBoardRecognize()
{
}

TBoardRecognize::~TBoardRecognize()
{
}

void TBoardRecognize::ShowSquare(int sq)
{
  int sq64 = SQUARE_TO_64(sq);
  int size = BoardCapture.BoardSize/8;
  int delta = size*BoardCapture.BitmapSizeX;
  int corner = 1/16;
  int *start = BoardCapture.StartPixel + Y_COORD(sq64)*delta + X_COORD(sq64)*size +
                  BoardCapture.BitmapSizeX*corner + corner;
  int real_size = size - 2*corner;
  SquareViewForm->gSquarePixels->ColCount = real_size;
  SquareViewForm->gSquarePixels->RowCount = real_size;
  for (int i=0; i<real_size; i++) {
    int *cur = start;
    for (int j=0; j<real_size; j++) {
      if (IsPixelBlack(*cur))
        SquareViewForm->PaintSquare(i,j,clBlack);
      else {
        if (IsPixelWhite(*cur))
          SquareViewForm->PaintSquare(i,j,clWhite);
        else
          SquareViewForm->PaintSquare(i,j,clBlue);
      }
      if (IsPixelMark(*cur))
        SquareViewForm->PaintSquare(i,j,clRed);
      cur++;
    }
    start += BoardCapture.BitmapSizeX;
  }
}

bool TBoardRecognize::IsPixelBlack(int x)
{
  int r = x & 255;
  int g = (x>>8) & 255;
  int b = (x>>16) & 255;
  switch (ProgramType) {
     case chessbase:
        return r < 100 && g < 100 && b < 100;
     case bereg: case instantchess: case kurnik:
     case winboard: case chessassistant: case chessgate: case spinchat:
     case chessclub_dasher:
        return r < 50 && g < 50 && b < 50;
  }
  return false;
}



bool TBoardRecognize::IsPixelWhite(int x)
{
  int r = x & 255;
  int g = (x>>8) & 255;
  int b = (x>>16) & 255;
  switch (ProgramType) {
     case chessassistant:
        return r > 220 && g >220 && b > 220;
     case chessbase:
        return r > 190 && g >190 && b > 190;
     case bereg:
        return r > 240 && g >240 && b > 240;
     case instantchess:
        return r > 250 && g >250 && b > 250;
     case kurnik:
        return r > 250 && g >250 && b > 250;
     case winboard:
        return r > 190 && g >190 && b > 190;
     case chessgate:
        return r > 254 && g >254 && b > 254;
     case spinchat:
        return r > 190 && g >190 && b > 190;
     case chessclub_dasher:
        return r > 220 && g >220 && b > 220;
  }
  return false;
}

bool TBoardRecognize::IsPixelMark(int x)
{
  int b = x & 255;
  int g = (x>>8) & 255;
  int r = (x>>16) & 255;
  switch (ProgramType) {
     case instantchess:
        return r == 0 && g == 102 && b == 153;
  }
  return false;
}

void TBoardRecognize::Recognize()
{
  BoardCapture.CaptureBoard();
  if (BoardCapture.Captured) {
    FindPos.Init();
    ConvertBoardToFindPos();
  }
}

void TBoardRecognize::ShowPosition()
{
  if (BoardCapture.Captured) {
     for (int y=0; y<8; y++)
       for (int x=0; x<8; x++) {
          int sq = y*8+x;
          if (sq == 22) {
            int k; k++;
          }
          if (SquareIsMarked(y*8+x)) {
            BoardScanerWindow->PaintScreenSquare(x,y,clRed);
            continue;
          }
          if (FindPos.GetBit(White,y*8+x)) {
            BoardScanerWindow->PaintScreenSquare(x,y,clWhite);
            continue;
          }
          if (FindPos.GetBit(Black,y*8+x)) {
            BoardScanerWindow->PaintScreenSquare(x,y,clBlack);
            continue;
          }
          BoardScanerWindow->PaintScreenSquare(x,y,clBlue);
       }
  }
  else {
     for (int y=0; y<8; y++)
       for (int x=0; x<8; x++)
          BoardScanerWindow->PaintScreenSquare(x,y,clBlue);
  }
}

void TBoardRecognize::ConvertBoardToFindPos()
{
   int size = BoardCapture.BoardSize/8;
   int delta = size*BoardCapture.BitmapSizeX;
   int corner = size/8;
   int depth;
   int width;
   int recognize_type; //0 by pixels, 1 by square
   int black_max;
   bool calc_white = false;
   switch (ProgramType) {
      case chessassistant:
         width = size/4;
         depth = 2;
         corner = size/16;
         recognize_type = 1;
         break;
      case chessbase:
         width = size/4;
         depth = 3;
         corner = size/16;
         recognize_type = 2;
         black_max = 180;
         calc_white = true;
         break;
      case bereg:
         width = size/8;
         depth = 4;
         corner = size/16;
         recognize_type = 1;
         break;
      case instantchess:
         width = 7;
         depth = 4;
         corner = size/8;
         recognize_type = 1;
         black_max = 60;
         break;
      case kurnik:
         width = size/8;
         depth = width;
         corner = size/16;
         recognize_type = 1;
         break;
      case winboard:
         width = size/4;
         depth = 3;
         corner = size/16;
         recognize_type = 1;
         break;
      case chessgate:
         width = size/4;
         depth = 3;
         corner = size/16;
         recognize_type = 2;
         calc_white = false;
         black_max = 60;
         break;
      case spinchat:
         width = size/6;
         depth = 2;
         corner = size/16;
         recognize_type = 1;
         break;
      case chessclub_dasher:
         width = size/6;
         depth = 3;
         corner = size/16;
         recognize_type = 1;
         break;
   }
   FindPos.Init();
   for (int i=0; i<64; i++) {
      int *start = BoardCapture.StartPixel + Y_COORD(i)*delta + X_COORD(i)*size +
                  BoardCapture.BitmapSizeX*corner + corner;

      int real_size = size - 2*corner;
      switch (recognize_type) {
        case 1:
          do {
            int *cur = start;
            bool white_finded = false;
            for (int j=0; j<real_size-depth+1; j++) {
              if (CheckForWhite(cur,real_size,width,depth)) {
                FindPos.SetBit(White,i);
                white_finded = true;
                break;
              }
              cur += BoardCapture.BitmapSizeX;
            }
            if (!white_finded) {
              cur = start;
              for (int j=0; j<real_size-depth-1; j++) {
                if (CheckForBlack(cur,real_size,width,depth)) {
                  FindPos.SetBit(Black,i);
                  break;
                }
                cur += BoardCapture.BitmapSizeX;
              }
            }
          }
          while (false);
          break;
        case 2:
          do {
            int *temp_start = start;
            int black_cnt = 0;
            for (int y=0; y<real_size; y++) {
              int *cur = temp_start;
              for (int x=0; x<real_size; x++) {
                if (IsPixelBlack(*cur) || (calc_white && IsPixelWhite(*cur)))
                  black_cnt++;
                cur++;
              }
              temp_start += BoardCapture.BitmapSizeX;
            }
            if (black_cnt > black_max) {
              int *cur = start;
              int black_finded = false;
              for (int j=0; j<real_size-depth-1; j++) {
                if (CheckForBlack(cur,real_size,width,depth)) {
                  FindPos.SetBit(Black,i);
                  black_finded = true;
                  break;
                }
                cur += BoardCapture.BitmapSizeX;
              }
              if (!black_finded)
                FindPos.SetBit(White,i);
            }
          }
          while (false);
          break;
        case 3:
          break;
      }
   }
}

bool TBoardRecognize::CheckForWhite(int *cur, int size, int width, int depth)
{
  if (depth <= 0)
    return true;
  int phase = 0;
  int *start_ptr;
  for (int i=0; i<size; i++) {
    if (cur < BoardCapture.StartPixel || cur > BoardCapture.StartPixel + 1280*1024) {
      int k; k++;
    }
    switch (phase) {
      case 0:
         if (IsPixelWhite(*cur)) {
           start_ptr = cur;
           phase = 1;
         }
         break;
      case 1:
         if (!IsPixelWhite(*cur)) {
           if(cur-start_ptr>width) {
             if (CheckForWhite(start_ptr + BoardCapture.BitmapSizeX, cur-start_ptr, width, depth - 1))
               return true;
           }
           phase = 0;
         }
         break;
    }
    cur++;
  }

  if (phase == 1 && cur-start_ptr > width)
    if (CheckForWhite(start_ptr + BoardCapture.BitmapSizeX,cur-start_ptr, width, depth - 1))
      return true;

  return false;
}

bool TBoardRecognize::CheckForBlack(int *cur, int size, int width, int depth)
{
  if (depth <= 0)
    return true;
  int phase = 0;
  int *start_ptr;
  for (int i=0; i<size; i++) {
    switch (phase) {
      case 0:
         if (IsPixelBlack(*cur)) {
           start_ptr = cur;
           phase = 1;
         }
         break;
      case 1:
         if (!IsPixelBlack(*cur)) {
           if(cur-start_ptr>width) {
             if (CheckForBlack(start_ptr + BoardCapture.BitmapSizeX, cur-start_ptr, width, depth - 1))
               return true;
           }
           phase = 0;
         }
         break;
    }
    cur++;
  }

  if (phase == 1 && cur-start_ptr > width)
    if (CheckForBlack(start_ptr + BoardCapture.BitmapSizeX,cur-start_ptr, width, depth - 1))
      return true;

  return false;
}

bool TBoardRecognize::SquareIsMarked(int sq)
{
  int size = BoardCapture.BoardSize/8;
  int delta = size*BoardCapture.BitmapSizeX;
  int *start = BoardCapture.StartPixel + Y_COORD(sq)*delta + X_COORD(sq)*size + BoardCapture.BoardSize/16;
  for (int i=0; i<4; i++) {
    if (IsPixelMark(*start))
      return true;
    start += BoardCapture.BitmapSizeX;
  }
  return false;
}

