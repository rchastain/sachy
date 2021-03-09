/*
 *  makemove.c
 *  Provadeni tahu a vyber tahu
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <time.h>
#include <sys/time.h>
#include "data.h"

extern int IQ, SIQ;
extern TMove historie[MAXMOVHIS]; /* 1.tah = [1] */

extern char logBuffer[256];

void MakeMove(TMove aMove, TGame *aGame)
{
  int lPieceIdx, lCapturedPieceIdx, lRookIdx;
  TSquare lFrom, lTo, lAux;
  TMove lMove;
  /*
  StrToSqr(&aMoveStr[0], &lFrom);
  StrToSqr(&aMoveStr[2], &lTo);
  lMove.from = lFrom;
  lMove.to = lTo;
  */
  lMove = aMove;
  lFrom = lMove.from;
  lTo = lMove.to;
  
  switch (aGame->sidetomove)
  {
    case WHITE:
      if (!FindPiece(lFrom, aGame->board, &(aGame->whiteplayer), &lPieceIdx))
      {
        fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
        PrintBoard(*aGame);
        //printf("%s", aMoveStr);
        //return;
        exit(0);
      }
      break;
    case BLACK:
      if (!FindPiece(lFrom, aGame->board, &(aGame->blackplayer), &lPieceIdx))
      {
        fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
        PrintBoard(*aGame);
        //printf("%s", aMoveStr);
        //return;
        exit(0);
      }
      break;
  }
  
  switch (aGame->sidetomove)
  {
    case WHITE:
      aGame->whiteplayer.pieces[lPieceIdx].pos = lTo;
      if (aGame->whiteplayer.pieces[lPieceIdx].fig == WKNG)
      {
        aGame->whiteplayer.rochada[0] = 0;
        aGame->whiteplayer.rochada[1] = 0;
      }
      else if ((aGame->whiteplayer.pieces[lPieceIdx].fig == WROK))
      {
        if (lFrom.x == 7)
          aGame->whiteplayer.rochada[0] = 0;
        if (lFrom.x == 0)
          aGame->whiteplayer.rochada[1] = 0;
      }
      break;
    case BLACK:
      aGame->blackplayer.pieces[lPieceIdx].pos = lTo;
      if (aGame->blackplayer.pieces[lPieceIdx].fig == BKNG)
      {
        aGame->blackplayer.rochada[0] = 0;
        aGame->blackplayer.rochada[1] = 0;
      }
      else if ((aGame->blackplayer.pieces[lPieceIdx].fig == BROK))
      {
        if (lFrom.x == 7)
          aGame->blackplayer.rochada[0] = 0;
        if (lFrom.x == 0)
          aGame->blackplayer.rochada[1] = 0;
      }
      break;
  }
  /* pokud bere nejakou figuru, tak vyres protihrace */
  if (aGame->board[lTo.x][lTo.y] != EMPT)
  {
    /* pokud se tahne na rohova pole, zakaz prislusne rochady */
    if (lTo.x == 0 && lTo.y == 0)
      aGame->whiteplayer.rochada[1] = 0;
    else if (lTo.x == 7 && lTo.y == 0)
      aGame->whiteplayer.rochada[0] = 0;
    else if (lTo.x == 0 && lTo.y == 7)
      aGame->blackplayer.rochada[1] = 0;
    else if (lTo.x == 7 && lTo.y == 7)
      aGame->blackplayer.rochada[0] = 0;
    switch (aGame->sidetomove)
    {
      case BLACK:
        if (!FindPiece(lTo, aGame->board, &(aGame->whiteplayer), &lCapturedPieceIdx))
        {
          fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
          //return;
          exit(0);
        }
        break;
      case WHITE:
        if (!FindPiece(lTo, aGame->board, &(aGame->blackplayer), &lCapturedPieceIdx))
        {
          fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
          //return;
          exit(0);
        }
        break;
    }
    /* ted odebrat figuru z protihracovych figurek */
    switch (aGame->sidetomove)
    {
      case BLACK:
        aGame->whiteplayer.pieces[lCapturedPieceIdx].fig = EMPT;
        RemovePiece(&(aGame->whiteplayer));
        break;
      case WHITE:
        aGame->blackplayer.pieces[lCapturedPieceIdx].fig = EMPT;
        RemovePiece(&(aGame->blackplayer));
        break;
    }
  }
  else if (
    (aGame->enpas == lMove.to.x)
    && ((aGame->board[lFrom.x][lFrom.y] == BPWN) || (aGame->board[lFrom.x][lFrom.y] == WPWN))
    && (lMove.to.x != lMove.from.x)
  )
  {
    switch (aGame->sidetomove)
    {
      case WHITE:
        lAux.y = 4;
        break;
      case BLACK:
        lAux.y = 3;
        break;
    }
    lAux.x = aGame->enpas;
    switch (aGame->sidetomove)
    {
      case BLACK:
        if (!FindPiece(lAux, aGame->board, &(aGame->whiteplayer), &lCapturedPieceIdx))
        {
          fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
          //return;
          exit(0);
        }
        break;
      case WHITE:
        if (!FindPiece(lAux, aGame->board, &(aGame->blackplayer), &lCapturedPieceIdx))
        {
          fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
          //return;
          exit(0);
        }
        break;
    }
    /* ted odebrat pesce z protihracovych figurek */
    switch (aGame->sidetomove)
    {
      case BLACK:
        aGame->whiteplayer.pieces[lCapturedPieceIdx].fig = EMPT;
        RemovePiece(&(aGame->whiteplayer));
        break;
      case WHITE:
        aGame->blackplayer.pieces[lCapturedPieceIdx].fig = EMPT;
        RemovePiece(&(aGame->blackplayer));
        break;
    }
    /* jeste ji zrusit na sachovnici */
    aGame->board[lAux.x][lAux.y] = EMPT;
  }
  
  /* hejbni prislusnou figurou na sachovnici */
  aGame->board[lTo.x][lTo.y] = aGame->board[lFrom.x][lFrom.y];
  aGame->board[lFrom.x][lFrom.y] = EMPT;
  
  switch (aGame->sidetomove) /* nastavi promennou enpas */
  {
    case WHITE:
      if ((aGame->whiteplayer.pieces[lPieceIdx].fig == WPWN) && (lTo.y - lFrom.y == 2))
        aGame->enpas = lTo.x;
      else
        aGame->enpas = -1;
      break;
    case BLACK:
      if ((aGame->blackplayer.pieces[lPieceIdx].fig == BPWN) && (lTo.y - lFrom.y == -2))
        aGame->enpas = lTo.x;
      else
        aGame->enpas = -1;
      break;
  }
  
  /* pokud je rochada, hejbni jeste vezema */
  if (((aGame->sidetomove == WHITE) && (aGame->whiteplayer.pieces[lPieceIdx].fig == WKNG)) ||
      ((aGame->sidetomove == BLACK) && (aGame->blackplayer.pieces[lPieceIdx].fig == BKNG)))
  {
    //if (strcmp(aMoveStr, "e1g1") == 0)
    if (aMove.from.x == 4 && aMove.from.y == 0 && aMove.to.x == 6)
    {
      aGame->whiteplayer.rochada[2] = 1;
      lAux.x = 7;
      lAux.y = 0;
      if (!FindPiece(lAux, aGame->board, &(aGame->whiteplayer), &lRookIdx))
      {
        fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
        //return;
        exit(0);
      }
      else
      {
        lAux.x = 5;
        aGame->whiteplayer.pieces[lRookIdx].pos = lAux;
        aGame->board[7][0] = EMPT;
        aGame->board[5][0] = WROK;
      }
    }
    //else if (strcmp(aMoveStr, "e1c1") == 0)
    else if (aMove.from.x == 4 && aMove.from.y == 0 && aMove.to.x == 2)
    {
      aGame->whiteplayer.rochada[2] = 1;
      lAux.x = 0;
      lAux.y = 0;
      if (!FindPiece(lAux, aGame->board, &(aGame->whiteplayer), &lRookIdx))
      {
        fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
        //return;
        exit(0);
      }
      else
      {
        lAux.x = 3;
        aGame->whiteplayer.pieces[lRookIdx].pos = lAux;
        aGame->board[0][0] = EMPT;
        aGame->board[3][0] = WROK;
      }
    }
    //else if (strcmp(aMoveStr, "e8g8") == 0)
    else if (aMove.from.x == 4 && aMove.from.y == 7 && aMove.to.x == 6)
    {
      aGame->blackplayer.rochada[2] = 1;
      lAux.x = 7;
      lAux.y = 7;
      if (!FindPiece(lAux, aGame->board, &(aGame->blackplayer), &lRookIdx))
      {
        fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
        //return;
        exit(0);
      }
      else
      {
        lAux.x = 5;
        aGame->blackplayer.pieces[lRookIdx].pos = lAux;
        aGame->board[7][7] = EMPT;
        aGame->board[5][7] = BROK;
      }
    }
    //else if (strcmp(aMoveStr, "e8c8") == 0)
    else if (aMove.from.x == 4 && aMove.from.y == 7 && aMove.to.x == 2)
    {
      aGame->blackplayer.rochada[2] = 1;
      lAux.x = 0;
      lAux.y = 7;
      if (!FindPiece(lAux, aGame->board, &(aGame->blackplayer), &lRookIdx))
      {
        fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
        //return;
        exit(0);
      }
      else
      {
        lAux.x = 3;
        aGame->blackplayer.pieces[lRookIdx].pos = lAux;
        aGame->board[0][7] = EMPT;
        aGame->board[3][7] = BROK;
      }
    }
  }
  
  /* pokud pesec dorazi na konec, zmen ho na damu */
  if (
    ((aGame->sidetomove == WHITE) && (aGame->whiteplayer.pieces[lPieceIdx].fig == WPWN) && (lTo.y == 7)) ||
    ((aGame->sidetomove == BLACK) && (aGame->blackplayer.pieces[lPieceIdx].fig == BPWN) && (lTo.y == 0))
  ) /* budeme menit */
  {
    /*
    switch (aGame->sidetomove)
    {
      case WHITE:
        aGame->whiteplayer.pieces[lPieceIdx].fig = WQEN;
        aGame->board[lTo.x][lTo.y] = WQEN;
        break;
      case BLACK:
        aGame->blackplayer.pieces[lPieceIdx].fig = BQEN;
        aGame->board[lTo.x][lTo.y] = BQEN;
        break;
    }
    */
    if (lMove.promotion == EMPT)
    {
      fprintf(stderr, "# Internal error (%s, line %d)\n", __FILE__, __LINE__);
      exit(0);
      /*
      switch (aGame->sidetomove)
      {
        case WHITE:
          aGame->whiteplayer.pieces[lPieceIdx].fig = WQEN;
          aGame->board[lTo.x][lTo.y] = WQEN;
          break;
        case BLACK:
          aGame->blackplayer.pieces[lPieceIdx].fig = BQEN;
          aGame->board[lTo.x][lTo.y] = BQEN;
          break;
      }
      */
    }
    else
    {
      switch (aGame->sidetomove)
      {
        case WHITE:
          aGame->whiteplayer.pieces[lPieceIdx].fig = lMove.promotion;
          aGame->board[lTo.x][lTo.y] = lMove.promotion;
          break;
        case BLACK:
          aGame->blackplayer.pieces[lPieceIdx].fig = -1 * lMove.promotion;
          aGame->board[lTo.x][lTo.y] = -1 * lMove.promotion;
          break;
      }
    } 
  }
  /* update historie tahu */
  (aGame->fHalfMovesCount)++;
  historie[aGame->fHalfMovesCount] = lMove;
  /* prohodit hrace na tahu */
  if (aGame->sidetomove == WHITE)
    aGame->sidetomove = BLACK;
  else
    aGame->sidetomove = WHITE;
}

