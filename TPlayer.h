//---------------------------------------------------------------------------

#ifndef TPlayerH
#define TPlayerH
//---------------------------------------------------------------------------
#include "windows.h"

class TPlayer {
  public:
    TPlayer();
    ~TPlayer();
    int AllTime;
    int IncTime;
    void OpenTime();
    void CloseTime();
    int GetRestTime();
    void Init();
    DWORD LastTime;
  private:
};
#endif
