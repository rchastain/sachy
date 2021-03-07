/*
 *  inic.c
 */

#include <stdlib.h>
#include <string.h>
#include "data.h"

void InitGame(TGame *aGame)
/* postavi pieces na sachovnici a nastavi hrace (jejich pieces ...) */
{
  int x, y, i, j, s1, s2;
  TPlayer *lPlayer;
  
  for (y = 2; y <= 5; y++)
    for (x = 0; x <= 7; x++)
      (*aGame).board[x][y] = EMPT; /* prazdna pole */
  
  for (x = 0; x <= 7; x++)
  {
    aGame->board[x][1] = WPWN; /* pesaci */
    aGame->board[x][6] = BPWN;
  }
  
  aGame->board[0][0] = WROK;  /* ostatni pieces bily */
  aGame->board[1][0] = WKNT;  /* ostatni pieces */
  aGame->board[2][0] = WBSP;  /* ostatni pieces */
  aGame->board[3][0] = WQEN;  /* ostatni pieces */
  aGame->board[4][0] = WKNG;  /* ostatni pieces */
  aGame->board[5][0] = WBSP;  /* ostatni pieces */
  aGame->board[6][0] = WKNT;  /* ostatni pieces */
  aGame->board[7][0] = WROK;  /* ostatni pieces */
  
  aGame->board[0][7] = BROK;  /* ostatni pieces cerny */
  aGame->board[1][7] = BKNT;  /* ostatni pieces */
  aGame->board[2][7] = BBSP;  /* ostatni pieces */
  aGame->board[3][7] = BQEN;  /* ostatni pieces */
  aGame->board[4][7] = BKNG;  /* ostatni pieces */
  aGame->board[5][7] = BBSP;  /* ostatni pieces */
  aGame->board[6][7] = BKNT;  /* ostatni pieces */
  aGame->board[7][7] = BROK;  /* ostatni pieces */
  
  aGame->sidetomove = WHITE;  /* bily je na tahu */
  aGame->fHalfMovesCount = 0;   /* pocet hranych tahu */
  aGame->tmoves = TMOVES;
  aGame->tseconds = TSECONDS;
  aGame->tclockmode = 0;
  aGame->tleft = aGame->tseconds;
  aGame->enpas = -1;    /* zadny pesec */
  
  lPlayer = &(aGame->whiteplayer);
  s1 = 0;
  s2 = 1;
  
  for (i = 1; i <= 2; i++)
  {
    lPlayer->rochada[0] = 1; /* muze malou */
    lPlayer->rochada[1] = 1; /* muze velkou */
    lPlayer->rochada[2] = 0; /* zadna rochada nebyla provedena */
    switch (i)
    {
    case 1:
      lPlayer->pieces[ 0].fig = WKNG;
      lPlayer->pieces[ 1].fig = WQEN;
      lPlayer->pieces[ 2].fig = WROK;
      lPlayer->pieces[ 3].fig = WROK;
      lPlayer->pieces[ 4].fig = WKNT;
      lPlayer->pieces[ 5].fig = WKNT;
      lPlayer->pieces[ 6].fig = WBSP;
      lPlayer->pieces[ 7].fig = WBSP;
      lPlayer->pieces[ 8].fig = WPWN;
      lPlayer->pieces[ 9].fig = WPWN;
      lPlayer->pieces[10].fig = WPWN;
      lPlayer->pieces[11].fig = WPWN;
      lPlayer->pieces[12].fig = WPWN;
      lPlayer->pieces[13].fig = WPWN;
      lPlayer->pieces[14].fig = WPWN;
      lPlayer->pieces[15].fig = WPWN;
      break;
    case 2:
      lPlayer->pieces[ 0].fig = BKNG;
      lPlayer->pieces[ 1].fig = BQEN;
      lPlayer->pieces[ 2].fig = BROK;
      lPlayer->pieces[ 3].fig = BROK;
      lPlayer->pieces[ 4].fig = BKNT;
      lPlayer->pieces[ 5].fig = BKNT;
      lPlayer->pieces[ 6].fig = BBSP;
      lPlayer->pieces[ 7].fig = BBSP;
      lPlayer->pieces[ 8].fig = BPWN;
      lPlayer->pieces[ 9].fig = BPWN;
      lPlayer->pieces[10].fig = BPWN;
      lPlayer->pieces[11].fig = BPWN;
      lPlayer->pieces[12].fig = BPWN;
      lPlayer->pieces[13].fig = BPWN;
      lPlayer->pieces[14].fig = BPWN;
      lPlayer->pieces[15].fig = BPWN;
      break;
    }
    /* jeste nastavit pozice figur */
    lPlayer->pieces[0].pos.x = 4;
    lPlayer->pieces[0].pos.y = s1;
    lPlayer->pieces[1].pos.x = 3;
    lPlayer->pieces[1].pos.y = s1;
    lPlayer->pieces[2].pos.x = 0;
    lPlayer->pieces[2].pos.y = s1;
    lPlayer->pieces[3].pos.x = 7;
    lPlayer->pieces[3].pos.y = s1;
    lPlayer->pieces[4].pos.x = 1;
    lPlayer->pieces[4].pos.y = s1;
    lPlayer->pieces[5].pos.x = 6;
    lPlayer->pieces[5].pos.y = s1;
    lPlayer->pieces[6].pos.x = 2;
    lPlayer->pieces[6].pos.y = s1;
    lPlayer->pieces[7].pos.x = 5;
    lPlayer->pieces[7].pos.y = s1;
    
    for (j = 0; j <= 7; j++)
    {
      lPlayer->pieces[j + 8].pos.x = j;
      lPlayer->pieces[j + 8].pos.y = s2;
    }
    
    /* prohodit hrace a znovu ... */
    lPlayer = &(aGame->blackplayer);
    s1 = 7;
    s2 = 6;
  }
}

