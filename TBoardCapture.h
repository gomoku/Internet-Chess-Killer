//---------------------------------------------------------------------------

#ifndef TBoardCaptureH
#define TBoardCaptureH

#include <vcl.h>
#include "move.h"
//---------------------------------------------------------------------------

const int chessbase = 0;
const int chessassistant = 1;
const int bereg = 2;
const int instantchess = 3;
const int kurnik = 4;
const int winboard = 5;
const int chessgate = 6;
const int spinchat = 7;
const int chessclub_dasher = 8;


class TBoardCapture {
  public:
    TBoardCapture();
    ~TBoardCapture();
    bool Captured;
    int ProgramType;
    int *StartPixel;
    int BitmapSizeX;
    int BoardSize;
    void CaptureBoard();
    void ShowScreen();
    void MakeMove(mv_t move, int board_reversed);
  private:
    bool BoardRecognized;
    int *ScreenBuffer;
    int ScreenSizeX, ScreenSizeY;
    HDC dc, screendc;
    HGDIOBJ Handle;
    BITMAPINFO BitmapInfo;
    int Dummy[4]; //HACK
    int BitmapX, BitmapY, BitmapSizeY;
    bool ColourIsBorder(int colour);
    bool FindBlack(int x, int y, int size);
    bool FindNotBlack(int x, int y, int size);
    bool CheckForBoard(int x, int y, int size);
    bool ColorIsBorder(int color);
    bool TBoardCapture::ColorIsWhite(int x, int y);
    void Release();
};

#endif
