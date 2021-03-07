/*
 *  movegen.c
 *  Generovani a provadeni tahu
 */

#include <stdio.h>
#include "data.h"

/* lokalni funkce */

void ROKMoveGen(TGame *, TMoveList, int *, int, int);
void BSPMoveGen(TGame *, TMoveList, int *, int, int);

int MoveGen(TGame *aGame, TMoveList aList, int aOne)
{
  TPlayer *lPlayer;
  TPieceData *lPiece;
  TSquare to;
  TMove lMove;
  char lMoveStr[/*5*/6];
  int lCount = 0, i = 1;
  
  if (aGame->sidetomove == WHITE)
  {
    ROKMoveGen(aGame, aList, &lCount, WQEN, aOne);
    if (aOne && lCount)
      return 1;
    BSPMoveGen(aGame, aList, &lCount, WQEN, aOne);
    if (aOne && lCount)
      return 1;
    ROKMoveGen(aGame, aList, &lCount, WROK, aOne);
    if (aOne && lCount)
      return 1;
    BSPMoveGen(aGame, aList, &lCount, WBSP, aOne);
    if (aOne && lCount)
      return 1;
    lPlayer = &(aGame->whiteplayer);
  }
  else
  {
    ROKMoveGen(aGame, aList, &lCount, BQEN, aOne);
    if (aOne && lCount)
      return 1;
    BSPMoveGen(aGame, aList, &lCount, BQEN, aOne);
    if (aOne && lCount)
      return 1;
    ROKMoveGen(aGame, aList, &lCount, BROK, aOne);
    if (aOne && lCount)
      return 1;
    BSPMoveGen(aGame, aList, &lCount, BBSP, aOne);
    if (aOne && lCount)
      return 1;
    lPlayer = &(aGame->blackplayer);
  }
  
  while (i < 16 && (lPlayer->pieces[i].fig != EMPT))
  {
    lPiece = &(lPlayer->pieces[i]);
    switch (lPiece->fig)
    {
    case WKNT:
    case BKNT:
      to.x = lPiece->pos.x;
      to.y = lPiece->pos.y;
      to.x = lPiece->pos.x + 2;
      to.y = lPiece->pos.y - 1;
      lMove.from = lPiece->pos;
      lMove.to = to;
      lMove.promotion = EMPT;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      to.x = lPiece->pos.x + 2;
      to.y = lPiece->pos.y + 1;
      lMove.from = lPiece->pos;
      lMove.to = to;
      lMove.promotion = EMPT;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      to.x = lPiece->pos.x - 2;
      to.y = lPiece->pos.y - 1;
      lMove.from = lPiece->pos;
      lMove.to = to;
      lMove.promotion = EMPT;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      to.x = lPiece->pos.x - 2;
      to.y = lPiece->pos.y + 1;
      lMove.from = lPiece->pos;
      lMove.to = to;
      lMove.promotion = EMPT;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      to.x = lPiece->pos.x + 1;
      to.y = lPiece->pos.y - 2;
      lMove.from = lPiece->pos;
      lMove.to = to;
      lMove.promotion = EMPT;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      to.x = lPiece->pos.x + 1;
      to.y = lPiece->pos.y + 2;
      lMove.from = lPiece->pos;
      lMove.to = to;
      lMove.promotion = EMPT;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      to.x = lPiece->pos.x - 1;
      to.y = lPiece->pos.y + 2;
      lMove.from = lPiece->pos;
      lMove.to = to;
      lMove.promotion = EMPT;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      to.x = lPiece->pos.x - 1;
      to.y = lPiece->pos.y - 2;
      lMove.from = lPiece->pos;
      lMove.to = to;
      lMove.promotion = EMPT;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      break;
    case BPWN:
      to.x = lPiece->pos.x;
      to.y = lPiece->pos.y - 1;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      lMove.from = lPiece->pos;
      lMove.to = to;
      //if ((lPiece->fig == WPWN) && (to.y == 7)) lMove.promotion = WQEN;
      //else if ((lPiece->fig == BPWN) && (to.y == 0)) lMove.promotion = BQEN;
      if (to.y == 0) lMove.promotion = BQEN;
      else lMove.promotion = EMPT;
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      
      to.x = lPiece->pos.x;
      to.y = lPiece->pos.y - 2;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      lMove.from = lPiece->pos;
      lMove.to = to;
      lMove.promotion = EMPT;
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      
      to.x = lPiece->pos.x + 1;
      to.y = lPiece->pos.y - 1;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      lMove.from = lPiece->pos;
      lMove.to = to;
      //if ((lPiece->fig == WPWN) && (to.y == 7)) lMove.promotion = WQEN;
      //else if ((lPiece->fig == BPWN) && (to.y == 0)) lMove.promotion = BQEN;
      if (to.y == 0) lMove.promotion = BQEN;
      else lMove.promotion = EMPT;
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      
      to.x = lPiece->pos.x - 1;
      to.y = lPiece->pos.y - 1;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      lMove.from = lPiece->pos;
      lMove.to = to;
      //if ((lPiece->fig == WPWN) && (to.y == 7)) lMove.promotion = WQEN;
      //else if ((lPiece->fig == BPWN) && (to.y == 0)) lMove.promotion = BQEN;
      if (to.y == 0) lMove.promotion = BQEN;
      else lMove.promotion = EMPT;
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      
      break;
    
    case WPWN:
      to.x = lPiece->pos.x;
      to.y = lPiece->pos.y + 1;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      lMove.from = lPiece->pos;
      lMove.to = to;
      //if ((lPiece->fig == WPWN) && (to.y == 7)) lMove.promotion = WQEN;
      //else if ((lPiece->fig == BPWN) && (to.y == 0)) lMove.promotion = BQEN;
      if (to.y == 7) lMove.promotion = WQEN;
      else lMove.promotion = EMPT;
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
      
      to.x = lPiece->pos.x;
      to.y = lPiece->pos.y + 2;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      lMove.from = lPiece->pos;
      lMove.to = to;
      lMove.promotion = EMPT;
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }

      to.x = lPiece->pos.x + 1;
      to.y = lPiece->pos.y + 1;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      lMove.from = lPiece->pos;
      lMove.to = to;
      //if ((lPiece->fig == WPWN) && (to.y == 7)) lMove.promotion = WQEN;
      //else if ((lPiece->fig == BPWN) && (to.y == 0)) lMove.promotion = BQEN;
      if (to.y == 7) lMove.promotion = WQEN;
      else lMove.promotion = EMPT;
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }

      to.x = lPiece->pos.x - 1;
      to.y = lPiece->pos.y + 1;
      SqrToStr(lPiece->pos, &lMoveStr[0]);
      SqrToStr(to, &lMoveStr[2]);
      lMove.from = lPiece->pos;
      lMove.to = to;
      //if ((lPiece->fig == WPWN) && (to.y == 7)) lMove.promotion = WQEN;
      //else if ((lPiece->fig == BPWN) && (to.y == 0)) lMove.promotion = BQEN;
      if (to.y == 7) lMove.promotion = WQEN;
      else lMove.promotion = EMPT;
      if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
      {
        aList[lCount] = lMove;
        lCount++;
        if (aOne)
          return 1;
      }
    }
    i++;
  }
  
  /* KRALe si nechame az na konec */
  lPiece = &(lPlayer->pieces[0]);
  if ((lPiece->fig == WKNG) || (lPiece->fig == BKNG))
  {
    to.x = lPiece->pos.x;
    to.y = lPiece->pos.y;
    /* rochada */
    to.x = lPiece->pos.x + 2;
    to.y = lPiece->pos.y;
    lMove.from = lPiece->pos;
    lMove.to = to;
    lMove.promotion = EMPT;
    SqrToStr(lPiece->pos, &lMoveStr[0]);
    SqrToStr(to, &lMoveStr[2]);
    if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
    {
      aList[lCount] = lMove;
      lCount++;
      if (aOne)
        return 1;
    }
    to.x = lPiece->pos.x - 2;
    to.y = lPiece->pos.y;
    lMove.from = lPiece->pos;
    lMove.to = to;
    lMove.promotion = EMPT;
    SqrToStr(lPiece->pos, &lMoveStr[0]);
    SqrToStr(to, &lMoveStr[2]);
    if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
    {
      aList[lCount] = lMove;
      lCount++;
      if (aOne)
        return 1;
    }
    /* end of rochada */
    to.x = lPiece->pos.x + 1;
    to.y = lPiece->pos.y;
    lMove.from = lPiece->pos;
    lMove.to = to;
    lMove.promotion = EMPT;
    SqrToStr(lPiece->pos, &lMoveStr[0]);
    SqrToStr(to, &lMoveStr[2]);
    if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
    {
      aList[lCount] = lMove;
      lCount++;
      if (aOne)
        return 1;
    }
    to.x = lPiece->pos.x + 1;
    to.y = lPiece->pos.y - 1;
    lMove.from = lPiece->pos;
    lMove.to = to;
    lMove.promotion = EMPT;
    SqrToStr(lPiece->pos, &lMoveStr[0]);
    SqrToStr(to, &lMoveStr[2]);
    if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
    {
      aList[lCount] = lMove;
      lCount++;
      if (aOne)
        return 1;
    }
    to.x = lPiece->pos.x + 1;
    to.y = lPiece->pos.y + 1;
    lMove.from = lPiece->pos;
    lMove.to = to;
    lMove.promotion = EMPT;
    SqrToStr(lPiece->pos, &lMoveStr[0]);
    SqrToStr(to, &lMoveStr[2]);
    if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
    {
      aList[lCount] = lMove;
      lCount++;
      if (aOne)
        return 1;
    }
    to.x = lPiece->pos.x;
    to.y = lPiece->pos.y - 1;
    lMove.from = lPiece->pos;
    lMove.to = to;
    lMove.promotion = EMPT;
    SqrToStr(lPiece->pos, &lMoveStr[0]);
    SqrToStr(to, &lMoveStr[2]);
    if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
    {
      aList[lCount] = lMove;
      lCount++;
      if (aOne)
        return 1;
    }
    to.x = lPiece->pos.x;
    to.y = lPiece->pos.y + 1;
    lMove.from = lPiece->pos;
    lMove.to = to;
    lMove.promotion = EMPT;
    SqrToStr(lPiece->pos, &lMoveStr[0]);
    SqrToStr(to, &lMoveStr[2]);
    if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
    {
      aList[lCount] = lMove;
      lCount++;
      if (aOne)
        return 1;
    }
    to.x = lPiece->pos.x - 1;
    to.y = lPiece->pos.y - 1;
    lMove.from = lPiece->pos;
    lMove.to = to;
    lMove.promotion = EMPT;
    SqrToStr(lPiece->pos, &lMoveStr[0]);
    SqrToStr(to, &lMoveStr[2]);
    if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
    {
      aList[lCount] = lMove;
      lCount++;
      if (aOne)
        return 1;
    }
    to.x = lPiece->pos.x - 1;
    to.y = lPiece->pos.y;
    lMove.from = lPiece->pos;
    lMove.to = to;
    lMove.promotion = EMPT;
    SqrToStr(lPiece->pos, &lMoveStr[0]);
    SqrToStr(to, &lMoveStr[2]);
    if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
    {
      aList[lCount] = lMove;
      lCount++;
      if (aOne)
        return 1;
    }
    to.x = lPiece->pos.x - 1;
    to.y = lPiece->pos.y + 1;
    lMove.from = lPiece->pos;
    lMove.to = to;
    lMove.promotion = EMPT;
    SqrToStr(lPiece->pos, &lMoveStr[0]);
    SqrToStr(to, &lMoveStr[2]);
    if (IsMoveStr(lMoveStr) && IsLegal(lMoveStr, aGame, 0))
    {
      aList[lCount] = lMove;
      lCount++;
      if (aOne)
        return 1;
    }
  }
  else
  {
    printf("# DEBUG: Nulta figura != KRAL ??? BUG!\n");
    PrintPieces(*aGame);
  }
  return lCount;
}

