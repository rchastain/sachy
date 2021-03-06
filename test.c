/*
 *  test.c
 *  Provadi rychlostni testy
 */

#define TESTTIME 5

#include <stdio.h>
#include <time.h>
#include "data.h"

int Benchmark(TGame * aGame)
{
  TGame lGame;
  TMoveList lMoves;
  char lMoveStr[5];
  int lMoveCount, i;
  long lTotal = 0;
  time_t t1, t2;
  time(&t1);
  time(&t2);
  while ((t2 - t1) <= TESTTIME)
  {
    lMoveCount = MoveGen(aGame, lMoves, 0);
    for (i = 0; i < lMoveCount; i++)
    {
      lGame = *aGame;
      SqrToStr(lMoves[i].from, &lMoveStr[0]);
      SqrToStr(lMoves[i].to, &lMoveStr[2]);
      MakeMoveStr(lMoveStr, &lGame);
      Evaluate(&lGame);
    }
    lTotal += lMoveCount;
    time(&t2);
  }
  if (t2 - t1 > 0)
    return (lTotal / (t2 - t1));
  else
    return 0;
}

int Benchmark2(TGame * aGame)
{
  TGame lGame;
  TMoveList lMoves;
  //char lMoveStr[5];
  int lMoveCount, i;
  long lTotal = 0;
  time_t t1, t2;
  time(&t1);
  time(&t2);
  while ((t2 - t1) <= TESTTIME)
  {
    lMoveCount = MoveGen(aGame, lMoves, 0);
    for (i = 0; i < lMoveCount; i++)
    {
      lGame = *aGame;
      MakeMove(lMoves[i], &lGame);
      Evaluate(&lGame);
    }
    lTotal += lMoveCount;
    time(&t2);
  }
  if (t2 - t1 > 0)
    return (lTotal / (t2 - t1));
  else
    return 0;
}
