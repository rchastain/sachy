/*
 *  search.c
 *  Alpha-beta vyhledavaci funkce
 */

#include"data.h"

int SilentAlphaBeta(TGame *, int, int, int, int);

extern int IQ, SIQ;

int AlphaBeta(TGame *aGame, int depth, int alpha, int beta)
{
  TGame lGame;
  TMoveList lList;
  int best, p, i = 0, value;
  //char c[5];

  if (depth <= 0)
  {
    if (SIQ < 0)
      return SilentAlphaBeta(aGame, depth, alpha, beta, 0);
    else
      return Evaluate(aGame);
  }
  p = MoveGen(aGame, lList, 0);

  if (p == 0)
    return Evaluate(aGame);

  best = -1 * MATV;
  while (i < p && best < beta)
  {
    lGame = *aGame;
    /*
    SqrToStr(lList[i].from, &c[0]);
    SqrToStr(lList[i].to, &c[2]);
    MakeMoveStr(c, &lGame);
    */
    MakeMove(lList[i], &lGame);
    if (best > alpha)
      alpha = best;
    value = -1 * AlphaBeta(&lGame, depth - 1, -1 * beta, -1 * alpha);
    if (value > best)
      best = value;
    i++;
  }

  return best;
}

int SilentAlphaBeta(TGame *aGame, int depth, int alpha, int beta, int nt)
{
  TGame lGame;
  TMoveList lList;
  int best, lCount, i = 0, value;
  //char c[5];

  if (depth <= SIQ)
  {
    if (depth < 0)
      return EvaluateM(aGame);
    else
      return Evaluate(aGame);
  }
  lCount = SMoveGen(aGame, lList);

  best = -1 * MATV;
  while (i < lCount && best < beta)
  {
    lGame = *aGame;
    /*
    SqrToStr(lList[i].from, &c[0]);
    SqrToStr(lList[i].to, &c[2]);
    MakeMoveStr(c, &lGame);
    */
    MakeMove(lList[i], &lGame);
    if (best > alpha)
      alpha = best;
    value = -1 * SilentAlphaBeta(&lGame, depth - 1, -1 * beta, -1 * alpha, 0);
    if (value > best)
      best = value;
    i++;
  }
  /* vygenerovat take NULL tah */
  /* jinak bude napriklad nucen delat nevyhodne
     braci tahy */
  if (nt == 0)
  {
    lGame = *aGame;
    if (aGame->sidetomove == WHITE)
      lGame.sidetomove = BLACK;
    else
      lGame.sidetomove = WHITE;
    lGame.enpas = -1;  /* no enpassant while NULL move */
    value = -1 * SilentAlphaBeta(&lGame, depth - 1, -1 * beta, -1 * alpha, 1);
    if (value > best)
      best = value;
  }
  else if (nt == 1 && lCount == 0)
    return EvaluateM(aGame);

  return best;
}
