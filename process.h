//---------------------------------------------------------------------------

#ifndef processH
#define processH
//---------------------------------------------------------------------------
#include <windows.h>
#include <vcl.h>

class TProcess {
public:
  void Send(AnsiString Command);
  bool Get(char *buf);
  bool IsActive();
  TProcess(AnsiString FileName);
  ~TProcess();
private:
  HANDLE write_stdin, read_stdout;
};

#endif
