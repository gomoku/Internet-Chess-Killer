//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <Registry.hpp>
#pragma hdrstop

#include "main.h"
#include "process.h"
#include "parameters.h"
#include "about.h"
#include "engine_param.h"
#include "debug.h"
#include "ScreenScanerForm.h"
#include "SquareViewWindow.h"
#include "TimeWindow.h"
#include "RegisterWindow.h"

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

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------

  const int maxServers = 11;
  int ServersId[maxServers] = {0,1,2,3,4,5,6,7,7,7,8};


AnsiString GetRegistryValue(AnsiString KeyName)
{
  AnsiString S;
  TRegistry *Registry = new TRegistry(KEY_READ);
  try
  {
    Registry->RootKey = HKEY_LOCAL_MACHINE;
    // False because we do not want to create it if it doesn't exist
    bool b = Registry->OpenKey(KeyName,false);
    S = Registry->ReadString("VideoBiosDate");
  }
  __finally
  {
    delete Registry;
  }
  return S;
}

void CenterForm(TForm *Form)
{
  int CenterX = Screen->Width/2;
  int CenterY = Screen->Height/2;
  Form->Top = CenterY - Form->Height/2;
  Form->Left = CenterX - Form->Width/2;
}

__fastcall TMainForm::TMainForm(TComponent* Owner)
  : TForm(Owner)
{

  ini = new TIniFile("internetchesskiller.ini");
  Top = ini->ReadInteger("MainForm","Top",200);
  Left = ini->ReadInteger("MainForm","Left",200);
  Height = ini->ReadInteger("MainForm","Height",163);
  Width = ini->ReadInteger("MainForm","Width",422);

  EngineFileName = ini->ReadString("Engine","FileName","");
  MultiPV = ini->ReadInteger("Engine","MultiPV",2);
  HashSize = ini->ReadInteger("Engine","HashSize",64);

  ProgramTypeIndex = ini->ReadInteger("Common","ProgramType",0);
  ProgramType = ServersId[ProgramTypeIndex];

  IsDebug = ini->ReadInteger("Common","Debug",0);
  SwitchOfEngineWhenOpponentMove = ini->ReadInteger("Common","SwitchOfEngineWhenOpponentMove",0);
  Key = ini->ReadString("Common","Key","");
  iKey = 0;
  iKey = _atoi64(Key.c_str());

  Time = ini->ReadInteger("Time","Time",5);
  TimeSec = ini->ReadInteger("Time","TimeSec",0);
  IncTime = ini->ReadInteger("Time","IncTime",0);
  LagTime = ini->ReadInteger("Time","LagTime",100);

  MainThread = NULL;
  SetTimeEvent = true;
  StartDebug = IsDebug;
  StopDebug = false;
  EventChangeTime = false;

  util_init();
  my_random_init();
  square_init();
  piece_init();
  pawn_init_bit();
  value_init();
  vector_init();
  attack_init();
  move_do_init();
  random_init();
  hash_init();

  /*
  //GetVolumeInformation()
  TCHAR RootPathName[250];
  TCHAR scBuf[200];
  TCHAR lpVolumeNameBuffer[200];
  DWORD dwVolumeSerialNumber, dwMaximumComponentLength;
  DWORD dwFileSystemFlags;
  TCHAR lpFileSystemNameBuffer[50];

  GetWindowsDirectory(RootPathName,100);
  RootPathName[2] = 0;

  DWORD MaximumComponentLength, FileSystemFlags;
  iComputerId = 0;
  if (GetVolumeInformation(RootPathName,lpVolumeNameBuffer,100,&dwVolumeSerialNumber,
     &dwMaximumComponentLength,&dwFileSystemFlags,lpFileSystemNameBuffer,20)) {
     unsigned __int64 rr = dwVolumeSerialNumber;
     iComputerId = rr * rr;
  }
  if (iComputerId == 0)
    iComputerId = 15271626003423485089;

  AnsiString ss = GetRegistryValue("HARDWARE\\DESCRIPTION\\System");

  unsigned __int64 rr = 0;
  for (int i=1; i<=ss.Length(); i++) {
    int l = ss[i];
    rr += l;
  }
  iComputerId = rr * rr * rr * rr + 1;
  */
  SetFullCaption();
}
//---------------------------------------------------------------------------

