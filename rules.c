/*
 *  rules.c
 *  Implementace pravidel
 */

#include <stdlib.h>
#include <stdio.h>
#include "data.h"

#define abs(x) (x > 0 ? x : -x)
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

int TahFigurou2(TMove, TGame *);

int IsMoveStr(char *aStr)
{
  if (
    aStr[0] >= 'a' && aStr[0] <= 'h' &&
    aStr[2] >= 'a' && aStr[2] <= 'h' &&
    aStr[1] >= '1' && aStr[1] <= '8' &&
    aStr[3] >= '1' && aStr[3] <= '8'
  )
    return 1;
  else
    return 0;
}

int IsPossible(char *aMoveStr, TGame *aGame, int aOutput)
{
  int lPiece, lOutput;
  TMove lMove;
  StrToSqr(&aMoveStr[0], &lMove.from);
  StrToSqr(&aMoveStr[2], &lMove.to);
  lPiece = aGame->board[lMove.from.x][lMove.from.y];
  /* stoji tam figura? */
  if (lPiece == EMPT)
  {
    lOutput = aOutput && printf("# Illegal move (nothing to move): %s\n", aMoveStr);
    return 0;
  }
  /* stoji tam figura stejne barvy? */
  switch (aGame->sidetomove)
  {
  case WHITE:
    if (BlackPiece(lPiece))
    {
      lOutput = aOutput && printf("# Illegal move (not your figure): %s\n", aMoveStr);
      return 0;
    }
    break;
  case BLACK:
    if (WhitePiece(lPiece))
    {
      lOutput = aOutput && printf("# Illegal move (not your figure): %s\n", aMoveStr);
      return 0;
    }
    break;
  }
  /* lze tou figurou takto tahnout? */
  if (!PieceCanMove(lMove, aGame))
  {
    lOutput = aOutput && printf("# Illegal move (ambiguous move): %s\n", aMoveStr);
    return 0;
  }
  /* volna cesta ? */
  if ((lPiece == WQEN || lPiece == BQEN || lPiece == WROK || lPiece == BROK || lPiece == WBSP || lPiece == BBSP)
      && !(EmptyPath(lMove, aGame->board)))
  {
    lOutput = aOutput && printf("# Illegal move (occupied way): %s\n", aMoveStr);
    return 0;
  }
  
  if (lOutput) {};
  
  return 1;
}

int IsLegal(char *aMoveStr, TGame * aGame, int aOutput)
{
  /* zda je tah podle pravidel */
  TSquare from, to;
  int lPiece, lOutput;
  TMove lMove;
  static TGame lGame;
  StrToSqr(&aMoveStr[0], &from);
  StrToSqr(&aMoveStr[2], &to);
  lMove.from = from;
  lMove.to = to;
  /* to ... neni tam figura stejne barvy? */
  lPiece = aGame->board[to.x][to.y];
  if (lPiece != EMPT) switch (aGame->sidetomove)
    {
    case WHITE:
      if (WhitePiece(lPiece))
      {
        lOutput = aOutput && printf("# Illegal move (can't take own figure): %s\n", aMoveStr);
        return 0;
      }
      break;
    case BLACK:
      if (BlackPiece(lPiece))
      {
        lOutput = aOutput && printf("# Illegal move (can't take own figure): %s\n", aMoveStr);
        return 0;
      }
      break;
    }
  /* lze tou figurou takto tahnout? volne cesty jsou v IsPossible() */
  lPiece = aGame->board[from.x][from.y];
  if ((lPiece == WKNG || lPiece == BKNG || lPiece == WPWN || lPiece == BPWN) && !TahFigurou2(lMove, aGame))
  {
    lOutput = aOutput && printf("# Illegal move (ambiguous move): %s\n", aMoveStr);
    return 0;
  }
  /* nebudu pak v sachu? */
  /* proved v kopii hry a pokud pak nebude v sachu, tak
     tu kopii zkopni do originalu ... jinak exit */
  lGame = *aGame;
  MakeMoveStr(aMoveStr, &lGame);
  /* printf(" ... test sachu ...\n"); */
  if (InCheck(aGame->sidetomove, &lGame))
  {
    lOutput = aOutput && printf("# Illegal move (moving into check): %s\n", aMoveStr);
    return 0;
  }
  if (lOutput) {};
  /* vse je tedy ok */
  return 1;
}

int WhitePiece(int aPiece)
{
  if (aPiece == WPWN || aPiece == WQEN || aPiece == WKNG || aPiece == WROK || aPiece == WKNT || aPiece == WBSP)
    return 1;
  else
    return 0;
}

