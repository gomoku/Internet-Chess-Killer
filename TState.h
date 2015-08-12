//---------------------------------------------------------------------------
#ifndef TStateH
#define TStateH
#include <vcl.h>
#include "move.h"
#include "board.h"
//---------------------------------------------------------------------------

const max_game_len = 1000;

class TState {
  public:
    TState();
    ~TState();
    mv_t MoveHistory[max_game_len];
    int MoveHistoryLen;
    mv_t LastMove;
    bool IsEqual(TState * state);
    void CopyTo(TState * state);
    void SetNewGame();
    void AddMove(mv_t move);
    bool IsWhite();
    bool Inited;
    int Len();
    void SaveToStr(char *s);
    void CommitLastMove();
    mv_t GetLastMove();
    board_t Board;
};

#endif
