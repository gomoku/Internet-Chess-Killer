//---------------------------------------------------------------------------

#ifndef TMainThreadObjectH
#define TMainThreadObjectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
#include "move.h"
#include "TEngine.h"
#include "TBoardCapture.h"
#include "TBoardRecognize.h"

class TMainThread : public TThread
{
private:
protected:
   void __fastcall Execute();
public:
   __fastcall TMainThread(bool CreateSuspended);
   __fastcall ~TMainThread();
   void __fastcall ShowResult();
   void __fastcall MakeBestMove();
   unsigned __int64 iComputerId;
   TEngine *Engine;
   bool last_key_w_state;
   bool last_key_b_state;
   mv_t BestMove;
};
//---------------------------------------------------------------------------
#endif