int BlackPiece(int aPiece)
{
  if (aPiece == BPWN || aPiece == BQEN || aPiece == BKNG || aPiece == BROK || aPiece == BKNT || aPiece == BBSP)
    return 1;
  else
    return 0;
}

int TahFigurou2(TMove aMove, TGame * aGame)
{
  TPieceData fig;
  int dx, dy;
  static TGame TH;
  /* nastav fig */
  fig.fig = aGame->board[aMove.from.x][aMove.from.y];
  fig.pos.x = aMove.from.x;
  fig.pos.y = aMove.from.y;
  /* jestli je to pesec, tak zavolej specialni proceduru pro pesce, jinou figuru rovnou testni */
  dx = (int)(aMove.to.x - aMove.from.x);
  dy = (int)(aMove.to.y - aMove.from.y);
  switch (fig.fig)
  {
  case WPWN:
  case BPWN:
    if ((PawnCanMove(aMove, aGame)))
      return 1;
    break;
  case WKNG:
    if ((abs(dx) <= 1) && dy <= 1)
      return 1;
    else if ((dx == 2) && (dy == 0) && (aGame->whiteplayer.rochada[0]) &&
             (aGame->board[5][0] == EMPT) &&
             (aGame->board[6][0] == EMPT) &&  /* mala rochada */
             !InCheck(WHITE, aGame))
    {
      TH = *aGame;
      MakeMoveStr("e1f1", &TH);
      if (InCheck(WHITE, &TH))
        return 0;
      else
        return 1;
    }
    else if ((dx == -2) && (dy == 0) && (aGame->whiteplayer.rochada[1]) &&
             (aGame->board[3][0] == EMPT) &&
             (aGame->board[2][0] == EMPT) &&  /* velka rochada */
             (aGame->board[1][0] == EMPT) &&
             !InCheck(WHITE, aGame))
    {
      TH = *aGame;
      MakeMoveStr("e1d1", &TH);
      if (InCheck(WHITE, &TH))
        return 0;
      else
        return 1;
    }
    break;
  case BKNG:
    if ((abs(dx) <= 1) && dy <= 1)
      return 1;
    else if ((dx == 2) && (dy == 0) && (aGame->blackplayer.rochada[0]) &&
             (aGame->board[5][7] == EMPT) &&
             (aGame->board[6][7] == EMPT) &&  /* mala rochada */
             !InCheck(BLACK, aGame))
    {
      TH = *aGame;
      MakeMoveStr("e8f8", &TH);
      if (InCheck(BLACK, &TH))
        return 0;
      else
        return 1;
    }
    else if ((dx == -2) && (dy == 0) && (aGame->blackplayer.rochada[1]) &&
             (aGame->board[3][7] == EMPT) &&
             (aGame->board[2][7] == EMPT) &&  /* velka rochada */
             (aGame->board[1][7] == EMPT) &&
             !InCheck(BLACK, aGame))
    {
      TH = *aGame;
      MakeMoveStr("e8d8", &TH);
      if (InCheck(BLACK, &TH))
        return 0;
      else
        return 1;
    }
    break;
  }
  return 0;
}