void MakeMoveStr(char *aMoveStr, TGame *aGame)
{
  TMove lMove;
  TSquare lFrom, lTo;
  char lPromotion = EMPT;
  StrToSqr(&aMoveStr[0], &lFrom);
  StrToSqr(&aMoveStr[2], &lTo);
  if (strlen(aMoveStr) > 4)
  {
    //printf("# Five characters detected (%s)\n", aMoveStr);
    lPromotion = StrToPrm(&aMoveStr[4]);
    //printf("# Promotion value: %d\n", lPromotion);
  }
  lMove.from = lFrom;
  lMove.to = lTo;
  lMove.promotion = lPromotion;
  MakeMove(lMove, aGame);
}

/*
int BestMove(TGame *aGame, char aMoveStr[6], int aOutput)
{
  static TGame lGame;
  TMoveList lList;
  int lCount, lBest, lBestVal, lVal, lValArr[MAXMOVGEN], lDepth = 1, i;
  time_t t1, t2;
  char lMoveStr[6];
  
  time(&t1);
  lCount = MoveGen(aGame, lList, 0);
  if (lCount == 0)
    return -1;
  
  if (lCount == 1)
  {
    lBest = 0;
    t2 = t1;
  }
  else
    while (1)
    {
      lBest = -1;
      lBestVal = -1 * MATV - 1;
      for (i = 0; i < lCount; i++)
      {
        lGame = *aGame;
        SqrToStr(lList[i].from, &aMoveStr[0]);
        SqrToStr(lList[i].to, &aMoveStr[2]);
        MakeMoveStr(aMoveStr, &lGame);
        lVal = -1 * AlphaBeta(&lGame, lDepth - 1, -1 * MATV, -1 * lBestVal);
        lValArr[i] = lVal;
        if (lVal > lBestVal)
        {
          lBest = i;
          lBestVal = lVal;
          strcpy(lMoveStr, aMoveStr);
        }
      }
      time(&t2);
      if (aOutput) printf("%d %d %ld 0 %s\n", lDepth, lBestVal, t2 - t1, lMoveStr);
      
      if ((((t2 - t1) * 5) * (aGame->tmoves - (((aGame->fHalfMovesCount + 1)) / 2) % aGame->tmoves)) > aGame->tleft)
        break;
      else
      {
        Sort(lList, lValArr, lCount);
        lDepth++;
      }
    }
  
  SqrToStr(lList[lBest].from, &aMoveStr[0]);
  SqrToStr(lList[lBest].to, &aMoveStr[2]);
  if (lList[lBest].promotion != EMPT)
    PrmToStr(lList[lBest].promotion, &aMoveStr[4]);
  
  return (t2 - t1 + 1);
}
*/