void TMainForm::SetFullCaption()
{
  AnsiString Title = "InternetChessKiller 1.71 ";
  Caption = Title;
}

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  ini->WriteInteger("MainForm","Top",Top);
  ini->WriteInteger("MainForm","Left",Left);
  ini->WriteInteger("MainForm","Height",Height);
  ini->WriteInteger("MainForm","Width",Width);
  ini->WriteString("Engine","FileName",EngineFileName);
  ini->WriteInteger("Engine","MultiPV",MultiPV);
  ini->WriteInteger("Engine","HashSize",HashSize);
  ini->WriteInteger("Common","ProgramType",ProgramTypeIndex);
  ini->WriteInteger("Common","Debug",IsDebug);
  ini->WriteInteger("Common","SwitchOfEngineWhenOpponentMove",SwitchOfEngineWhenOpponentMove);
  ini->WriteString("Common","Key",Key);
  ini->WriteInteger("Time","Time",Time);
  ini->WriteInteger("Time","TimeSec",TimeSec);
  ini->WriteInteger("Time","IncTime",IncTime);
  ini->WriteInteger("Time","LagTime",LagTime);
  DelScan();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Parameters1Click(TObject *Sender)
{
  AnsiString Servers[maxServers];

  Servers[0] = "www.playchess.com";
  Servers[1] = "www.chessplanet.ru";
  Servers[2] = "www.bereg.ru";
  Servers[3] = "www.instantchess.com";
  Servers[4] = "www.kurnik.org";
  Servers[5] = "winboard";
  Servers[6] = "www.chessgate.de";
  Servers[7] = "www.spinchat.de";
  Servers[8] = "www.superchat.at";
  Servers[9] = "www.aby.spinchat.de";
  Servers[10] = "www.radio7.spinchat.de";
  Servers[10] = "www.chessclub.com dasher (Classic Sets - Book)";

  bool IsRestricted[maxServers] =
       {false,false,false,true,false,false,false,false,false,false,false};

  unsigned __int64 Allowed[maxServers][3] =
       {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};


  int Used[maxServers];
  int cur = 0;
  for (int i = 0; i<maxServers; i++) {
    Used[i] = -1;
    if (IsRestricted[i]) {
       bool find = true;
       /*
       for (int j=0; j<3; j++)
         if (Allowed[i][j] == iComputerId) {
            find = true;
            break;
         }
       */
       if (find)
          Used[cur++] = i;
    }
    else
       Used[cur++] = i;
  }
  CommonSettingsForm->edProgramType->Items->Clear();
  int i = 0;
  int cur_index = 0;
  while (Used[i] >= 0 && i<maxServers) {
     if (ProgramTypeIndex == Used[i])
        cur_index = i;
     CommonSettingsForm->edProgramType->Items->Append(Servers[Used[i]]);
     i++;
  }

  CommonSettingsForm->edProgramType->ItemIndex = cur_index;
  CommonSettingsForm->chThinkMode->Checked = SwitchOfEngineWhenOpponentMove;
  CommonSettingsForm->chDebug->Checked = IsDebug;
  CenterForm(CommonSettingsForm);
  if (CommonSettingsForm->ShowModal() == mrOk) {
     ProgramTypeIndex = Used[CommonSettingsForm->edProgramType->ItemIndex];
     ProgramType = ServersId[ProgramTypeIndex];
     SwitchOfEngineWhenOpponentMove = CommonSettingsForm->chThinkMode->Checked;
     bool d = CommonSettingsForm->chDebug->Checked;
     if (d && !IsDebug)
       StartDebug = true;
     if (!d && IsDebug)
       StopDebug = true;
     IsDebug = d;
  }
}
//---------------------------------------------------------------------------
void TMainForm::DelScan()
{
  if (MainThread != NULL) {
    MainThread->Terminate();
    Sleep(100);
    delete MainThread;
    Sleep(100);
    delete Engine;
  }
}
//---------------------------------------------------------------------------
void TMainForm::CreateScan()
{
   try {
     if (EngineFileName != "" && FileExists(EngineFileName)) {
       Engine = new TEngine(EngineFileName);
       Memo->Lines->Clear();
       MainThread = new TMainThread(false);
     }
   }
   catch (...) {
      Application->MessageBox("Start engine error","Error");
   }
}

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
  CreateScan();
  SetWindowPos(MainForm->Handle,HWND_TOPMOST,Left,Top,Width,Height,SWP_SHOWWINDOW);
  /*
  HANDLE procHandle = GetCurrentProcess();
  SetPriorityClass(procHandle, HIGH_PRIORITY_CLASS);
  */
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::About2Click(TObject *Sender)
{
  CenterForm(AboutForm);
  AboutForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SpeedButton1Click(TObject *Sender)
{
  EngineParamForm->edEngineFileName->Text = EngineFileName;
  EngineParamForm->MultiPV->Text = StrToInt(MultiPV);
  EngineParamForm->edHashSize->Text = StrToInt(HashSize);
  CenterForm(EngineParamForm);
  if (EngineParamForm->ShowModal() == mrOk) {
    EngineFileName = EngineParamForm->edEngineFileName->Text;
    MultiPV = EngineParamForm->MultiPV->Text.ToInt();
    HashSize = EngineParamForm->edHashSize->Text.ToInt();
    DelScan();
    CreateScan();
  }
}
//------------------------------------------------------------
void __fastcall TMainForm::ShowDebugWindow1Click(TObject *Sender)
{
  BoardScanerWindow->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ScreenDebug1Click(TObject *Sender)
{
  ScreenScanerWindow->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SquareDebug1Click(TObject *Sender)
{
   SquareViewForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::btTimeDialogClick(TObject *Sender)
{
  CenterForm(TimeForm);
  TimeForm->edTime->Value = Time;
  TimeForm->edTimeSec->Value = TimeSec;
  TimeForm->edIncTime->Value = IncTime;
  TimeForm->edLagTime->Value = LagTime;
  if (TimeForm->ShowModal() == mrOk) {
    Time = TimeForm->edTime->Value;
    TimeSec = TimeForm->edTimeSec->Value;
    IncTime = TimeForm->edIncTime->Value;
    LagTime = TimeForm->edLagTime->Value;
    SetTimeEvent = true;
  }
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::csWhiteTimeUpClick(TObject *Sender)
{
  EventChangeTime = true;
  EventChangeTimeIsWhite = true;
  EventChangeTimeIsUp = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::csWhiteTimeDownClick(TObject *Sender)
{
  EventChangeTime = true;
  EventChangeTimeIsWhite = true;
  EventChangeTimeIsUp = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::csBlackTimeUpClick(TObject *Sender)
{
  EventChangeTime = true;
  EventChangeTimeIsWhite = false;
  EventChangeTimeIsUp = true;

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::csBlackTimeDownClick(TObject *Sender)
{
  EventChangeTime = true;
  EventChangeTimeIsWhite = false;
  EventChangeTimeIsUp = false;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Registration1Click(TObject *Sender)
{
  /*
  CenterForm(FormRegister);
  char s[100];
  _ui64toa(iComputerId,s,10);
  FormRegister->edComputerId->Text = s;
  FormRegister->edKey->Text = Key;
  if (FormRegister->ShowModal() == mrOk) {
    Key = FormRegister->edKey->Text;
    iKey = _atoi64(Key.c_str());
  }
  SetFullCaption();
  */
}
//---------------------------------------------------------------------------