void ROKMoveGen(TGame * aGame, TMoveList aList, int *lCount, int aPiece, int aOne)
{
  int x, y, i, pv = 0;
  TPieceData v[2];
  TSquare to;
  TMove lMove;
  char lMoveStr[5];
  if (aGame->sidetomove == WHITE)
  {
    i = 1;
    while (i < 16)
    {
      while (i < 16 && aGame->whiteplayer.pieces[i].fig != aPiece)
        i++;
      if (i < 16)
      {
        v[pv] = aGame->whiteplayer.pieces[i];
        pv++;
        i++;
      }
    }
  }
  else
  {
    i = 1;
    while (i < 16)
    {
      while (i < 16 && aGame->blackplayer.pieces[i].fig != aPiece)
        i++;
      if (i < 16)
      {
        v[pv] = aGame->blackplayer.pieces[i];
        pv++;
        i++;
      }
    }
  }
  for (i = 0; i < pv; i++)
  {
    /* doprava */
    x = v[i].pos.x + 1;
    y = v[i].pos.y;
    SqrToStr(v[i].pos, &lMoveStr[0]);
    lMove.from = v[i].pos;
    while (x <= 7 && aGame->board[x][y] == EMPT)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
      x++;
    }   /* while doprava ... EMPT */
    if (x <= 7)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
    }   /* posledni doprava ( braci ) */
    /* doleva */
    x = v[i].pos.x - 1;
    while (x >= 0 && aGame->board[x][y] == EMPT)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
      x--;
    }   /* while doleva ... EMPT */
    if (x >= 0)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
    }   /* posledni doleva ( braci ) */
    /* dolu */
    x = v[i].pos.x;
    y = v[i].pos.y - 1;
    while (y >= 0 && aGame->board[x][y] == EMPT)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
      y--;
    }   /* while dolu ... EMPT */
    if (y >= 0)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
    }   /* posledni dolu ( braci ) */
    /* nahoru */
    x = v[i].pos.x;
    y = v[i].pos.y + 1;
    while (y <= 7 && aGame->board[x][y] == EMPT)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
      y++;
    }   /* while nahoru ... EMPT */
    if (y <= 7)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
    }   /* posledni dolu ( braci ) */
  }     /* for i */
}