int BestMove2(TGame *aGame, char aMoveStr[/*5*/6], int aOutput)
{
  static TGame lGame;
  TMoveList lList;
  int lCount, lBest, lBestVal, lVal, lValArr[MAXMOVGEN], lDepth = 1, i, lTotal = 0;
  //time_t t1, t2;
  long t1, t2;
  struct timeval timecheck;
  char lMoveStr[6];
  double lTimeAvailable, lTimeElapsed;
  
  //time(&t1);
  gettimeofday(&timecheck, NULL);
  t1 = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
  
  lCount = MoveGen(aGame, lList, 0);
  lTotal += lCount;
  if (lCount == 0)
    return -1;
  
  if (lCount == 1)
  {
    lBest = 0;
    t2 = t1;
  }
  else
    while (1)
    {
      lBest = -1;
      lBestVal = -1 * MATV - 1; /* pokud najde mat ... */
      for (i = 0; i < lCount; i++)
      {
        lGame = *aGame;
        /*
        SqrToStr(lList[i].from, &aMoveStr[0]);
        SqrToStr(lList[i].to, &aMoveStr[2]);
        MakeMoveStr(aMoveStr, &lGame);
        */
        MakeMove(lList[i], &lGame);
        lVal = -1 * AlphaBeta(&lGame, lDepth - 1, -1 * MATV, -1 * lBestVal);
        lValArr[i] = lVal;
        if (lVal > lBestVal)
        {
          lBest = i;
          lBestVal = lVal;
          //strcpy(lMoveStr, aMoveStr);
          if (aOutput) {
            SqrToStr(lList[i].from, &lMoveStr[0]);
            SqrToStr(lList[i].to, &lMoveStr[2]);
            if (lList[i].promotion != EMPT)
              PrmToStr(lList[i].promotion, &lMoveStr[4]);
          }
        }
      }
      //time(&t2);
      gettimeofday(&timecheck, NULL);
      t2 = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
      
      if (aOutput) printf("%d %d %ld %d %s\n", lDepth, lBestVal, t2 - t1, lTotal, lMoveStr);
      /* TODO make this better */
      lTimeAvailable = (double)aGame->tseconds / aGame->tmoves;
      //lTimeElapsed = difftime(t2, t1);
      lTimeElapsed = (double)(t2 - t1) / 1000;
      sprintf(logBuffer, "time available: %f; time elapsed: %f", lTimeAvailable, lTimeElapsed);
      ToLog(logBuffer, 1);
      
      if (
        (lBestVal == MATV) ||
        //((((t2 - t1) * 5) * (aGame->tmoves - ((aGame->fHalfMovesCount + 1) / 2) % aGame->tmoves)) > aGame->tleft)
        (lTimeElapsed > lTimeAvailable / 5)
      )
        break;
      else
      {
        Sort(lList, lValArr, lCount);
        lDepth++;
      }
    }
  
  SqrToStr(lList[lBest].from, &aMoveStr[0]);
  SqrToStr(lList[lBest].to, &aMoveStr[2]);
  if (lList[lBest].promotion != EMPT)
    PrmToStr(lList[lBest].promotion, &aMoveStr[4]);
  
  return (t2 - t1 + 1);
}
