//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <inifiles.hpp>

//---------------------------------------------------------------------------
#include "TMainThreadObject.h"
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include "CSPIN.h"

const unsigned __int64 reg = 128934701231203412345234789;

class TMainForm : public TForm
{
__published:	// IDE-managed Components
  TMainMenu *MainMenu1;
  TMenuItem *File1;
  TMemo *Memo;
  TMenuItem *Parameters1;
  TMenuItem *About1;
  TMenuItem *About2;
  TGroupBox *GroupBox1;
  TSpeedButton *SpeedButton1;
  TGroupBox *GroupBox2;
  TLabel *lbEngineName;
  TEdit *edLastMove;
  TShape *shBoardCaptured;
  TMenuItem *Debug1;
  TMenuItem *ShowDebugWindow1;
  TMenuItem *SquareDebug1;
  TMenuItem *ScreenDebug1;
  TShape *shPosRecognized;
  TGroupBox *GroupBox3;
  TEdit *edWhiteTime;
  TShape *shWhite;
  TEdit *edBlackTime;
  TShape *shBlack;
  TSpeedButton *btTimeDialog;
  TGroupBox *GroupBox4;
  TCSpinButton *csWhiteTime;
  TCSpinButton *csBlackTime;
  TCheckBox *chWhite;
  TCheckBox *chBlack;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall Parameters1Click(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall About2Click(TObject *Sender);
  void __fastcall SpeedButton1Click(TObject *Sender);
  void __fastcall ShowDebugWindow1Click(TObject *Sender);
  void __fastcall ScreenDebug1Click(TObject *Sender);
  void __fastcall SquareDebug1Click(TObject *Sender);
  void __fastcall btTimeDialogClick(TObject *Sender);
  void __fastcall csWhiteTimeUpClick(TObject *Sender);
  void __fastcall csWhiteTimeDownClick(TObject *Sender);
  void __fastcall csBlackTimeUpClick(TObject *Sender);
  void __fastcall csBlackTimeDownClick(TObject *Sender);
  void __fastcall Registration1Click(TObject *Sender);
private:	// User declarations
  void DelScan();
  void CreateScan();
public:		// User declarations
   __fastcall TMainForm(TComponent* Owner);
   void SetFullCaption();
   TMainThread *MainThread;
   TEngine *Engine;
   AnsiString EngineFileName;
   //unsigned __int64 iComputerId;
   AnsiString ComputerId;
   AnsiString Key;
   __int64 iKey;
   TIniFile *ini;
   int MultiPV;
   int HashSize;
   int ProgramType;
   int ProgramTypeIndex;
   bool SwitchOfEngineWhenOpponentMove;

   int Time;
   int TimeSec;
   int IncTime;
   int LagTime;
   bool SetTimeEvent;

   bool IsDebug;
   bool StartDebug;
   bool StopDebug;
   bool EventChangeTime;
   bool EventChangeTimeIsWhite;
   bool EventChangeTimeIsUp;

};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
