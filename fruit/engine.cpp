//---------------------------------------------------------------------------


#pragma hdrstop

#include "engine.h"
#include "parse.h"
#include "main.h"

#include "attack.h"
#include "hash.h"
#include "move_do.h"
#include "pawn.h"
#include "piece.h"
#include "protocol.h"
#include "random.h"
#include "square.h"
#include "my_util.h"
#include "value.h"
#include "vector.h"
#include "fen.h"
#include "move.h"
#include "move_legal.h"
#include "move_gen.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void TEngine::AddMove(mv_t move)
{
   undo_t undo;
   move_do(&Board,move,&undo);
   char move_str[6];
   strcat(move_history," ");
   move_to_string(move,move_str,6);
   strcat(move_history,move_str);
   move_history_len++;
}

/*
void TEngine::AddMove(board_t * board, mv_t move)
{
   undo_t undo;
   move_do(&Board,move,&undo);
   char move_str[6];
   strcat(move_history," ");
   move_to_string(board,move,move_str,6);
   strcat(move_history,move_str);
   move_history_len++;
}
*/

bool TEngine::Tick2(board_t * board, TFindPos * fp, bool full)
{
   mv_t pv[3];
   if (SearchPos(board,fp,pv, full)) {
      int pv_len = 0;
      for (int i=0; i<3; i++) {
        if (pv[i]==0)
          break;
        else
          pv_len++;
      }
      switch (pv_len) {
         case 0:
            if (LastMove) {
               LastMove = 0;
               StartNewThink();
            }
            if (!GameStarted) {
              StartNewGame();
              GameStarted = true;
            }
            return true;
         case 1:
            StateNewGame = false;
            if (LastMove != pv[0]) {
               LastMove = pv[0];
               StartNewThink();
            }
            return true;
         case 2:
            StateNewGame = false;
            AddMove(pv[0]);
            LastMove = pv[1];
            StartNewThink();
            return true;
      }
   }
   return false;
}

bool TEngine::Tick3(board_t * board, TFindPos * fp, bool full)
{
   mv_t pv[3];
   if (SearchPos(board,fp,pv, full)) {
      int pv_len = 0;
      for (int i=0; i<3; i++) {
        if (pv[i]==0)
          break;
        else
          pv_len++;
      }
      switch (pv_len) {
         case 0:
            if (!StateNewGame)
              StartNewGame();
            StartNewThink();
            return true;
         case 1:
            if (!StateNewGame)
              StartNewGame();
            LastMove = pv[0];
            StartNewThink();
            return true;
      }
   }
   return false;
}

void TEngine::Tick(TFindPos * fp)
{
  if (Reversed)
    fp->Reverse();

  if (Tick2(&Board,fp,true)) {
    StateNewGame = false;
    return;
  }

  if (!StateNewGame) {
    if (Tick3(&StartBoard,fp,false)) {
      StateNewGame = false;
      return;
    }

    fp->Reverse();
    if (Tick3(&StartBoard,fp,false)) {
      StateNewGame = false;
      Reversed = !Reversed;
      return;
    }
  }
}

bool TEngine::Eval(board_t * Board, TFindPos * fp)
{
  const sq_t * ptr;
  int from;
  TFindPos fp_board(0,0);
  for (int colour = 0; colour < ColourNb; colour++) {
    for (ptr = &Board->piece[colour][0]; (from=*ptr) != SquareNone; ptr++)
      fp_board.SetBit(colour,SQUARE_TO_64(from));
    for (ptr = &Board->pawn[colour][0]; (from=*ptr) != SquareNone; ptr++) {
      fp_board.SetBit(colour,SQUARE_TO_64(from));
    }
  }
  return !memcmp(fp,&fp_board,sizeof (TFindPos));
}


bool TEngine::SearchPos(board_t * Board, TFindPos * fp, mv_t pv[], bool full)
{
   //------------------------
   if (Eval(Board,fp)) {
     pv[0] = 0;
     return true;
   }
   //-----------------------
   if (LastMove) {
      undo_t undo;
      move_do(Board,LastMove,&undo);
      bool eval = Eval(Board,fp);
      if (Eval(Board,fp)) {
         move_undo(Board,LastMove, &undo);
         pv[0] = LastMove;
         pv[1] = 0;
         return true;
      }
      list_t list;
      gen_legal_moves(&list,Board);
      for (int i=0; i<LIST_SIZE(&list); i++) {
         undo_t undo2;
         mv_t move2 = LIST_MOVE(&list,i);
         move_do(Board,move2,&undo2);
         bool eval = Eval(Board,fp);
         move_undo(Board,move2, &undo2);
         if (eval) {
             move_undo(Board,LastMove, &undo);
             pv[0] = LastMove;
             pv[1] = move2;
             pv[2] = 0;
             return true;;
         }
      }
      move_undo(Board,LastMove,&undo);
   }
   //----------------------------------------------------
   list_t list;
   gen_legal_moves(&list,Board);
   //----------------------------------------------------
   for (int i=0; i<LIST_SIZE(&list); i++) {
      undo_t undo;
      mv_t move = LIST_MOVE(&list,i);
      move_do(Board,move,&undo);
      bool eval = Eval(Board,fp);
      move_undo(Board,move,&undo);
      if (eval) {
         pv[0] = move;
         pv[1] = 0;
         return true;
      }
   }
   //------------------------------------------------------
   if (full) {
     for (int i=0; i<LIST_SIZE(&list); i++) {
        undo_t undo;
        mv_t move = LIST_MOVE(&list,i);
        move_do(Board,move,&undo);
        list_t list2;
        gen_legal_moves(&list2,Board);
        for (int j=0; j<LIST_SIZE(&list2); j++) {
           undo_t undo2;
           mv_t move2 = LIST_MOVE(&list2,j);
           move_do(Board,move2,&undo2);
           /*
           char s_move[6], s_move2[6];
           move_to_string(move,s_move,6);
           move_to_string(move2,s_move2,6);
           */
           bool eval = Eval(Board,fp);
           move_undo(Board,move2,&undo2);
           if (eval) {
              pv[0] = move;
              pv[1] = move2;
              pv[2] = 0;
              move_undo(Board,move,&undo);
              return true;
          }
        }
        move_undo(Board,move,&undo);
     }
   }
   //-------------------------------------------------------
   return false;
}

