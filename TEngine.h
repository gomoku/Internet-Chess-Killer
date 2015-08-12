//---------------------------------------------------------------------------

#ifndef TEngineH
#define TEngineH
#include <vcl.h>
#include "process.h"
#include "move.h"
#include "board.h"
#include "move_do.h"
#include "find_pos.h"
#include "TBoardRecognize.h"
#include "TUCIInterface.h"
#include "TState.h"
#include "TPlayer.h"
#include "TDebug.h"
//---------------------------------------------------------------------------

class TEngine {
  public:
    TEngine(AnsiString FileName);
    ~TEngine();
    void Tick();
    void ShowPosition();
    mv_t TreatThinkResult();
    TBoardRecognize BoardRecognize;
    bool PositionRecognized;
    TState State;
    bool Reversed;
    bool AutoPlay;
    bool AutoPlayWhite;
    bool AutoPlayBlack;
    bool EventAutoPlay;
    int Time;
    int TimeSec;
    int IncTime;
    int LagTime;
    void InitTimers();
    TUCIInterface *UCIInterface;
    TPlayer WhitePlayer,BlackPlayer;
    TDebug Debug;
    bool IsDebug;
    bool Restrict;
    mv_t move_locked;
    void StartNewThink();
    bool StartNewGameEvent;
    bool SwitchOfEngineWhenOpponentMove;
    int MoveLimit;
  private:
    bool Eval(board_t * Board, TFindPos * fp);
    bool SearchPos(board_t * Board, TFindPos * fp, mv_t pv[]);
    bool GetNewState(TState * State);
    bool ExtractNewState(TState * new_state);
    int SquareTo_64(int x);
    board_t StartBoard;
    TFindPos FindPos, LastFindPos;
};

#endif
