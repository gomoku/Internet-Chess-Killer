//---------------------------------------------------------------------------
#pragma hdrstop

#include "TUCIInterface.h"
#include "move.h"
#include "main.h"
#include "parse.h"
#include "move_do.h"
#include "move_legal.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TUCIInterface::TUCIInterface(AnsiString FileName)
{
  Started = false;
  MultiPV = MainForm->MultiPV;
  Process = new TProcess(FileName);
  Process->Send("uci");
  char buf[10000];
  do {
    Sleep(100);
    if (Process->Get(buf)) {
      char *next_line = buf;
      while (true) {
        char *cur_line = next_line;
        if (cur_line[0] == 0)
          break;
        char *end = strstr(cur_line,"\r\n");
        end[0] = 0;
        end[1] = 0;
        next_line = end + 2;
        char *cur_word;
        while (cur_line) {
          cur_word = get_cur_word_str(&cur_line);
          int pos;
          if (string_equal(cur_word,"id")) {
            cur_word = get_cur_word_str(&cur_line);
            if (string_equal(cur_word,"name")) {
              AnsiString EngineName = cur_line;
              MainForm->lbEngineName->Caption = EngineName;
            }
          }
          if (string_equal(cur_word,"uciok")) {
            if (MainForm->MultiPV > 1) {
              Process->Send("setoption name MultiPV value " + IntToStr(MultiPV));
              Sleep(100);
              Process->Get(buf);
            }
            Process->Send("setoption name Hash value " + IntToStr(MainForm->HashSize));
            return;
          }
        }
      }
    }
  }
  while (true);
}

TUCIInterface::~TUCIInterface()
{
  Process->Send("stop");
  Process->Send("quit");
  Sleep(100);
  delete Process;
}

mv_t TUCIInterface::TreatEngineOutput(TState * State)
{
  char move_str[6] = "";
  undo_t last_move_undo;
  int best_move = 0;
  if (State->LastMove) {
     move_to_string(&State->Board,State->LastMove,move_str,6);
     move_do(&State->Board,State->LastMove,&last_move_undo);
  }
  char buf[100000];
  char *next_line = buf;
  if (Process->Get(buf)) {
    while (next_line[0] != 0) {
      char *cur_line = next_line;
      char *end;
      end = strstr(cur_line,"\r\n");
      if (end) {
        end[0] = 0;
        end[1] = 0;
        if (end[-1] == '\r')
          end[-1] = 0;
        next_line = end + 2;
        best_move = ParseLine(State,cur_line);
        if (best_move) {
          if (State->LastMove)
            move_undo(&State->Board,State->LastMove,&last_move_undo);
          Started = false;
          return best_move;
        }
      }
      else
        break;
    }
  }
  if (State->LastMove)
    move_undo(&State->Board,State->LastMove,&last_move_undo);
  return 0;
}

