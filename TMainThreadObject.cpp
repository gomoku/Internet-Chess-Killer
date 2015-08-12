//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TMainThreadObject.h"
#include "main.h"
#include "TBoardCapture.h"
#include "ScreenScanerForm.h"
#include "SquareViewWindow.h"
#include "debug.h"
#include "square.h"

#pragma package(smart_init)

void ConvertTimeToString(int time, AnsiString *s)
{
  time /= 1000;
  int h = time/3600;
  time -= (time/3600)*3600;
  int m = time/60;
  time -= (time/60)*60;
  int sec = time;
  char ss[100];
  sprintf(ss,"%i:%02i.%02i",h,m,sec);
  *s = ss;
}

__fastcall TMainThread::TMainThread(bool CreateSuspended)
        : TThread(CreateSuspended)
{
   Engine = MainForm->Engine;
   last_key_w_state = false;
   last_key_b_state = false;
}
__fastcall TMainThread::~TMainThread()
{
}

//---------------------------------------------------------------------------
void __fastcall TMainThread::Execute()
{
  int cnt2 = 0;
  int cnt3 = 0;
  int cnt4 = 0;
  int cnt5 = 0;
  while (!Terminated) {
    int cnt = 30;//ScanInterval;
    if (!Engine->BoardRecognize.BoardCapture.Captured)
      cnt = 200;
    if (Engine->AutoPlayWhite && Engine->State.IsWhite() || Engine->AutoPlayBlack && !Engine->State.IsWhite())
      cnt = 1000;
    while (cnt) {
      cnt -= 10;
      Sleep(10);
      BestMove = Engine->TreatThinkResult();
      if (BestMove && Engine->PositionRecognized) {
        Synchronize(MakeBestMove);
        Engine->Tick();
        BestMove = 0;
        break;
      }
      if (++cnt2 >= 10) {
        cnt2 = 0;
        Synchronize(ShowResult);
      }
    }
    if (Engine->move_locked) {
      if (++cnt3 > 50) {
        BestMove = Engine->move_locked;
        Synchronize(MakeBestMove);
        cnt3 = 0;
      }

    }
    /*
    if (Engine->State.Len() > 5) {
      cnt4++;
      if (Engine->WhitePlayer.LastTime != NULL || Engine->BlackPlayer.LastTime != NULL) {
        DWORD cur = GetTickCount();
        if (Engine->WhitePlayer.LastTime != NULL && (cur - Engine->WhitePlayer.LastTime) > 3000)
          exit(0);
        if (Engine->BlackPlayer.LastTime != NULL && (cur - Engine->BlackPlayer.LastTime) > 3000)
          exit(0);
        if (cnt4 > 10) {
          TCHAR RootPathName[250];
          TCHAR scBuf[200];
          TCHAR lpVolumeNameBuffer[200];
          DWORD dwVolumeSerialNumber, dwMaximumComponentLength;
          DWORD dwFileSystemFlags;
          TCHAR lpFileSystemNameBuffer[50];

          GetWindowsDirectory(RootPathName,100);
          RootPathName[2] = 0;

          DWORD MaximumComponentLength, FileSystemFlags;
          if (GetVolumeInformation(RootPathName,lpVolumeNameBuffer,100,&dwVolumeSerialNumber,
           &dwMaximumComponentLength,&dwFileSystemFlags,lpFileSystemNameBuffer,20)) {
             unsigned __int64 rr = dwVolumeSerialNumber;
             iComputerId = rr * rr;
             if (iComputerId != MainForm->iComputerId)
             exit(0);
          }
          cnt4 = 0;
          Engine->MoveLimit = 38;
        }
      }
    }
    */
    Engine->Tick();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainThread::MakeBestMove()
{
  Engine->BoardRecognize.BoardCapture.MakeMove(BestMove,Engine->Reversed);
  Engine->State.CommitLastMove();
  if (BestMove != Engine->State.GetLastMove()) {
    undo_t undo;
    move_do(&Engine->State.Board,BestMove,&undo);
    Engine->State.MoveHistory[Engine->State.MoveHistoryLen] = BestMove;
    Engine->State.MoveHistoryLen++;
    Engine->State.LastMove = 0;
    Engine->move_locked = BestMove;
  }
}

void __fastcall TMainThread::ShowResult()
{
  Engine->UCIInterface->ShowPV();
  Engine->SwitchOfEngineWhenOpponentMove = MainForm->SwitchOfEngineWhenOpponentMove; 
  Engine->BoardRecognize.ProgramType = Engine->BoardRecognize.BoardCapture.ProgramType =
      MainForm->ProgramType;
  MainForm->shBoardCaptured->Brush->Color = Engine->BoardRecognize.BoardCapture.Captured ? clGreen : clRed;
  MainForm->shPosRecognized->Brush->Color = Engine->PositionRecognized ? clGreen : clRed;
  if (ScreenScanerWindow->Visible)
    Engine->BoardRecognize.BoardCapture.ShowScreen();
  if (SquareViewForm->Visible && Engine->BoardRecognize.BoardCapture.Captured) {
    int sq = square_from_string(SquareViewForm->edSquare->Text.c_str());
    if (sq != SquareNone)
      Engine->BoardRecognize.ShowSquare(sq);
  }
  if (BoardScanerWindow->Visible) {
    Engine->BoardRecognize.ShowPosition();
    Engine->ShowPosition();
  }

  if (MainForm->StartDebug) {
    Engine->Debug.OpenNewFile();
    Engine->IsDebug = true;
    MainForm->StartDebug = false;
  }
  if (MainForm->StopDebug) {
    Engine->Debug.CloseCurFile();
    Engine->IsDebug = false;
    MainForm->StopDebug = false;
  }

  if (Engine->StartNewGameEvent && Engine->State.Len() == 0) {
    MainForm->chWhite->Checked = false;
    MainForm->chBlack->Checked = false;
    Engine->StartNewGameEvent = false;
  }
  bool key_w_state = GetKeyState('W') >> 15;
  if (key_w_state && !last_key_w_state)
    MainForm->chWhite->Checked = !MainForm->chWhite->Checked;
  last_key_w_state = key_w_state;

  if (Engine->BoardRecognize.BoardCapture.Captured) {
    if (MainForm->chWhite->Checked != Engine->AutoPlayWhite) {
      Engine->AutoPlayWhite = MainForm->chWhite->Checked;
      if (Engine->State.IsWhite())
        Engine->StartNewThink() ;
    }
  }

  bool key_b_state = GetKeyState('B') >> 15;
  if (key_b_state && !last_key_b_state)
    MainForm->chBlack->Checked = !MainForm->chBlack->Checked;
  last_key_b_state = key_b_state;

  if (Engine->BoardRecognize.BoardCapture.Captured) {
    if (MainForm->chBlack->Checked != Engine->AutoPlayBlack) {
      Engine->AutoPlayBlack = MainForm->chBlack->Checked;
      if (!Engine->State.IsWhite())
        Engine->StartNewThink() ;
    }
  }

  SHORT key_a = GetKeyState('A');
  if (key_a >> 15)
    Engine->WhitePlayer.AllTime += 1000;

  SHORT key_z = GetKeyState('Z');
  if (key_z >> 15)
    Engine->WhitePlayer.AllTime -= 1000;

  SHORT key_k = GetKeyState('S');
  if (key_k >> 15)
    Engine->BlackPlayer.AllTime += 1000;

  SHORT key_m = GetKeyState('X');
  if (key_m >> 15)
    Engine->BlackPlayer.AllTime -= 1000;

  Engine->LagTime = MainForm->LagTime;
  if (MainForm->EventChangeTime) {
    if (MainForm->EventChangeTimeIsWhite) {
      if (MainForm->EventChangeTimeIsUp)
        Engine->WhitePlayer.AllTime += 1000;
      else
        Engine->WhitePlayer.AllTime -= 1000;
    }
    else {
      if (MainForm->EventChangeTimeIsUp)
        Engine->BlackPlayer.AllTime += 1000;
      else
        Engine->BlackPlayer.AllTime -= 1000;
    }
    MainForm->EventChangeTime = false;
  }
  //time
  if (MainForm->SetTimeEvent) {
    Engine->Time = MainForm->Time;
    Engine->TimeSec = MainForm->TimeSec;
    Engine->IncTime = MainForm->IncTime;
    Engine->InitTimers() ;
    MainForm->SetTimeEvent = false;
  }
  int time = Engine->WhitePlayer.GetRestTime();
  AnsiString s;
  if (time<=0)
    s = "------";
  else
    ConvertTimeToString(time,&s);
  MainForm->edWhiteTime->Text = s;
  time = Engine->BlackPlayer.GetRestTime();
  if (time<=0)
    s = "------";
  else
    ConvertTimeToString(time,&s);
  MainForm->edBlackTime->Text = s;
  if (Engine->State.LastMove) {
    char move_str[6] = "";
    move_to_string(&Engine->State.Board,Engine->State.LastMove,move_str,6);
    MainForm->edLastMove->Text = move_str;
  }
  else
    MainForm->edLastMove->Text = "";
}


