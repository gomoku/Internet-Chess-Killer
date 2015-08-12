//---------------------------------------------------------------------------


#pragma hdrstop

#include "TEngine.h"
#include "parse.h"
#include "main.h"

#include "attack.h"
#include "hash.h"
#include "move_do.h"
#include "pawn.h"
#include "piece.h"
#include "protocol.h"
#include "random.h"
#include "square.h"
#include "my_util.h"
#include "value.h"
#include "vector.h"
#include "fen.h"
#include "move.h"
#include "move_legal.h"
#include "move_gen.h"

#include "find_pos.h"
#include "debug.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define X_COORD(x)  ((x)&7)
#define Y_COORD(y)  ((y)>>3)

int TEngine::SquareTo_64(int x)
{
  int File = SQUARE_FILE(x) - 4;
  int Rank = SQUARE_RANK(x) - 4;
  return (Rank*8) + 7-File;
}

TEngine::TEngine(AnsiString FileName)
{
  UCIInterface = new TUCIInterface(FileName);
  Reversed = true;
  AutoPlay = false;
  AutoPlayWhite = false;
  AutoPlayBlack = false;
  EventAutoPlay = false;
  LagTime = 0;
  move_locked = 0;
  Restrict = false;
  StartNewGameEvent = false;
  MoveLimit = 38;
  SwitchOfEngineWhenOpponentMove = false;
  board_from_fen(&StartBoard,StartFen);
}
TEngine::~TEngine()
{
  delete UCIInterface;
}

bool TEngine::ExtractNewState(TState * new_state)
{
  BoardRecognize.Recognize();
  PositionRecognized = false;
  if (BoardRecognize.BoardCapture.Captured) {
    FindPos = BoardRecognize.FindPos;
    if (Reversed)
      FindPos.Reverse();
    if (State.LastMove && (State.IsWhite() && LastFindPos.PieceMoved(White,&FindPos)
                           || !State.IsWhite() && LastFindPos.PieceMoved(Black,&FindPos))) {
       move_locked = 0;
       undo_t undo;
       move_do(&State.Board,State.LastMove,&undo);
       State.MoveHistory[State.MoveHistoryLen] = State.LastMove;
       State.MoveHistoryLen++;
       State.LastMove = 0;
    }
    if (IsDebug) {
      Debug.SaveString("tick");
      bool find_pos_changed = Debug.FindPosChanged(&FindPos);
      if (find_pos_changed) {
        Debug.SaveString("FindPosChanged");
        Debug.SaveFindPos();
      }
    }
    *new_state = State;
    if (State.Inited && GetNewState(new_state))
      PositionRecognized = true;
    else {
      new_state->SetNewGame();
      if (GetNewState(new_state)) {
        new_state->Inited = true;
        PositionRecognized = true;
        InitTimers();
        AutoPlay = false;
        AutoPlayWhite = false;
        AutoPlayBlack = false;
        StartNewGameEvent = true;
        move_locked = 0;
        if (IsDebug)
          Debug.OpenNewFile();
      }
      else {
        FindPos.Reverse();
        if (GetNewState(new_state)) {
          new_state->Inited = true;
          PositionRecognized = true;
          Reversed = !Reversed;
          InitTimers();
          AutoPlay = false;
          AutoPlayWhite = false;
          AutoPlayBlack = false;
          StartNewGameEvent = true;
          move_locked = 0;
          if (IsDebug)
            Debug.OpenNewFile();
        }
      }
    }
  }
  if (IsDebug) {
    char s[10000];
    sprintf(s,"recognized = %i",PositionRecognized);
    Debug.SaveString(s);
    Debug.SaveString("extract new state new_state:");
    new_state->SaveToStr(s);
    Debug.SaveString(s);
    Debug.SaveString("state:");
    State.SaveToStr(s);
    Debug.SaveString(s);
    Debug.SaveString("board:");
    board_save_to_str(&State.Board,s);
    Debug.SaveString(s);
  }
  return PositionRecognized;
}

