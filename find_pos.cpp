//---------------------------------------------------------------------------


#pragma hdrstop

#include <string.h>
#include "find_pos.h"
#include "colour.h"

#define SET_BIT(x,i)  ((x)=((x)|(((uint64)1)<<(i))))

//---------------------------------------------------------------------------
const uint64 one_bit = 1;

void TFindPos::Init()
{
  find_pos[White] = 0;
  find_pos[Black] = 0;
}

TFindPos::TFindPos()
{
  Init();
}

void TFindPos::Reverse()
{
  for (int colour = 0; colour<ColourNb; colour++)
    find_pos[colour] = ReverseUINT64(find_pos[colour]);
}

void TFindPos::SetBit(int colour, int cnt)
{
  find_pos[colour] |= (one_bit<<cnt);
}

bool TFindPos::GetBit(int colour, int cnt)
{
  return (find_pos[colour]>>cnt) & 1;
}

uint64 TFindPos::ReverseUINT64(uint64 r)
{
  uint64 res = 0;
  for (int i=0; i<64; i++)
    res |= ((r>>(63-i))&1)<<i;
  return res;
}

void TFindPos::SaveToString(char *s)
{
  strcpy(s,"White:");
  for (int i=0; i<64; i++) {
    if (i%8 == 0)
      strcat(s,"\n");
    if (GetBit(White,i))
      strcat(s,"*");
    else
      strcat(s,"_");
  }
  strcat(s,"\nBlack:");
  for (int i=0; i<64; i++) {
    if (i%8 == 0)
      strcat(s,"\n");
    if (GetBit(Black,i))
      strcat(s,"*");
    else
      strcat(s,"_");
  }
}

bool TFindPos::IsEqual(TFindPos *fp)
{
  return (find_pos[White] == fp->find_pos[White]) && (find_pos[Black] == fp->find_pos[Black]);
}

bool TFindPos::PieceMoved(int colour, TFindPos * NewFindPos)
{
   return (find_pos[colour] & (~NewFindPos->find_pos[colour])) != 0;
}



