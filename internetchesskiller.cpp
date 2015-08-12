//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("main.cpp", MainForm);
USEFORM("parameters.cpp", CommonSettingsForm);
USEFORM("about.cpp", AboutForm);
USEFORM("engine_param.cpp", EngineParamForm);
USEFORM("debug.cpp", BoardScanerWindow);
USEFORM("ScreenScanerForm.cpp", ScreenScanerWindow);
USEFORM("SquareViewWindow.cpp", SquareViewForm);
USEFORM("TimeWindow.cpp", TimeForm);
USEFORM("RegisterWindow.cpp", FormRegister);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->CreateForm(__classid(TMainForm), &MainForm);
     Application->CreateForm(__classid(TCommonSettingsForm), &CommonSettingsForm);
     Application->CreateForm(__classid(TAboutForm), &AboutForm);
     Application->CreateForm(__classid(TEngineParamForm), &EngineParamForm);
     Application->CreateForm(__classid(TBoardScanerWindow), &BoardScanerWindow);
     Application->CreateForm(__classid(TScreenScanerWindow), &ScreenScanerWindow);
     Application->CreateForm(__classid(TSquareViewForm), &SquareViewForm);
     Application->CreateForm(__classid(TTimeForm), &TimeForm);
     Application->CreateForm(__classid(TFormRegister), &FormRegister);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  catch (...)
  {
     try
     {
       throw Exception("");
     }
     catch (Exception &exception)
     {
       Application->ShowException(&exception);
     }
  }
  return 0;
}
//---------------------------------------------------------------------------
