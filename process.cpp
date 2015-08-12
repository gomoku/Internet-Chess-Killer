//---------------------------------------------------------------------------
#pragma hdrstop
#include "process.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
bool TProcess::Get(char *buf)
{
  DWORD avail;
  if (!PeekNamedPipe(read_stdout,NULL,NULL,NULL,&avail,NULL))
    throw "Error peek from pipe";
  if (avail > 0) {
    DWORD read;
    if (!ReadFile(read_stdout,buf,avail,&read,NULL))
      throw "Error read from pipe";
    if (read < avail)
      throw "Error read from pipe";
    while (buf[read-1] != '\n') {
       Sleep(10);
       if (!PeekNamedPipe(read_stdout,NULL,NULL,NULL,&avail,NULL))
         throw "Error peek from pipe";
       if (avail == 0)
         continue;
       DWORD read1;
       if (!ReadFile(read_stdout,buf + read,avail,&read1,NULL))
         throw "Error read from pipe";
       if (read1 < avail)
         throw "Error read from pipe";
       read += read1;
    }
    buf[read] = 0;
    return true;
  }
  return false;
}

void TProcess::Send(AnsiString Command)
{
  Command += "\n";
  DWORD Len = Command.Length();
  DWORD Writed;
  if (!WriteFile(write_stdin,Command.c_str(),Len,&Writed,NULL))
    throw "Can't write into pipe";
  if (Writed < Len)
    throw "Can't write into pipe";
}

TProcess::TProcess(AnsiString FileName)
{
  SECURITY_ATTRIBUTES sa;
  SECURITY_DESCRIPTOR sd;
  InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
  SetSecurityDescriptorDacl(&sd, true, NULL, false);
  sa.lpSecurityDescriptor = &sd;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = true;
  HANDLE newstdin,newstdout;
  if (!CreatePipe(&newstdin,&write_stdin,&sa,0))
    throw "Error create pipe";
  if (!CreatePipe(&read_stdout,&newstdout,&sa,0)) {
    CloseHandle(newstdin);
    CloseHandle(write_stdin);
    throw "Error create pipe";
  }
  STARTUPINFO si;
  GetStartupInfo(&si);
  si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;
  si.hStdOutput = newstdout;
  si.hStdError = newstdout;
  si.hStdInput = newstdin;
  PROCESS_INFORMATION pi;
  if (!CreateProcess(FileName.c_str(),NULL,NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi))
    throw "Create process error";
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  CloseHandle(newstdout);
  CloseHandle(newstdin);
}
TProcess::~TProcess()
{
  CloseHandle(read_stdout);
  CloseHandle(write_stdin);
}
