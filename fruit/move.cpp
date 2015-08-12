
// move.cpp

// includes

#include <ctype.h>
#include <string.h>

#include "attack.h"
#include "colour.h"
#include "list.h"
#include "move.h"
#include "move_do.h"
#include "move_gen.h"
#include "piece.h"
#include "square.h"
#include "my_util.h"

// constants

static const int PromotePiece[4] = { Knight64, Bishop64, Rook64, Queen64 };
static bool find_attack_piece(board_t * board,  int colour, int to, int exclude_sq, char *s);


// functions

// move_is_ok()

bool move_is_ok(int move) {

   if (move < 0 || move >= 65536) return false;

   if (move == MoveNone) return false;
   if (move == MoveNull) return false;

   return true;
}

// move_promote()

int move_promote(int move) {

   int code, piece;

   ASSERT(move_is_ok(move));

   ASSERT(MOVE_IS_PROMOTE(move));

   code = (move >> 12) & 3;
   piece = PromotePiece[code];

   if (SQUARE_RANK(MOVE_TO(move)) == Rank8) {
      piece |= WhiteFlag;
   } else {
      ASSERT(SQUARE_RANK(MOVE_TO(move))==Rank1);
      piece |= BlackFlag;
   }

   ASSERT(piece_is_ok(piece));

   return piece;
}

// move_order()

int move_order(int move) {

   ASSERT(move_is_ok(move));

   return ((move & 07777) << 2) | ((move >> 12) & 3);
}

// move_is_capture()

bool move_is_capture(int move, const board_t * board) {

   ASSERT(move_is_ok(move));
   ASSERT(board!=NULL);

   return MOVE_IS_EN_PASSANT(move) || board->square[MOVE_TO(move)] != Empty;
}

// move_is_under_promote()

bool move_is_under_promote(int move) {

   ASSERT(move_is_ok(move));

   return MOVE_IS_PROMOTE(move) && (move & MoveAllFlags) != MovePromoteQueen;
}

// move_is_tactical()

bool move_is_tactical(int move, const board_t * board) {

   ASSERT(move_is_ok(move));
   ASSERT(board!=NULL);

   return (move & (1 << 15)) != 0 || board->square[MOVE_TO(move)] != Empty; // HACK
}

// move_capture()

int move_capture(int move, const board_t * board) {

   ASSERT(move_is_ok(move));
   ASSERT(board!=NULL);

   if (MOVE_IS_EN_PASSANT(move)) {
      return PAWN_OPP(board->square[MOVE_FROM(move)]);
   }

   return board->square[MOVE_TO(move)];
}

// move_to_string()

bool move_to_string(int move, char string[], int size) {

   ASSERT(move==MoveNull||move_is_ok(move));
   ASSERT(string!=NULL);
   ASSERT(size>=6);

   if (size < 6) return false;

   // null move

   if (move == MoveNull) {
      strcpy(string,NullMoveString);
      return true;
   }

   // normal moves

   square_to_string(MOVE_FROM(move),&string[0],3);
   square_to_string(MOVE_TO(move),&string[2],3);
   ASSERT(strlen(string)==4);

   // promotes

   if (MOVE_IS_PROMOTE(move)) {
      string[4] = tolower(piece_to_char(move_promote(move)));
      string[5] = '\0';
   }

   return true;
}

static bool find_attack_piece(board_t * board, int colour, int to, int exclude_sq, char *s)
{
   const sq_t * ptr;
   int from;

   int piece1 = board->square[exclude_sq];
   int piece2;

   for (ptr = &board->piece[colour][0]; (from=*ptr) != SquareNone; ptr++) {
     int piece2 = board->square[from];
     if (piece1 == piece2 && from != exclude_sq && PIECE_ATTACK(board,piece2,from,to)) {
       int file1 = SQUARE_FILE(exclude_sq);
       int file2 = SQUARE_FILE(from);
       if (file1 == file2)
          *s = '1' + SQUARE_RANK(exclude_sq) - 4;
       else
          *s = 'a' + file1 - 4;
       return true;
     }
   }

   return false;
}

