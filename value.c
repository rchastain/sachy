/*
 *  value.c
 *  Ohodnoceni pozice
 */

#include"data.h"

#define abs(x) (x > 0 ? x : -x)
#define sign(x) (x == 0 ? 0 : x / abs(x))

int Evaluate(TGame * aGame)
{
  int lMoveCount;
  TMoveList lList;
  lMoveCount = MoveGen(aGame, lList, 1);
  if (lMoveCount == 0)
  {
    if (InCheck(aGame->sidetomove, aGame)) /* MAT */
      return -1 * MATV;
    else
      return 0; /* PAT */
  }
  else
    return EvaluateM(aGame);
}

int EvaluateM(TGame * aGame)
{
  int i, v = 0, x, y, b2s = 0, c2s = 0;
  v += 2 * (aGame->whiteplayer.rochada[0] - aGame->blackplayer.rochada[0]);
  v += aGame->whiteplayer.rochada[1] - aGame->blackplayer.rochada[1];
  v += aGame->whiteplayer.rochada[2] - aGame->blackplayer.rochada[2];
  v += 2 * (sign(aGame->board[3][3]) + sign(aGame->board[3][4])
            + sign(aGame->board[4][3]) + sign(aGame->board[4][4]));
  v += sign(aGame->board[2][2]) +
       sign(aGame->board[2][3]) +
       sign(aGame->board[2][4]) +
       sign(aGame->board[2][5]) +
       sign(aGame->board[3][2]) +
       sign(aGame->board[3][5]) +
       sign(aGame->board[4][2]) +
       sign(aGame->board[4][5]) +
       sign(aGame->board[5][2]) +
       sign(aGame->board[5][3]) +
       sign(aGame->board[5][4]) +
       sign(aGame->board[5][5]);
  for (i = 1; i < 16; i++)
  {
    switch (aGame->whiteplayer.pieces[i].fig)
    {
    case WPWN:
      v += PWNV;
      if (aGame->whiteplayer.pieces[i].pos.x > 4) v++;
      if (aGame->whiteplayer.pieces[i].pos.x > 5) v++;
      break;
    case WKNT:
      v += KNTV;
      x = aGame->whiteplayer.pieces[i].pos.x;
      y = aGame->whiteplayer.pieces[i].pos.y;
      if (x > 0 && y > 0 && x < 7 && y < 7) v++;
      break;
    case WBSP:
      v += BSPV;
      b2s++;
      if (b2s > 1) v++;
      break;
    case WROK:
      v += ROKV;
      break;
    case WQEN:
      v += QENV;
      break;
    }
    switch (aGame->blackplayer.pieces[i].fig)
    {
    case BPWN:
      v -= PWNV;
      if (aGame->blackplayer.pieces[i].pos.x < 3) v--;
      if (aGame->blackplayer.pieces[i].pos.x < 2) v--;
      break;
    case BKNT:
      v -= KNTV;
      x = aGame->blackplayer.pieces[i].pos.x;
      y = aGame->blackplayer.pieces[i].pos.y;
      if (x > 0 && y > 0 && x < 7 && y < 7) v--;
      break;
    case BBSP:
      v -= BSPV;
      c2s++;
      if (c2s > 1) v--;
      break;
    case BROK:
      v -= ROKV;
      break;
    case BQEN:
      v -= QENV;
      break;
    }
  }
  if (aGame->sidetomove == WHITE)
    return v;
  else
    return -v;
}