int PieceCanMove(TMove aMove, TGame * aGame)
{
  TPieceData fig;
  int dx, dy;
  static TGame TH;
  /* nastav fig */
  fig.fig = aGame->board[aMove.from.x][aMove.from.y];
  fig.pos.x = aMove.from.x;
  fig.pos.y = aMove.from.y;
  /* jestli je to pesec, tak zavolej specialni proceduru pro pesce, jinou figuru rovnou testni */
  dx = aMove.to.x - aMove.from.x;
  dy = aMove.to.y - aMove.from.y;
  switch (fig.fig)
  {
  case WPWN:
  case BPWN:
    if ((PawnCanMove(aMove, aGame)))
      return 1;
    break;
  case WKNG:
    if ((abs(dx) <= 1) && dy <= 1)
      return 1;
    else if ((dx == 2) && (dy == 0) && (aGame->whiteplayer.rochada[0]) &&
             (aGame->board[5][0] == EMPT) &&
             (aGame->board[6][0] == EMPT) &&  /* mala rochada */
             !InCheck(WHITE, aGame))
    {
      TH = *aGame;
      MakeMoveStr("e1f1", &TH);
      if (InCheck(WHITE, &TH))
        return 0;
      else
        return 1;
    }
    else if ((dx == -2) && (dy == 0) && (aGame->whiteplayer.rochada[1]) &&
             (aGame->board[3][0] == EMPT) &&
             (aGame->board[2][0] == EMPT) &&  /* velka rochada */
             (aGame->board[1][0] == EMPT) &&
             !InCheck(WHITE, aGame))
    {
      TH = *aGame;
      MakeMoveStr("e1d1", &TH);
      if (InCheck(WHITE, &TH))
        return 0;
      else
        return 1;
    }
    break;
  case BKNG:
    if ((abs(dx) <= 1) && dy <= 1)
      return 1;
    else if ((dx == 2) && (dy == 0) && (aGame->blackplayer.rochada[0]) &&
             (aGame->board[5][7] == EMPT) &&
             (aGame->board[6][7] == EMPT) &&  /* mala rochada */
             !InCheck(BLACK, aGame))
    {
      TH = *aGame;
      MakeMoveStr("e8f8", &TH);
      if (InCheck(BLACK, &TH))
        return 0;
      else
        return 1;
    }
    else if ((dx == -2) && (dy == 0) && (aGame->blackplayer.rochada[1]) &&
             (aGame->board[3][7] == EMPT) &&
             (aGame->board[2][7] == EMPT) &&  /* velka rochada */
             (aGame->board[1][7] == EMPT) &&
             !InCheck(BLACK, aGame))
    {
      TH = *aGame;
      MakeMoveStr("e8d8", &TH);
      if (InCheck(BLACK, &TH))
        return 0;
      else
        return 1;
    }
    break;
  case WKNT:
  case BKNT:
    if ((abs(dx) == 2) && (abs(dy) == 1))
      return 1;
    else if ((abs(dx) == 1) && (abs(dy) == 2))
      return 1;
    break;
  case WBSP:
  case BBSP:
  case WQEN:
  case BQEN:
    if (((abs(dx) == abs(dy)) || (dy * dx == 0))
        /* && EmptyPath(aMove, aGame->board) */)
      return 1;
    break;
  case WROK:
  case BROK:
    if (((dx == 0) || (dy == 0))
        /* && EmptyPath(aMove, aGame->board) */)
      return 1;
    break;
  }
  return 0;
}

int PawnCanMove(TMove aMove, TGame * aGame)
{
  /* zda je aMove podle pravidel */
  int dx, dy;
  dx = aMove.to.x - aMove.from.x;
  dy = aMove.to.y - aMove.from.y;
  switch (aGame->sidetomove)
  {
  case WHITE:
    if (dx == 0)    /* tah dopredu */
    {
      if ((dy == 2) && (aMove.from.y == 1) &&
          (aGame->board[aMove.from.x][2] == EMPT) &&
          (aGame->board[aMove.from.x][aMove.to.y] == EMPT))
        return 1;
      else if ((dy == 1) &&
               (aGame->board[aMove.from.x][aMove.to.y] == EMPT))
        return 1;
    }   /* aMove sikmo */
    if (((dy * abs(dx)) == 1) &&
        aGame->board[aMove.to.x][aMove.to.y] != EMPT)
      return 1;
    else if ((aMove.to.x == aGame->enpas) && (aMove.to.y == 5) &&
             ((dy * abs(dx)) == 1) &&
             aGame->board[aMove.to.x][aMove.to.y] == EMPT)
      return 1;
    break;
  case BLACK:
    if (dx == 0)    /* taj dopredu */
    {
      if ((dy == -2) && (aMove.from.y == 6) &&
          (aGame->board[aMove.from.x][5] == EMPT) &&
          (aGame->board[aMove.from.x][aMove.to.y] == EMPT))
        return 1;
      else if ((dy == -1) &&
               (aGame->board[aMove.from.x][aMove.to.y] == EMPT))
        return 1;
    }   /* tah sikmo */
    if (((dy * abs(dx)) == -1) &&
        aGame->board[aMove.to.x][aMove.to.y] != EMPT)
      return 1;
    else if ((aMove.to.x == aGame->enpas) && (aMove.to.y == 2) &&
             ((dy * abs(dx)) == -1) &&
             aGame->board[aMove.to.x][aMove.to.y] == EMPT)
      return 1;
    break;
  }
  return 0;
}

