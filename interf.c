/*
 *  interf.c
 *  Rozhrani s uzivatelem
 */

#include<stdio.h>
#include"data.h"

extern TMove historie[MAXMOVHIS]; /* 1.tah = [1] */
extern int IQ, SIQ;

void PrintMoves(TMoveList aList, int aCount)
{
  int i;
  char lStr[5];
  printf("Possible moves (%d): ", aCount);
  for (i = 0; i < aCount; i++)
  {
    SqrToStr(aList[i].from, &lStr[0]);
    SqrToStr(aList[i].to, &lStr[2]);
    printf("%s ", lStr);
  }
  printf("\n");
}

void PrintBoard(TGame aGame)
{
  int x, y;
  printf("#\n#   +===+===+===+===+===+===+===+===+\n");
  for (y = 7; y >= 0; y--)
  {
    printf("# %d |", y + 1);
    for (x = 0; x <= 7; x++)
      switch (aGame.board[x][y])
      {
      case EMPT:
        printf("   |");
        break;
      case WPWN:
        printf(" P |");
        break;
      case WBSP:
        printf(" B |");
        break;
      case WKNT:
        printf(" N |");
        break;
      case WROK:
        printf(" R |");
        break;
      case WQEN:
        printf(" Q |");
        break;
      case WKNG:
        printf(" K |");
        break;
      case BPWN:
        printf(" p |");
        break;
      case BBSP:
        printf(" b |");
        break;
      case BKNT:
        printf(" n |");
        break;
      case BROK:
        printf(" r |");
        break;
      case BQEN:
        printf(" q |");
        break;
      case BKNG:
        printf(" k |");
        break;
      default:
        printf("****");
      }
    if (y > 0) printf("\n#   +---+---+---+---+---+---+---+---+\n");
    else printf("\n#   +===+===+===+===+===+===+===+===+\n");
  }
  printf("#     a   b   c   d   e   f   g   h\n#\n");
}

void PrintHist(TGame aGame)
{
  int i;
  char odc[3], doc[3];
  for (i = 1; i <= aGame.fHalfMovesCount; i++)
  {
    SqrToStr(historie[i].from, odc);
    SqrToStr(historie[i].to, doc);
    if (i % 2 == 1)
      printf("%d. %2.2s-%2.2s", (i + 1) / 2, odc, doc);
    else
      printf("  %2.2s-%2.2s\n", odc, doc);
  }
  if (aGame.fHalfMovesCount % 2 == 1)
    printf("\n");
}

void PrintPieces(TGame aGame)
/* vytiskne pozice vsech figur */
{
  int i, x;
  char notpoz[3];
  TPlayer *hrac;
  printf("DEBUG: Pozice vsech figur:\n");
  hrac = &(aGame.whiteplayer);
  printf(" whiteplayer: ");
  for (x = 0; x <= 1; x++)
  {
    for (i = 0; i <= 15; i++)
    {
      switch (hrac->pieces[i].fig)
      {
      case WPWN:
        printf("  P");
        break;
      case WBSP:
        printf("  S");
        break;
      case WKNT:
        printf("  J");
        break;
      case WROK:
        printf("  V");
        break;
      case WQEN:
        printf("  Q");
        break;
      case WKNG:
        printf("  K");
        break;
      case BPWN:
        printf(" *P");
        break;
      case BBSP:
        printf(" *S");
        break;
      case BKNT:
        printf(" *J");
        break;
      case BROK:
        printf(" *V");
        break;
      case BQEN:
        printf(" *Q");
        break;
      case BKNG:
        printf(" *K");
        break;
      }
      if ((hrac->pieces[i].fig) != EMPT)
      {
        SqrToStr(hrac->pieces[i].pos, notpoz);
        printf(" :%s ; ", notpoz);
      }
    }
    if (x == 0)
    {
      hrac = &(aGame.blackplayer);
      printf(" \nCERNY: ");
    }
    else
      printf(" \n");
  }
}

void SqrToStr(TSquare p, char *ch)
{
  ch[0] = (char)(p.x + (int)('a'));
  ch[1] = (char)(p.y + (int)('1'));
  ch[2] = '\0';
}

void StrToSqr(char *ch, TSquare *p)
{
  p->x = (int)(ch[0] - 'a');
  p->y = (int)(ch[1] - '1');
}

void PrmToStr(char aPiece, char *ch) // Roland
{
  if ((aPiece == WBSP) || (aPiece == BBSP))
  {
    ch[0] = 'b';
    ch[1] = '\0';
  }
  else if ((aPiece == WKNT) || (aPiece == BKNT))
  {
    ch[0] = 'n';
    ch[1] = '\0';
  }
  else if ((aPiece == WROK) || (aPiece == BROK))
  {
    ch[0] = 'r';
    ch[1] = '\0';
  }
  else if ((aPiece == WQEN) || (aPiece == BQEN))
  {
    ch[0] = 'q';
    ch[1] = '\0';
  }
  else
  {
    ch[0] = '\0';
    ch[1] = '\0';
  }
}

int StrToPrm(char *ch)
{
  if (ch[0] == 'b') return WBSP;
  else if (ch[0] == 'n') return WKNT;
  else if (ch[0] == 'r') return WROK;
  else if (ch[0] == 'q') return WQEN;
  else
    return EMPT;
}

/* soubory */

int ReadConfig(void)
{
  int k, l;
  FILE *f;

  f = fopen(CFGFILE, "rt");
  if (f == NULL)
  {
    printf("Warning: error reading configuration file %s: ", CFGFILE);
    perror(NULL);
    printf("Using default values\n");
    return 0;
  }
  else
  {
    fscanf(f, "%d %d", &k, &l);
    if (k > 0 && l <= 0)
    {
      IQ = k;
      SIQ = l;
    }
    else
    {
      printf("Incorrect values in %s", CFGFILE);
      return 0;
    }
  }
  return 1;
}

void SaveFile(TGame * aGame, char *fname)
{
  FILE *f;
  int i;
  char t[5];
  f = fopen(fname, "wt");
  if (f == NULL)
    perror("Error saving file");
  else
  {
    fprintf(f, "%d\n", aGame->fHalfMovesCount);
    for (i = 1; i <= aGame->fHalfMovesCount; i++)
    {
      SqrToStr(historie[i].from, &t[0]);
      SqrToStr(historie[i].to, &t[2]);
      fprintf(f, "%d %s\n", i, t);
    }
    fclose(f);
    printf("%s written.\n", fname);
  }
}

void OpenFile(TGame * aGame, char *fname)
{
  FILE *f;
  int i, j, pht;
  char lMove[5];
  InitGame(aGame);
  f = fopen(fname, "rt");
  if (f == NULL)
    perror("Error opening file");
  else
  {
    fscanf(f, "%d\n", &pht);  /* j -> zahodit ... */
    for (i = 1; i <= pht; i++)
    {
      if (!feof(f))
      {
        fscanf(f, "%d %s\n", &j, lMove);
        if (j != i)
          printf("%d: Non consistent data\n", i);
        StrToSqr(&(lMove[0]), &(historie[i].from));
        StrToSqr(&(lMove[2]), &(historie[i].to));
        MakeMoveStr(lMove, aGame);
      }
      else
        printf("%d: Non consistent data\n", i);
    }
    fclose(f);
    printf("%s opened.\n", fname);
  }
}
