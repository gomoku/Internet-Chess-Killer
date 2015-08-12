//---------------------------------------------------------------------------

#ifndef parseH
#define parseH
//---------------------------------------------------------------------------
#include <string.h>

extern char*  get_cur_word_str(char *start[], char end[]=NULL);
extern int    get_cur_word_int(char *ptr_start[]);
extern bool   string_equal(const char s1[], const char s2[]);

#endif
