//---------------------------------------------------------------------------
#ifndef TUCIInterfaceH
#define TUCIInterfaceH

#include <io.h>
#include <vcl.h>
#include "move.h"
#include "move_do.h"
#include "process.h"
#include "TState.h"

//---------------------------------------------------------------------------

const int max_pv_len = 100;
const int max_pv_cnt = 10;
const int max_score_len = 10;

typedef struct{
   char score[max_score_len];
   int pv_len;
   mv_t moves[max_pv_len];
   char moves_str[max_pv_len][8];
   undo_t undo[max_pv_len];
} line_t;

class TUCIInterface {
  public:
    TUCIInterface(AnsiString FileName);
    ~TUCIInterface();
    int MultiPV;
    void StartThink(TState * State);
    void StartThink(TState * State, int wtime, int btime, int winc, int binc);
    mv_t TreatEngineOutput(TState * State);
    void ShowPV();
    bool Started;
  private:
    void MakeGoString(TState * State, AnsiString *pos);
    mv_t ParseLine(TState * State, char *line);
    TProcess *Process;
    line_t PV[max_pv_cnt];
};

#endif
