//---------------------------------------------------------------------------
#include <Filectrl.hpp>
#include "TDebug.h"


#pragma hdrstop



//---------------------------------------------------------------------------

#pragma package(smart_init)

TDebug::TDebug()
{
  f = NULL;
  LastPosInited = false;
}

TDebug::~TDebug()
{
  if (f)
    CloseCurFile();
}

void TDebug::OpenNewFile()
{
  if (f)
    CloseCurFile();
  if (!DirectoryExists("log"))
    CreateDir("log");
	static int log_id = 1;
	while (log_id < 1000) {
		char file_name[32];
		sprintf(file_name,"log\\log.%03d", log_id);
		f = fopen(file_name, "r+");
		if (f) {
			fclose(f);
			log_id++;
			continue;
		}
		f = fopen(file_name,"a+");
		break;
	}
}

void TDebug::SaveString(char *s)
{
  fwrite(s,strlen(s),1,f);
  fwrite("\n",1,1,f);
}

bool TDebug::FindPosChanged(TFindPos *fp)
{
  bool e;
  if (LastPosInited)
    e = LastPos.IsEqual(fp);
  else
    e = false;
  LastPosInited = true;
  if (!e)
    LastPos = *fp;
  return !e;
}

void TDebug::SaveFindPos()
{
  char s[1000];
  LastPos.SaveToString(s);
  SaveString(s);
}

void TDebug::CloseCurFile()
{
  fclose(f);
  f = NULL;
}