void BSPMoveGen(TGame * aGame, TMoveList aList, int *lCount, int aPiece, int aOne)
{
  int x, y, i, pv = 0;
  TPieceData v[2];
  TSquare to;
  TMove lMove;
  char lMoveStr[5];
  if (aGame->sidetomove == WHITE)
  {
    i = 1;
    while (i < 16)
    {
      while (i < 16 && aGame->whiteplayer.pieces[i].fig != aPiece)
        i++;
      if (i < 16)
      {
        v[pv] = aGame->whiteplayer.pieces[i];
        pv++;
        i++;
      }
    }
  }
  else
  {
    i = 1;
    while (i < 16)
    {
      while (i < 16 && aGame->blackplayer.pieces[i].fig != aPiece)
        i++;
      if (i < 16)
      {
        v[pv] = aGame->blackplayer.pieces[i];
        pv++;
        i++;
      }
    }
  }
  for (i = 0; i < pv; i++)
  {
    /* doprava nahoru */
    x = v[i].pos.x + 1;
    y = v[i].pos.y + 1;
    SqrToStr(v[i].pos, &lMoveStr[0]);
    lMove.from = v[i].pos;
    while (x <= 7 && y <= 7 && aGame->board[x][y] == EMPT)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
      x++;
      y++;
    }   /* while doprava ... EMPT */
    if (x <= 7 && y <= 7)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
    }   /* posledni doprava a nahoru ( braci ) */
    /* doleva a nahoru */
    x = v[i].pos.x - 1;
    y = v[i].pos.y + 1;
    while (x >= 0 && y <= 7 && aGame->board[x][y] == EMPT)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
      x--;
      y++;
    }   /* while doleva a nahoru ... EMPT */
    if (x >= 0 && y <= 7)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
    }   /* posledni doleva a nahoru ( braci ) */
    /* doprava dolu */
    x = v[i].pos.x + 1;
    y = v[i].pos.y - 1;
    SqrToStr(v[i].pos, &lMoveStr[0]);
    lMove.from = v[i].pos;
    while (x <= 7 && y >= 0 && aGame->board[x][y] == EMPT)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
      x++;
      y--;
    }   /* while doprava ... EMPT */
    if (x <= 7 && y >= 0)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
    }   /* posledni doprava a dolu ( braci ) */
    /* doleva a dolu */
    x = v[i].pos.x - 1;
    y = v[i].pos.y - 1;
    while (x >= 0 && y >= 0 && aGame->board[x][y] == EMPT)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
      x--;
      y--;
    }   /* while doleva a nahoru ... EMPT */
    if (x >= 0 && y >= 0)
    {
      to.x = x;
      to.y = y;
      SqrToStr(to, &lMoveStr[2]);
      if (IsLegal(lMoveStr, aGame, 0))
      {
        lMove.to = to;
        lMove.promotion = EMPT;
        aList[*lCount] = lMove;
        (*lCount)++;
        if (aOne)
          return;
      }
    }   /* posledni (braci) */
  }     /* for i */
}

int SMoveGen(TGame * aGame, TMoveList aList)
{
  int p, i, c = 0;
  p = MoveGen(aGame, aList, 0);
  for (i = 0; i < p; i++)
    if (SMove(aGame, aList[i]))
    {
      aList[c] = aList[i];
      c++;
    }
  return (c);
}
