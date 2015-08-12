//---------------------------------------------------------------------------

#ifndef TBoardRecognizeH
#define TBoardRecognizeH

#include "board.h"
#include "TBoardCapture.h"
#include "find_pos.h"
//---------------------------------------------------------------------------

class TBoardRecognize {
  public:
    int ProgramType;
    TBoardRecognize();
    ~TBoardRecognize();
    void Recognize();
    void ShowPosition();
    void ShowSquare(int sq);
    TBoardCapture BoardCapture;
    TFindPos FindPos;
    bool SquareIsMarked(int sq);
  private:
    bool IsPixelWhite(int x);
    bool IsPixelBlack(int x);
    bool IsPixelMark(int x);
    void ConvertBoardToFindPos();
    bool CheckForWhite(int *cur, int size, int width, int depth);
    bool CheckForBlack(int *cur, int size, int width, int depth);
    board_t Board, StartBoard;
};

#endif