void TUCIInterface::ShowPV()
{
  MainForm->Memo->Lines->Clear();
  if (Started)
    for (int i=0; i<MainForm->MultiPV; i++) {
      AnsiString s;
      if (PV[i].pv_len > 0) {
        s += PV[i].score;
        s += " ";
      }
      for (int j=0; j<PV[i].pv_len; j++) {
         char move_s[8];
         s += PV[i].moves_str[j];
           s  += " ";
      }
      MainForm->Memo->Lines->Append(s);
    }
}
mv_t TUCIInterface::ParseLine(TState * State, char *line)
{
  board_t backup_board = State->Board;
  int cur_colour = State->Board.turn;
  char *cur_word = get_cur_word_str(&line);
  if (string_equal(cur_word,"bestmove")) {
    cur_word = get_cur_word_str(&line);
    int move = move_from_string(cur_word,&State->Board);
    return move;
  }
  if (!string_equal(cur_word,"info"))
    return 0;
  bool pv_mode = false;
  int pv_num = 0;

  bool pv_exists = false;

  while (line) {
      cur_word = get_cur_word_str(&line);
      switch (strlen(cur_word)) {
         case 2:
           if (string_equal(cur_word,"pv")) {
              PV[pv_num].pv_len = 0;
              pv_mode = true;
              pv_exists = true;
              break;
           }
           break;
         case 3:
           if (string_equal(cur_word,"nps")) {
              get_cur_word_int(&line);
              break;
           }
           break;
         case 4:
           if (string_equal(cur_word,"time")) {
              get_cur_word_int(&line);
              break;
           }
           if (pv_mode) {
             int pv_len = PV[pv_num].pv_len;
             mv_t m = move_from_string(cur_word,&State->Board);
             if (!move_is_pseudo(m,&State->Board) || !pseudo_is_legal(m,&State->Board)) {
                for (int i=PV[pv_num].pv_len-1; i>=0; i--)
                  move_undo(&State->Board,PV[pv_num].moves[i],&PV[pv_num].undo[i]);
                return 0;
             }
             move_to_string(&State->Board,m,PV[pv_num].moves_str[pv_len],8);
             move_do(&State->Board,m,&PV[pv_num].undo[pv_len]);
             PV[pv_num].moves[pv_len] = m;
             PV[pv_num].pv_len++;
           }
           break;
         case 5:
           if (string_equal(cur_word,"depth")) {
              get_cur_word_int(&line);
              break;
           }
           if (string_equal(cur_word,"nodes")) {
              get_cur_word_int(&line);
              break;
           }
           if (string_equal(cur_word,"score")) {
              cur_word = get_cur_word_str(&line);
              if (string_equal(cur_word,"cp")) {
                 int score = get_cur_word_int(&line);
                 if (cur_colour == Black)
                   score = -score;
                 float f = score;
                 f /= 100;
                 sprintf(PV[pv_num].score,"(%-.2f)",f);
                 break;
              }
              if (string_equal(cur_word,"mate")) {
                 cur_word = get_cur_word_str(&line);
                 strcpy(PV[pv_num].score,"mate ");
                 strcat(PV[pv_num].score,cur_word);
                 break;
              }
              if (string_equal(cur_word,"lowerbound")) {
                 cur_word = get_cur_word_str(&line);
                 strcpy(PV[pv_num].score,"lowerbound ");
                 strcat(PV[pv_num].score,cur_word);
                 break;
              }
              if (string_equal(cur_word,"upperbound")) {
                 cur_word = get_cur_word_str(&line);
                 strcpy(PV[pv_num].score,"upperbound ");
                 strcat(PV[pv_num].score,cur_word);
                 break;
              }
              break;
           }
           if (pv_mode) {
             int pv_len = PV[pv_num].pv_len;
             mv_t m = move_from_string(cur_word,&State->Board);
             if (!move_is_pseudo(m,&State->Board) || !pseudo_is_legal(m,&State->Board)) {
                for (int i=PV[pv_num].pv_len-1; i>=0; i--)
                  move_undo(&State->Board,PV[pv_num].moves[i],&PV[pv_num].undo[i]);
                return 0;
             }
             move_to_string(&State->Board,m,PV[pv_num].moves_str[pv_len],8);
             move_do(&State->Board,m,&PV[pv_num].undo[pv_len]);
             PV[pv_num].moves[pv_len] = m;
             PV[pv_num].pv_len++;
           }
           break;
         case 6:
           if (string_equal(cur_word,"tbhits")) {
               get_cur_word_int(&line);
               break;
           }
           if (string_equal(cur_word,"string"))
               return 0;
           break;
         case 7:
           if (string_equal(cur_word,"multipv")) {
               pv_num = get_cur_word_int(&line) - 1;
               break;
           }
           if (string_equal(cur_word,"cpuload")) {
               get_cur_word_int(&line);
               break;
           }
           break;
         case 8:
           if (string_equal(cur_word,"seldepth")) {
               get_cur_word_int(&line);
               break;
           }
           if (string_equal(cur_word,"currmove")) {
               get_cur_word_str(&line);
               break;
           }
           if (string_equal(cur_word,"hashfull")) {
               get_cur_word_int(&line);
               break;
           }
           break;
         case 14:
           if (string_equal(cur_word,"currmovenumber")) {
               get_cur_word_int(&line);
               break;
           }
      }
  }

  if (pv_exists)
    for (int i=PV[pv_num].pv_len-1; i>=0; i--)
      move_undo(&State->Board,PV[pv_num].moves[i],&PV[pv_num].undo[i]);

  return 0;
}

void TUCIInterface::StartThink(TState * State)
{
   if (Started) {
     Process->Send("stop");
     do {
       Sleep(10);
     } while (!TreatEngineOutput(State));
   }
   AnsiString pos;
   MakeGoString(State,&pos);
   Process->Send(pos);
   for (int i=0; i<max_pv_cnt; i++)
     PV[i].pv_len = 0;
   Process->Send("go infinite");
   Started = true;
}


void TUCIInterface::StartThink(TState * State, int wtime, int btime, int winc, int binc)
{
   if (Started) {
     Process->Send("stop");
     do {
       Sleep(10);
     } while (!TreatEngineOutput(State));
   }
   AnsiString pos;
   MakeGoString(State,&pos);
   Process->Send(pos);
   for (int i=0; i<max_pv_cnt; i++)
     PV[i].pv_len = 0;
   char s[100];
   strcpy(s,"go");
   if (wtime > 0)
     sprintf(s+strlen(s)," wtime %i", wtime);
   if (btime > 0)
     sprintf(s+strlen(s)," btime %i", btime);
   if (winc > 0)
     sprintf(s+strlen(s)," winc %i", winc);
   if (binc> 0)
     sprintf(s+strlen(s)," binc %i", binc);
   Process->Send(s);
   Started = true;
}

void TUCIInterface::MakeGoString(TState * State, AnsiString *pos)
{
   *pos = "position startpos";
   if (State->MoveHistoryLen > 0 || State->LastMove)
     *pos += " moves";
   char move_str[6] = "";
   for (int i=0; i<State->MoveHistoryLen; i++) {
     move_to_string(State->MoveHistory[i],move_str,6);
     *pos += " ";
     *pos += move_str;
   }
   if (State->LastMove) {
     move_to_string(State->LastMove,move_str,6);
     *pos += " ";
     *pos += move_str;
   }
}