int EmptyPath(TMove aMove, TBoard sach)
{
  int i, j, odx, dox, ody, doy, sx, sy;
  odx = aMove.from.x;
  dox = aMove.to.x;
  ody = aMove.from.y;
  doy = aMove.to.y;
  sx = (int)(dox - odx);
  if (sx != 0)
    sx = sx / abs(sx);
  sy = (int)(doy - ody);
  if (sy != 0)
    sy = sy / abs(sy);
  i = odx + sx;
  j = ody + sy;
  if ((sx >= 0) && (sy >= 0))
    while ((i <= (dox - sx)) && (j <= (doy - sy)) && (i >= 0) && (j >= 0))
    {
      if (sach[i][j] != EMPT)
        return 0;
      i += sx;
      j += sy;
    }
  else if ((sx >= 0) && (sy < 0))
    while ((i <= (dox - sx)) && (j >= (doy - sy)) && (i >= 0) && (j >= 0))
    {
      if (sach[i][j] != EMPT)
        return 0;
      i += sx;
      j += sy;
    }
  else if ((sx < 0) && (sy >= 0))
    while ((i >= (dox - sx)) && (j <= (doy - sy)) && (i >= 0) && (j >= 0))
    {
      if (sach[i][j] != EMPT)
        return 0;
      i += sx;
      j += sy;
    }
  else if ((sx < 0) && (sy < 0))
    while ((i >= (dox - sx)) && (j >= (doy - sy)) && (i >= 0) && (j >= 0))
    {
      if (sach[i][j] != EMPT)
        return 0;
      i += sx;
      j += sy;
    }
  return 1;
}

int FindPiece(TSquare aSquare, TBoard aBoard, TPlayer * aPlayer, int * aPieceIndex)
{
  int i = 0, lPiece = aBoard[aSquare.x][aSquare.y];
  while (
    i <= 15
    && !(
      (lPiece == (aPlayer->pieces[i].fig))
      && (aSquare.x == aPlayer->pieces[i].pos.x)
      && (aSquare.y == aPlayer->pieces[i].pos.y)
    )
  )
    i++;
  if (i > 15)
    return 0;
  else
    *aPieceIndex = i;
  return 1;
}

void RemovePiece(TPlayer * aPlayer)
{
  int i = 0;
  while (i <= 15 && (aPlayer->pieces[i].fig != EMPT))
    i++;
  if (i > 15)
  {
    fprintf(stderr, "Internal error (%s, line %d)\n", __FILE__, __LINE__);
    //return;
    exit(0);
  }
  for (i = i; i < 15; i++)
    aPlayer->pieces[i] = aPlayer->pieces[i + 1];
  aPlayer->pieces[15].fig = EMPT;
}

int IsRookFamily(int aPiece, int aColor)
{
  if (aColor == WHITE)
  {
    switch (aPiece)
    {
    case BQEN:
    case BROK:
      return 1;
    }
  }
  else
  {
    switch (aPiece)
    {
    case WQEN:
    case WROK:
      return 1;
    }
  }
  return 0;
}

int IsBishopFamily(int aPiece, int aColor)
{
  if (aColor == WHITE)
  {
    switch (aPiece)
    {
    case BQEN:
    case BBSP:
      return 1;
    }
  }
  else
  {
    switch (aPiece)
    {
    case WQEN:
    case WBSP:
      return 1;
    }
  }
  return 0;
}

int IsKnight(int aPiece, int aColor)
{
  switch (aColor)
  {
  case WHITE:
    if (aPiece == BKNT)
      return 1;
    break;
  case BLACK:
    if (aPiece == WKNT)
      return 1;
    break;
  }
  return 0;
}

