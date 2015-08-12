//---------------------------------------------------------------------------
#pragma hdrstop
#include "TState.h"
#include "fen.h"
#include "move_do.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TState::TState()
{
  SetNewGame();
  Inited = false;
}

TState::~TState()
{
}

bool TState::IsWhite()
{
  int t = MoveHistoryLen;
  if (LastMove)
    t++;
  return (t%2) == 0;
}

void TState::AddMove(mv_t move)
{
  if (LastMove) {
    MoveHistory[MoveHistoryLen++] = LastMove;
    undo_t undo;
    move_do(&Board,LastMove,&undo);
  }
  LastMove = move;
}

void TState::SetNewGame()
{
  MoveHistoryLen = 0;
  board_from_fen(&Board,StartFen);
  LastMove = 0;
}

bool TState::IsEqual(TState * State)
{
  if (Inited != State->Inited)
    return false;
  int this_moves_len = MoveHistoryLen;
  mv_t this_moves[max_game_len];
  for (int i=0; i<this_moves_len; i++)
    this_moves[i] = MoveHistory[i];
  if (LastMove)
    this_moves[this_moves_len++] = LastMove;

  int others_moves_len = State->MoveHistoryLen;
  mv_t others_moves[max_game_len];
  for (int i=0; i<others_moves_len; i++)
    others_moves[i] = State->MoveHistory[i];
  if (State->LastMove)
    others_moves[others_moves_len++] = State->LastMove;

  if (this_moves_len != others_moves_len)
    return false;
  for (int i=0; i<this_moves_len; i++)
    if (this_moves[i]!=others_moves[i])
      return false;
  return true;
}

void TState::CopyTo(TState * State)
{
  State->Inited = Inited;
  State->LastMove = LastMove;
  State->MoveHistoryLen = MoveHistoryLen;
  for (int i=0; i<MoveHistoryLen; i++)
    State->MoveHistory[i] = MoveHistory[i];
  State->Board = Board;
}

int TState::Len()
{
  int len = MoveHistoryLen;
  if (LastMove)
    len++;
  return len;
}

void TState::SaveToStr(char *s)
{
  sprintf(s,"len:%i, inited:%i",Len(),Inited);
  char move_str[6];
  for (int i=0; i<MoveHistoryLen; i++) {
    move_to_string(MoveHistory[i],move_str,6);
    strcat(s,move_str);
    strcat(s,",");
  }
  if (LastMove) {
    strcat(s,"_");
    move_to_string(LastMove,move_str,6);
    strcat(s,move_str);
  }
}

mv_t TState::GetLastMove()
{
  if (LastMove)
    return LastMove;
  if (!MoveHistoryLen)
    return 0;
  return MoveHistory[MoveHistoryLen-1];
}

void TState::CommitLastMove()
{
  if (LastMove) {
    MoveHistory[MoveHistoryLen++] = LastMove;
    undo_t undo;
    move_do(&Board,LastMove,&undo);
  }
  LastMove = 0;
}



