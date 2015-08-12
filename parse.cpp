//---------------------------------------------------------------------------
#pragma hdrstop
#include "parse.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <stdlib.h>
#include "parse.h"

static const int max_word_len = 255;

char*  get_cur_word_str(char *start[], char end[])
{
	static char cur_word[max_word_len];
	int i = 0;

	while (*start[0] == ' ')
		(*start)++;

	if (end != NULL) {
		char *cur_end = strstr(*start,end);
		if (cur_end == NULL) {
			strcpy(cur_word,*start);
			*start = NULL;
		}
		else {
			int len = cur_end-*start;
			memcpy(cur_word,*start,len);
			cur_word[len] = '\0';
			*start += len;
		}
  	return cur_word;
	}
	while (*start[0] != ' ' && *start[0] != ';'&& *start[0] != '\0') {
		cur_word[i++] = *start[0];
		(*start)++;
	}
	cur_word[i] = '\0';

	while (*start[0] == ' ' || *start[0] == ';')
		(*start)++;

	if (*start[0] == '\0')
		*start = NULL;
	return cur_word;
}

int get_cur_word_int(char *start[])
{
	char *cur_word = get_cur_word_str(start);
	return atoi(cur_word);
}

bool string_equal(const char s1[], const char s2[]) {

   return strcmp(s1,s2) == 0;
}