int InCheck(int ZH, TGame * aGame)
{
  int kx = 0, ky = 0, i, j;
  switch (ZH)
  {
  case WHITE:
    kx = aGame->whiteplayer.pieces[0].pos.x;
    ky = aGame->whiteplayer.pieces[0].pos.y;
    break;
  case BLACK:
    kx = aGame->blackplayer.pieces[0].pos.x;
    ky = aGame->blackplayer.pieces[0].pos.y;
    break;
  }
  /* projed kolmice a diagonaly od krale ... */
  /* kolmice */
  i = kx + 1;
  j = ky;
  while (i <= 7 && (aGame->board[i][j] == EMPT))
    i++;    /* zprava */
  if (i <= 7 && IsRookFamily(aGame->board[i][j], ZH))
    return 1;
  i = kx - 1;
  j = ky;
  while (i >= 0 && (aGame->board[i][j] == EMPT))
    i--;    /* zleva */
  if (i >= 0 && IsRookFamily(aGame->board[i][j], ZH))
    return 1;
  i = kx;
  j = ky + 1;
  while (j <= 7 && (aGame->board[i][j] == EMPT))
    j++;    /* shora */
  if (j <= 7 && IsRookFamily(aGame->board[i][j], ZH))
    return 1;
  i = kx;
  j = ky - 1;
  while (j >= 0 && (aGame->board[i][j] == EMPT))
    j--;    /* zdola */
  if (j >= 0 && IsRookFamily(aGame->board[i][j], ZH))
    return 1;
  /* pesci */
  switch (ZH)
  {
  case WHITE:
    if (kx <= 6 && ky <= 6 && (aGame->board[kx + 1][ky + 1] == BPWN))
      return 1;
    if (kx >= 1 && ky <= 6 && (aGame->board[kx - 1][ky + 1] == BPWN))
      return 1;
    break;
  case BLACK:
    if (kx <= 6 && ky >= 1 && (aGame->board[kx + 1][ky - 1] == WPWN))
      return 1;
    if (kx >= 1 && ky >= 1 && (aGame->board[kx - 1][ky - 1] == WPWN))
      return 1;
    break;
  }
  /* diagonalne */
  i = kx + 1;
  j = ky + 1;
  while (i <= 7 && j <= 7 && (aGame->board[i][j] == EMPT))
  {
    i++;
    j++;
  }     /* zprzhor */
  if (i <= 7 && j <= 7 && IsBishopFamily(aGame->board[i][j], ZH))
    return 1;
  i = kx - 1;
  j = ky + 1;
  while (i >= 0 && j <= 7 && (aGame->board[i][j] == EMPT))
  {
    i--;
    j++;
  }     /* zlezhor */
  if (i >= 0 && j <= 7 && IsBishopFamily(aGame->board[i][j], ZH))
    return 1;
  i = kx + 1;
  j = ky - 1;
  while (i <= 7 && j >= 0 && (aGame->board[i][j] == EMPT))
  {
    i++;
    j--;
  }     /* zprzdol */
  if (i <= 7 && j >= 0 && IsBishopFamily(aGame->board[i][j], ZH))
    return 1;
  i = kx - 1;
  j = ky - 1;
  while (i >= 0 && j >= 0 && (aGame->board[i][j] == EMPT))
  {
    i--;
    j--;
  }     /* zlezdol */
  if (i >= 0 && j >= 0 && IsBishopFamily(aGame->board[i][j], ZH))
    return 1;
  /* zkontroluj kone */
  i = kx + 1;
  j = ky + 2;
  if (i <= 7 && j <= 7 && IsKnight(aGame->board[i][j], ZH))
    return 1;
  i = kx + 1;
  j = ky - 2;
  if (i <= 7 && j >= 0 && IsKnight(aGame->board[i][j], ZH))
    return 1;
  i = kx - 1;
  j = ky + 2;
  if (i >= 0 && j <= 7 && IsKnight(aGame->board[i][j], ZH))
    return 1;
  i = kx - 1;
  j = ky - 2;
  if (i >= 0 && j >= 0 && IsKnight(aGame->board[i][j], ZH))
    return 1;
  i = kx + 2;
  j = ky + 1;
  if (i <= 7 && j <= 7 && IsKnight(aGame->board[i][j], ZH))
    return 1;
  i = kx + 2;
  j = ky - 1;
  if (i <= 7 && j >= 0 && IsKnight(aGame->board[i][j], ZH))
    return 1;
  i = kx - 2;
  j = ky + 1;
  if (i >= 0 && j <= 7 && IsKnight(aGame->board[i][j], ZH))
    return 1;
  i = kx - 2;
  j = ky - 1;
  if (i >= 0 && j >= 0 && IsKnight(aGame->board[i][j], ZH))
    return 1;
  /* kralove nesmeji byt u sebe */
  if ((2 > abs((int)(aGame->whiteplayer.pieces[0].pos.y - aGame->blackplayer.pieces[0].pos.y))) &&
      (2 > abs((int)(aGame->whiteplayer.pieces[0].pos.x - aGame->blackplayer.pieces[0].pos.x))))
    return 1;
  
  return 0;
}

int IsMate(TGame * aGame)
{
  TMoveList mg;
  if (MoveGen(aGame, mg, 1))
    return 0;
  else if (InCheck(aGame->sidetomove, aGame))
    return 1;
  else
    return 0;
}

int IsStaleMate(TGame * aGame)
{
  TMoveList lList;
  if (MoveGen(aGame, lList, 1))
    return 0;
  else if (InCheck(aGame->sidetomove, aGame))
    return 0;
  else
    return 1;
}

int SMove(TGame * aGame, TMove aMove)
{
  int lPiece;
  lPiece = aGame->board[aMove.to.x][aMove.to.y];
  if (lPiece != EMPT && lPiece != WKNG && lPiece != BKNG) /* braci tah */
    return 1;
  if (InCheck(aGame->sidetomove, aGame))
    return 1;
  else
    return 0;
}
