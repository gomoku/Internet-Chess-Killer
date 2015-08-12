//---------------------------------------------------------------------------
#pragma hdrstop

#include "TPlayer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TPlayer::TPlayer()
{
  Init();
}

TPlayer::~TPlayer()
{
}

void TPlayer::Init()
{
  LastTime = NULL;
}

void TPlayer::OpenTime()
{
  AllTime += IncTime;
  LastTime = GetTickCount();
}

void TPlayer::CloseTime()
{
  if (LastTime != NULL) {
    DWORD NewTime = GetTickCount();
    AllTime -= (NewTime - LastTime);
    LastTime = NULL;
  }
}

int TPlayer::GetRestTime()
{
  if (LastTime == NULL)
    return AllTime;
  DWORD NewTime = GetTickCount();
  int cnt = AllTime - (NewTime - LastTime);
  return cnt;
}