void TEngine::StartNewThink()
{
   Process->Send("stop");
   Sleep(10);
   AnsiString pos = "position startpos";
   if (move_history_len > 0 || LastMove) {
     pos += " moves";
     pos += move_history;
     if (LastMove) {
       char move_str[6] = "";
       move_to_string(LastMove,move_str,6);
       pos += " ";
       pos += move_str;
     }
   }
   Process->Send(pos);
   for (int i=0; i<100; i++)
     PV[i].pv_len = 0;
   Process->Send("go infinite");
}


void TEngine::StartNewGame()
{
   StateNewGame = true;
   LastMove = 0;
   move_history_len = 0;
   move_history[0] = 0;
   board_from_fen(&Board,StartFen);
   Process->Send("stop");
   Process->Send("ucinewgame");
   Process->Send("position startpos");
   Process->Send("go infinite");
}

void TEngine::ParseLine(char *line)
{
  char *cur_word = get_cur_word_str(&line);
  if (!string_equal(cur_word,"info"))
    return;
  bool pv_mode = false;
  int pv_num = 0;
  undo_t undo;
  if (LastMove)
    move_do(&Board,LastMove,&undo);

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
             mv_t m = move_from_string(cur_word,&Board);
             PV[pv_num].moves[pv_len] = m;
             move_do(&Board,m,&PV[pv_num].undo[pv_len]);
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
                 if (Board.turn == Black)
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
             mv_t m = move_from_string(cur_word,&Board);
             PV[pv_num].moves[pv_len] = m;
             move_do(&Board,m,&PV[pv_num].undo[pv_len]);
             PV[pv_num].pv_len++;
           }
           break;
         case 6:
           if (string_equal(cur_word,"tbhits")) {
               get_cur_word_int(&line);
               break;
           }
           if (string_equal(cur_word,"string"))
               return;
           break;
         case 7:
           if (string_equal(cur_word,"multipv")) {
               pv_num = get_cur_word_int(&line) - 1;
               if (pv_num > pv_cur_cnt)
                 pv_cur_cnt = pv_num;
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
      move_undo(&Board,PV[pv_num].moves[i],&PV[pv_num].undo[i]);

  if (LastMove)
    move_undo(&Board,LastMove,&undo);
}

void TEngine::Output()
{
  board_t backup_board;
  board_copy(&backup_board,&Board);
  bool flag = false;

  char move_str[6] = "";
  if (LastMove)
     move_to_string(&Board,LastMove,move_str,6);

  MainForm->edLastMove->Text = move_str;
  char buf[10000];
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
        if (flag && Board.square[75] == 66) {
           int k; k++;
        }
        ParseLine(cur_line);
        if (flag && Board.square[75] == 66) {
           int k; k++;
        }
      }
      else
        break;
    }
    MainForm->Memo->Lines->Clear();
    for (int i=0; i<MainForm->MultiPV; i++) {
       AnsiString s;
       s += PV[i].score;
       s += " ";
       undo_t last_move_undo;
       if (LastMove)
         move_do(&Board,LastMove,&last_move_undo);
       int cnt = 0;
       ASSERT(PV[i].pv_len>=0);
       for (int j=0; j<PV[i].pv_len; j++) {
          char move_s[6];
          //move_to_string(&Board,PV[i].moves[j],move_s,6);
          move_to_string(PV[i].moves[j],move_s,8);
          s += move_s;
          s += " ";
          /*
          move_do(&Board,PV[i].moves[j],&PV[i].undo[j]);
          cnt++;
          */
       }
       /*
       for (int j=PV[i].pv_len-1; j>=0; j--) {
         move_undo(&Board,PV[i].moves[j],&PV[i].undo[j]);
         cnt--;
       }
       if (cnt) {
          int k; k++;
       }
       */
       if (LastMove)
         move_undo(&Board,LastMove,&last_move_undo);
       MainForm->Memo->Lines->Append(s);
    }
  }
}

TEngine::TEngine(AnsiString FileName)
{
   util_init();
   my_random_init();
   square_init();
   piece_init();
   pawn_init_bit();
   value_init();
   vector_init();
   attack_init();
   move_do_init();
   random_init();
   hash_init();

   Reversed = true;
   board_from_fen(&StartBoard,StartFen);
   board_from_fen(&Board,StartFen);
   GameStarted = false;

   StateNewGame = false;
   LastCommitMove[0] = 0;
   Process = new TProcess(FileName);
   Process->Send("uci");
   char buf[10000];
   do {
      Sleep(10);
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
                   Process->Send("setoption name MultiPV value " + IntToStr(MainForm->MultiPV));
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

TEngine::~TEngine()
{
   delete Process;
}

