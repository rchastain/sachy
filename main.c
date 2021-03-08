/*
 *  main.c
 *  Sachovy program pro Unix
 *  Martin Macok
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"

#ifdef __linux__
#include <signal.h>
#endif

FILE *f1 = NULL;
FILE *f2 = NULL;

void ToLog(const char *aText, const int aSecondFile)
{
  time_t lTime = time(NULL);
  struct tm *lLocalTime = localtime(&lTime);
  char lTimeStr[32];
  FILE *lFile;
  strftime(lTimeStr, sizeof(lTimeStr) - 1, "%d/%m/%Y %H:%M:%S", lLocalTime);
  lFile = (aSecondFile == 0) ? f1 : f2;
  fprintf(lFile, "%s %s\n", lTimeStr, aText);
  fflush(lFile);
}

char logBuffer[256];

void Undo(TGame *);   /* vezme zpet posledni tah */

int IQ = DEPTH, SIQ = SDEPTH;
TMove historie[MAXMOVHIS];  /* 1.tah = [1] */

int main(int argc, char **argv)
{
  TGame lGame;
  TMoveList lList;
  TBook lBook;
  char lInput[80], lCmd[80], lFileName[30], lMoveStr[/*5*/6];
  char lPlacement[72], lActiveColor[2], lCastling[5], lEnPassant[3], lHalfMoves[4], lFullMove[4];
  int i, lComputerColor = BLACK, lTimeElapsed, lUseBook, pt, vt, tseconds2;
  time_t lTime;
  struct tm* lTimeStruct;
  char lLogName[30];
  
  time(&lTime);
  srand((unsigned int) lTime);
  
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  
#ifdef __linux__
  if (0) {
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
  }
  else
  {
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
  }
#endif
  
  printf("*** Sachy (simple chess program) v%s ***\n", VERSION "." REVISION);
  
  lTimeStruct = localtime(&lTime);
  strftime(lLogName, 30, "%Y%m%d%H%M%S-1.log", lTimeStruct);
  f1 = fopen(lLogName, "w");
  strftime(lLogName, 30, "%Y%m%d%H%M%S-2.log", lTimeStruct);
  f2 = fopen(lLogName, "w");
  
  InitGame(&lGame);
  
  if (0) ReadConfig();
  if (OpenBook(&lBook))
    lUseBook = 1;
  else
    lUseBook = 0;
  
  do
  {
    if (0)
      fflush(stdout);
    
    /* Game state */
    
    /*
    if (IsStaleMate(&lGame))
      printf("# Draw\n");
    else if (IsMate(&lGame))
      printf("# Checkmate\n");
    else if (InCheck(lGame.sidetomove, &lGame))
      printf("# Check\n");
    */
    
    /* Prompt */
    
    if (0) {
      printf("(%d)", (lGame.fHalfMovesCount) / 2 + 1);
      if (lGame.sidetomove == WHITE)
        printf(" White>\n");
      else
        printf(" Black>\n");
    }
    
    /* Engine move */
    
    if (lGame.sidetomove == lComputerColor)
    {
      printf("# Thinking\n");
      
      if (lUseBook)
      {
        pt = lBook.p[lBook.aktual].poctah;
        if (pt > 0)
        {
          vt = rand() % pt;
          strcpy(lMoveStr, lBook.p[lBook.aktual].tahy[vt]);
          lBook.aktual = lBook.p[lBook.aktual].to[vt];
          lTimeElapsed = 0;
        }
        else
          lUseBook = 0;
      }
      
      if (lUseBook == 0)
        lTimeElapsed = BestMove2(&lGame, lMoveStr, 1);
      
      if (lTimeElapsed >= 0)
      {
        lGame.tleft -= lTimeElapsed;
        if (((lGame.fHalfMovesCount + 3) / 2) == lGame.tmoves)
          lGame.tleft += lGame.tseconds;
        MakeMoveStr(lMoveStr, &lGame);
        PrintBoard(lGame);
        printf("# Time left: %d seconds\n", lGame.tleft);
        printf("move %s\n", lMoveStr);
      }
      else
      {
        printf("# Engine error");
        lComputerColor = NONE;
      }
      
      continue;
    }
    
    for (i = 0; (lInput[i] = getchar()) != '\n'; i++);
    lInput[i] = 0;
    sscanf(lInput, "%s", lCmd);
    
    sprintf(logBuffer, "%s %d -> %s", __FILE__, __LINE__, lInput);
    ToLog(logBuffer, 0);
    
    /* CECP commands */
    
    if (strcmp (lCmd, "protover") == 0)
    {
      printf("feature analyze=0\n");
      printf("feature colors=0\n");
      printf("feature debug=1\n");
      printf("feature myname=\"Sachy " VERSION "." REVISION "\"\n");
      printf("feature ping=1\n");
      printf("feature sigint=0\n");
      printf("feature sigterm=0\n");
      printf("feature variants=\"normal\"\n");
      printf("feature done=1\n");
      //fflush(stdout);
      continue;
    }
    else if (strcmp(lCmd, "new") == 0)
    {
      InitGame(&lGame);
      lComputerColor = BLACK;
      lUseBook = 1;
      lBook.aktual = 0;
    }
    else if (strcmp(lCmd, "setboard") == 0)
    {
      if (sscanf(lInput, "setboard %s %s %s %s %s %s", lPlacement, lActiveColor, lCastling, lEnPassant, lHalfMoves, lFullMove) != 6)
        printf("# Error parsing setboard command\n");
      else
        InitGameFromFen(&lGame, lPlacement, lActiveColor, lCastling, lEnPassant, lHalfMoves, lFullMove);
    }
    else if (strcmp(lCmd, "draw") == 0)
      printf("offer draw\n");
    else if (strcmp(lCmd, "go") == 0)
      lComputerColor = lGame.sidetomove;
    else if (strcmp(lCmd, "hint") == 0)
    {
      lTimeElapsed = BestMove2(&lGame, lMoveStr, 1);
      if (lTimeElapsed > 0)
        printf("Hint: %s\n", lMoveStr);
      else
        printf("Hint: a1a1\n");
    }
    else if (strcmp(lCmd, "level") == 0)
    {
      tseconds2 = 0;
      if (sscanf(lInput, "level %d %d %d", &lGame.tmoves, &lGame.tseconds, &lGame.tclockmode) != 3
       && sscanf(lInput, "level %d %d:%d %d", &lGame.tmoves, &lGame.tseconds, &tseconds2, &lGame.tclockmode) != 4)
        printf("# Error parsing level command\n");
      lGame.tseconds *= 60;
      lGame.tseconds += tseconds2;
      lGame.tleft = lGame.tseconds;
      if (lGame.tclockmode != 0)
        printf("# Only conventional clock mode is supported\n");
    }
    else if (strcmp(lCmd, "white") == 0)
    {
      lComputerColor = BLACK;
      lGame.sidetomove = WHITE;
    }
    else if (strcmp(lCmd, "black") == 0)
    {
      lComputerColor = WHITE;
      lGame.sidetomove = BLACK;
    }
    else if (strcmp(lCmd, "force") == 0)
      lComputerColor = NONE;
    else if (strcmp(lCmd, "undo") == 0)
    {
      Undo(&lGame);
      lUseBook = 0;
    }
    else if (strcmp(lCmd, "ping") == 0)
      if (sscanf(lInput, "ping %d", &i) == 1)
        printf("pong %d\n", i);
      else
        printf("pong 1\n");
    else if (strcmp(lCmd, "sd") == 0)
      if (0)
        sscanf(lInput, "sd %d", &lGame.tleft);
      else
        continue;
    else if (strcmp(lCmd, "st") == 0)
      if (0)
        sscanf(lInput, "st %d", &lGame.tleft);
      else
        continue;
    else if (strcmp(lCmd, "time") == 0)
      if (0)
        sscanf(lInput, "time %d", &lGame.tleft);
      else
        continue;
    else if (strcmp(lCmd, "?") == 0)
      printf("move @@@@\n");
    else if (strcmp(lCmd, "quit") == 0)
      break;
    else if (strcmp(lCmd, "remove") == 0)
    {
      Undo(&lGame);
      Undo(&lGame);
      lUseBook = 0;
    }
    else if (
      strcmp(lCmd, "accepted") == 0 ||
      strcmp(lCmd, "hard") == 0 ||
      strcmp(lCmd, "otim") == 0 ||
      strcmp(lCmd, "post") == 0 ||
      strcmp(lCmd, "random") == 0 ||
      strcmp(lCmd, "rejected") == 0 ||
      strcmp(lCmd, "result") == 0 ||
      strcmp(lCmd, "xboard") == 0
    )
      continue;
    
    /* Custom commands */
    
    else if (strcmp(lCmd, "book") == 0)
      PrintBook(&lBook);
    else if (strcmp(lCmd, "board") == 0)
      PrintBoard(lGame);
    else if (strcmp(lCmd, "test") == 0)
    {
      printf("Benchmark v1: %6d positions per second\n", Benchmark(&lGame));
      printf("Benchmark v2: %6d positions per second\n", Benchmark2(&lGame));
    }
    else if (strcmp(lCmd, "pieces") == 0)
      PrintPieces(lGame);
    else if (strcmp(lCmd, "eval") == 0)
      printf("%d\n", Evaluate(&lGame));
    else if (strcmp(lCmd, "hist") == 0)
      PrintHist(lGame);
    else if (strcmp(lCmd, "slist") == 0)
    {
      i = SMoveGen(&lGame, lList);
      PrintMoves(lList, i);
    }
    else if (strcmp(lCmd, "list") == 0)
    {
      i = MoveGen(&lGame, lList, 0);
      PrintMoves(lList, i);
    }
    else if (strcmp(lCmd, "ply") == 0)
    {
      scanf("%d", &i);
      if (i > 0)
        IQ = i;
    }
    else if (strcmp(lCmd, "sply") == 0)
    {
      scanf("%d", &i);
      if (i <= 0)
        SIQ = i;
    }
    else if (strcmp(lCmd, "open") == 0)
    {
      scanf("%29s", lFileName);
      OpenFile(&lGame, lFileName);
      lUseBook = 0;
    }
    else if (strcmp(lCmd, "save") == 0)
    {
      scanf("%29s", lFileName);
      SaveFile(&lGame, lFileName);
    }
    
    /* Opponent move */
    
    else if (IsMoveStr(lCmd))
    {
      if (IsPossible(lCmd, &lGame, 1) && IsLegal(lCmd, &lGame, 1))
      {
        MakeMoveStr(lCmd, &lGame);
        printf("# Move accepted\n");
        if (lUseBook)
        {
          pt = lBook.p[lBook.aktual].poctah;
          i = 0;
          while (i < pt && strcmp(lCmd, lBook.p[lBook.aktual].tahy[i]) != 0)
            i++;
          if (i < pt)
            lBook.aktual = lBook.p[lBook.aktual].to[i];
          else
            lUseBook = 0;
        }
        PrintBoard(lGame);
      }
      else
        printf("# Illegal move: %s\n", lCmd);
    }
    else
      printf("# Unknown command: %s\n", lCmd);
  }
  while (1);
  
  fclose(f1);
  fclose(f2);
  
  return 0;
}

void Undo(TGame *aGame)
{
  int i, lCount;
  char lMove[5];

  lCount = aGame->fHalfMovesCount;
  InitGame(aGame);
  for (i = 1; i < lCount; i++)
  {
    SqrToStr(historie[i].from, &(lMove[0]));
    SqrToStr(historie[i].to, &(lMove[2]));
    MakeMoveStr(lMove, aGame);
  }
}