void TEngine::Tick()
{
    TState new_state;
    PositionRecognized = ExtractNewState(&new_state);
    bool equal = new_state.IsEqual(&State);
    if (!PositionRecognized || equal) {
      if(equal) {
        if (move_locked)
          StartNewThink();
        move_locked = 0;
      }
      return;
    }
    Sleep(50);
    TState new_state2;
    PositionRecognized = ExtractNewState(&new_state2);
    if (!PositionRecognized)
      return;
    if (!new_state2.IsEqual(&new_state))
      return;
    if (move_locked) {
      if (new_state.GetLastMove()== move_locked)
        move_locked = 0;
      else
        return;
    }
    if (PositionRecognized && !new_state.IsEqual(&State) || EventAutoPlay) {
      if (!new_state.IsEqual(&State)) {
        if (new_state.IsWhite()) {
          BlackPlayer.CloseTime();
          WhitePlayer.OpenTime();
        }
        else {
          WhitePlayer.CloseTime();
          BlackPlayer.OpenTime();
        }
        LastFindPos = FindPos;
      }
      if (IsDebug) {
        Debug.SaveString("state changed, new state is:");
        char s[10000];
        new_state.SaveToStr(s);
        Debug.SaveString(s);
        Debug.SaveString("old state is:");
        State.SaveToStr(s);
        Debug.SaveString(s);
      }

      new_state.CopyTo(&State);
      StartNewThink();
      EventAutoPlay = false;
    }
}

void TEngine::StartNewThink()
{
  /*
  if (State.Len()>MoveLimit && (reg % MainForm->iComputerId != MainForm->iKey)) {
     MessageBox(Application->Handle,"Sorry, this program was restricted by 20 moves\n (for full version send request by email: dm-chess@yandex.ru)","Warning",MB_OK);
     return;
  }
  */
  undo_t undo;
  if (State.LastMove)
     move_do(&State.Board,State.LastMove,&undo);
  if (board_is_mate(&State.Board))
    return;
  if (State.LastMove)
     move_undo(&State.Board,State.LastMove,&undo);


  if (IsDebug) {
    Debug.SaveString("state for go:");
    char s[10000];
    State.SaveToStr(s);
    Debug.SaveString(s);
  }
  if (AutoPlayWhite && State.IsWhite() || AutoPlayBlack && !State.IsWhite()) {
    if (State.IsWhite()) {
      BlackPlayer.AllTime += LagTime;
      WhitePlayer.AllTime -= 400;
    }
    else {
      WhitePlayer.AllTime += LagTime;
      BlackPlayer.AllTime -= 400;
    }
    int wt = WhitePlayer.GetRestTime();
    if (wt < 10)
      wt = 100;
    int bt = BlackPlayer.GetRestTime();
    if (bt < 10)
      bt = 100;
    if (IsDebug) {
      char s[100];
      sprintf(s,"start go time white %i, black %i",WhitePlayer.AllTime,BlackPlayer.AllTime);
      Debug.SaveString(s);
    }
    UCIInterface->StartThink(&State,wt,bt,WhitePlayer.IncTime,BlackPlayer.IncTime);
    EventAutoPlay = false;
  }
  else {
    if (SwitchOfEngineWhenOpponentMove && (State.IsWhite() && AutoPlayBlack || !State.IsWhite() && AutoPlayWhite))
      return;
    if (IsDebug)
      Debug.SaveString("start go infinite");
    UCIInterface->StartThink(&State);
  }
  Sleep(10);
}

bool TEngine::GetNewState(TState * State)
{
   mv_t pv[3];
   if (SearchPos(&State->Board,&FindPos,pv)) {
      int pv_len = 0;
      for (int i=0; i<3; i++) {
        if (pv[i]==0)
          break;
        else
          pv_len++;
      }
      undo_t undo;
      switch (pv_len) {
         case 0:
            State->LastMove = 0;
            return true;
         case 1:
            State->LastMove = pv[0];
            return true;
         case 2:
            move_do(&State->Board,pv[0],&undo);
            State->MoveHistory[State->MoveHistoryLen] = pv[0];
            State->MoveHistoryLen++;
            State->LastMove = pv[1];
            return true;
      }
   }
   return false;
}