void InitGameFromFen(TGame *aGame, char *aPlacement, char *aActiveColor, char *aCastling, char *aEnPassant, char *aHalfMoves, char *aFullMove)
{
  int x, y, i, j, k, l;
  TPlayer *lWPlayer, *lBPlayer;
  
  /*
  printf("aPlacement=[%s]\n", aPlacement);
  printf("aActiveColor=[%s]\n", aActiveColor);
  printf("aCastling=[%s]\n", aCastling);
  printf("aEnPassant=[%s]\n", aEnPassant);
  printf("aHalfMoves=[%s]\n", aHalfMoves);
  printf("aFullMove=[%s]\n", aFullMove);
  */
  
  lWPlayer = &(aGame->whiteplayer);
  lBPlayer = &(aGame->blackplayer);
  
  x = 0;
  y = 7;
  j = 0;
  k = 0;
  for (i = 0; i < strlen(aPlacement); i++)
  {
    switch (aPlacement[i])
    {
      case '/':
        x = 0;
        y--;
        break;
      case '1' ... '8':
        l = aPlacement[i] - '0';
        while (l > 0)
        {
          aGame->board[x][y] = EMPT;
          x++;
          l--;
        }
        break;
      case 'P':
        aGame->board[x][y] = WPWN;
        lWPlayer->pieces[j].fig = aGame->board[x][y];
        lWPlayer->pieces[j].pos.x = x;
        lWPlayer->pieces[j].pos.y = y;
        x++;
        j++;
        break;
      case 'N':
        aGame->board[x][y] = WKNT;
        lWPlayer->pieces[j].fig = aGame->board[x][y];
        lWPlayer->pieces[j].pos.x = x;
        lWPlayer->pieces[j].pos.y = y;
        x++;
        j++;
        break;
      case 'B':
        aGame->board[x][y] = WBSP;
        lWPlayer->pieces[j].fig = aGame->board[x][y];
        lWPlayer->pieces[j].pos.x = x;
        lWPlayer->pieces[j].pos.y = y;
        x++;
        j++;
        break;
      case 'R':
        aGame->board[x][y] = WROK;
        lWPlayer->pieces[j].fig = aGame->board[x][y];
        lWPlayer->pieces[j].pos.x = x;
        lWPlayer->pieces[j].pos.y = y;
        x++;
        j++;
        break;
      case 'Q':
        aGame->board[x][y] = WQEN;
        lWPlayer->pieces[j].fig = aGame->board[x][y];
        lWPlayer->pieces[j].pos.x = x;
        lWPlayer->pieces[j].pos.y = y;
        x++;
        j++;
        break;
      case 'K':
        aGame->board[x][y] = WKNG;
        lWPlayer->pieces[j].fig = aGame->board[x][y];
        lWPlayer->pieces[j].pos.x = x;
        lWPlayer->pieces[j].pos.y = y;
        x++;
        j++;
        break;
      case 'p':
        aGame->board[x][y] = BPWN;
        lBPlayer->pieces[k].fig = aGame->board[x][y];
        lBPlayer->pieces[k].pos.x = x;
        lBPlayer->pieces[k].pos.y = y;
        x++;
        k++;
        break;
      case 'n':
        aGame->board[x][y] = BKNT;
        lBPlayer->pieces[k].fig = aGame->board[x][y];
        lBPlayer->pieces[k].pos.x = x;
        lBPlayer->pieces[k].pos.y = y;
        x++;
        k++;
        break;
      case 'b':
        aGame->board[x][y] = BBSP;
        lBPlayer->pieces[k].fig = aGame->board[x][y];
        lBPlayer->pieces[k].pos.x = x;
        lBPlayer->pieces[k].pos.y = y;
        x++;
        k++;
        break;
      case 'r':
        aGame->board[x][y] = BROK;
        lBPlayer->pieces[k].fig = aGame->board[x][y];
        lBPlayer->pieces[k].pos.x = x;
        lBPlayer->pieces[k].pos.y = y;
        x++;
        k++;
        break;
      case 'q':
        aGame->board[x][y] = BQEN;
        lBPlayer->pieces[k].fig = aGame->board[x][y];
        lBPlayer->pieces[k].pos.x = x;
        lBPlayer->pieces[k].pos.y = y;
        x++;
        k++;
        break;
      case 'k':
        aGame->board[x][y] = BKNG;
        lBPlayer->pieces[k].fig = aGame->board[x][y];
        lBPlayer->pieces[k].pos.x = x;
        lBPlayer->pieces[k].pos.y = y;
        x++;
        k++;
        break;
    }
  }
  
  if (aActiveColor[0] == 'w')
    aGame->sidetomove = WHITE;
  else
    aGame->sidetomove = BLACK;
  
  aGame->fHalfMovesCount = 2 * atoi(aFullMove) - (aGame->sidetomove == WHITE ? 2 : 1);
  aGame->tmoves = TMOVES;
  aGame->tseconds = TSECONDS;
  aGame->tclockmode = 0;
  aGame->tleft = aGame->tseconds;
  
  if (aEnPassant[0] == '-')
    aGame->enpas = -1;
  else
    aGame->enpas = aEnPassant[0] - 'a';
  
  lWPlayer->rochada[0] = strchr(aCastling, 'K') != NULL ? 1 : 0;
  lWPlayer->rochada[1] = strchr(aCastling, 'Q') != NULL ? 1 : 0;
  lWPlayer->rochada[2] = lWPlayer->rochada[0] == 1 && lWPlayer->rochada[1] == 1 ? 0 : 1;
  
  lBPlayer->rochada[0] = strchr(aCastling, 'K') != NULL ? 1 : 0;
  lBPlayer->rochada[1] = strchr(aCastling, 'Q') != NULL ? 1 : 0;
  lBPlayer->rochada[2] = lBPlayer->rochada[0] == 1 && lBPlayer->rochada[1] == 1 ? 0 : 1; 
}