bool move_to_string(board_t * board, int move, char string[], int size) {

   ASSERT(move==MoveNull||move_is_ok(move));
   ASSERT(string!=NULL);
   ASSERT(size>=6);

   if (size < 6) return false;

   // null move

   if (move == MoveNull) {
      strcpy(string,NullMoveString);
      return true;
   }

   int from = MOVE_FROM(move);
   int to = MOVE_TO(move);
   char sq[3];
   string[0] = 0;
   char s_from[2];
   s_from[1] = 0;

  switch (move & MoveFlags) {
    case MoveNormal:
      switch (board->square[from]) {
        case WhitePawn256:
          if (board->square[to] != Empty) {
            string[0] = 'a' + (SQUARE_FILE(from)-FileA);
            string[1] = 0;
          }
          break;
        case WhiteRook256:
          strcat(string,"R");
          if (find_attack_piece(board,White,to,from,&s_from[0]))
            strcat(string,s_from);
          break;
        case WhiteKnight256:
          strcat(string,"N");
          if (find_attack_piece(board,White,to,from,&s_from[0]))
            strcat(string,s_from);
          break;
        case WhiteBishop256:
          strcat(string,"B");
          if (find_attack_piece(board,White,to,from,&s_from[0]))
            strcat(string,s_from);
          break;
        case WhiteQueen256:
          strcat(string,"Q");
          if (find_attack_piece(board,White,to,from,&s_from[0]))
            strcat(string,s_from);
          break;
        case WhiteKing256:
          strcat(string,"K");
          break;
        case BlackPawn256:
          if (board->square[to] != Empty) {
            string[0] = 'a' + (SQUARE_FILE(from)-FileA);
            string[1] = 0;
          }
          break;
        case BlackRook256:
          strcat(string,"R");
          if (find_attack_piece(board,Black,to,from,&s_from[0]))
            strcat(string,s_from);
          break;
        case BlackKnight256:
          strcat(string,"N");
          if (find_attack_piece(board,Black,to,from,&s_from[0]))
            strcat(string,s_from);
          break;
        case BlackBishop256:
          strcat(string,"B");
          if (find_attack_piece(board,Black,to,from,&s_from[0]))
            strcat(string,s_from);
          break;
        case BlackQueen256:
          strcat(string,"Q");
          if (find_attack_piece(board,Black,to,from,&s_from[0]))
            strcat(string,s_from);
          break;
        case BlackKing256:
          strcat(string,"K");
          break;
      }
      if (board->square[to] != Empty)
        strcat(string,"x");
      square_to_string(to,sq,3);
      strcat(string,sq);
      break;
    case MoveCastle:
      if (SQUARE_FILE(to) == FileC)
        strcpy(string,"0-0-0");
      if (SQUARE_FILE(to) == FileG)
        strcpy(string,"0-0");
      break;
    case MovePromote:
      if (board->square[to] != Empty) {
        string[0] = 'a' + (SQUARE_FILE(from)-FileA);
        string[1] = 'x';
        string[2] = 0;
      }
      char sq[3];
      square_to_string(to,sq,3);
      strcat(string,sq);
      switch (move & MoveAllFlags) {
        case MovePromoteQueen:
          strcat(string,"Q");
          break;
        case MovePromoteRook:
          strcat(string,"R");
          break;
        case MovePromoteBishop:
          strcat(string,"B");
          break;
        case MovePromoteKnight:
          strcat(string,"N");
          break;
      }
      break;
    case MoveEnPassant:
      string[0] = 'a' + (SQUARE_FILE(from)-FileA);
      string[1] = 'x';
      square_to_string(to,&string[2],3);
      break;
   }
   return true;
}

// move_from_string()

int move_from_string(const char string[], const board_t * board) {

   char tmp_string[3];
   int from, to;
   int move;
   int piece, delta;

   ASSERT(string!=NULL);
   ASSERT(board!=NULL);

   // from

   tmp_string[0] = string[0];
   tmp_string[1] = string[1];
   tmp_string[2] = '\0';

   from = square_from_string(tmp_string);
   if (from == SquareNone) return MoveNone;

   // to

   tmp_string[0] = string[2];
   tmp_string[1] = string[3];
   tmp_string[2] = '\0';

   to = square_from_string(tmp_string);
   if (to == SquareNone) return MoveNone;

   move = MOVE_MAKE(from,to);

   // promote

   switch (string[4]) {
   case '\0': // not a promotion
      break;
   case 'n':
      move |= MovePromoteKnight;
      break;
   case 'b':
      move |= MovePromoteBishop;
      break;
   case 'r':
      move |= MovePromoteRook;
      break;
   case 'q':
      move |= MovePromoteQueen;
      break;
   default:
      return MoveNone;
   }

   // flags

   piece = board->square[from];

   if (PIECE_IS_PAWN(piece)) {
      if (to == board->ep_square) move |= MoveEnPassant;
   } else if (PIECE_IS_KING(piece)) {
      delta = to - from;
      if (delta == +2 || delta == -2) move |= MoveCastle;
   }

   return move;
}


// end of move.cpp