mv_t TEngine::TreatThinkResult()
{
  int move = UCIInterface->TreatEngineOutput(&State);
  if (move) {
    if (AutoPlayWhite && State.IsWhite() || AutoPlayBlack && !State.IsWhite()) {
      if (State.IsWhite()) {
        WhitePlayer.CloseTime();
        BlackPlayer.OpenTime();
      }
      else {
        BlackPlayer.CloseTime();
        WhitePlayer.OpenTime();
      }
      return move;
    }
    else
      StartNewThink();
  }
  return 0;
}

bool TEngine::Eval(board_t * Board, TFindPos * fp)
{
  const sq_t * ptr;
  int from;
  TFindPos fp_board;
  for (int colour = 0; colour < ColourNb; colour++) {
    for (ptr = &Board->piece[colour][0]; (from=*ptr) != SquareNone; ptr++)
      fp_board.SetBit(colour,SquareTo_64(from));
    for (ptr = &Board->pawn[colour][0]; (from=*ptr) != SquareNone; ptr++)
      fp_board.SetBit(colour,SquareTo_64(from));
  }
  return !memcmp(fp,&fp_board,sizeof(TFindPos));
}


bool TEngine::SearchPos(board_t * Board, TFindPos * fp, mv_t pv[])
{
  if (Eval(Board,fp)) {
    pv[0] = 0;
    return true;
  }
  list_t list;
  gen_legal_moves(&list,Board);
  for (int i=0; i<LIST_SIZE(&list); i++) {
    undo_t undo;
    mv_t move = LIST_MOVE(&list,i);
    move_do(Board,move,&undo);
    bool eval = Eval(Board,fp);
    move_undo(Board,move,&undo);
    if (eval) {
      pv[0] = move;
      pv[1] = 0;
      return true;
    }
  }
  //if (!AutoPlayWhite && !AutoPlayBlack)
    for (int i=0; i<LIST_SIZE(&list); i++) {
      undo_t undo;
      mv_t move = LIST_MOVE(&list,i);
      move_do(Board,move,&undo);
      list_t list2;
      gen_legal_moves(&list2,Board);
      for (int j=0; j<LIST_SIZE(&list2); j++) {
        undo_t undo2;
        mv_t move2 = LIST_MOVE(&list2,j);
        move_do(Board,move2,&undo2);
        bool eval = Eval(Board,fp);
        move_undo(Board,move2,&undo2);
        if (eval) {
          pv[0] = move;
          pv[1] = move2;
          pv[2] = 0;
          move_undo(Board,move,&undo);
          return true;
        }
      }
      move_undo(Board,move,&undo);
    }
  return false;
}

void TEngine::ShowPosition()
{
  undo_t undo;
  if (State.LastMove)
    move_do(&State.Board,State.LastMove,&undo);
  for (int y=0; y<8; y++)
    for (int x=0; x<8; x++) {
      int File = x+4;
      int Rank = y+4;
      int sq = Rank*16 + (15-File);
      if (Reversed)
        sq = 255 - sq;
      if (State.Board.square[sq] == Empty)
        BoardScanerWindow->PaintBoardSquare(x,y,clBlue);
      else {
        if (State.Board.square[sq] & WhiteFlag)
          BoardScanerWindow->PaintBoardSquare(x,y,clWhite);
        if (State.Board.square[sq] & BlackFlag)
          BoardScanerWindow->PaintBoardSquare(x,y,clBlack);
      }
    }
  if (State.LastMove)
    move_undo(&State.Board,State.LastMove,&undo);
}

void TEngine::InitTimers()
{
  WhitePlayer.AllTime = Time * 60000 + TimeSec * 1000;
  WhitePlayer.IncTime = IncTime * 1000;
  WhitePlayer.Init();
  BlackPlayer.AllTime = Time * 60000 + TimeSec * 1000;
  BlackPlayer.IncTime = IncTime * 1000;
  BlackPlayer.Init();
}


