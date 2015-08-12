//---------------------------------------------------------------------------

#ifndef TDebugH
#define TDebugH

#include "io.h"
#include "find_pos.h"
//---------------------------------------------------------------------------

class TDebug {
  public:
    TDebug();
    ~TDebug();
    void OpenNewFile();
    void SaveString(char *s);
    bool FindPosChanged(TFindPos *fp);
    void SaveFindPos();
    void CloseCurFile();
  private:
    FILE *f;
    bool LastPosInited;
    TFindPos LastPos;
};
#endif
