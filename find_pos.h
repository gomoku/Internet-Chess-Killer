//---------------------------------------------------------------------------

#ifndef find_posH
#define find_posH
//---------------------------------------------------------------------------

#include "colour.h"
#include "util.h"

class TFindPos {
  public:
    TFindPos();
    void Init();
    void Reverse();
    void SetBit(int colour, int cnt);
    bool GetBit(int colour, int cnt);
    void SaveToString(char *s);
    bool IsEqual(TFindPos *fp);
    uint64 find_pos[ColourNb];
    bool PieceMoved(int colour, TFindPos * NewFindPos);
  private:
    uint64 ReverseUINT64(uint64 r);
};

#endif
