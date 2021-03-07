/*
 *  data.h
 *  Globalni datove typy a funkce
 */

#include <stdio.h>

#define VERSION "0.2"
#define REVISION "0.4"

#define WHITE 0
#define BLACK 1
#define NONE -1

#define MAXMOVHIS 200
#define MAXMOVGEN 160
#define BOOKDEPTH 120
#define BOOKWIDTH 8
#define BOOKFILE "sachy.bk"
#define CFGFILE "sachy.cfg"

/* default time controls: 120 minutes / 40 moves */
#define TMOVES 40
#define TSECONDS 120 * 60

#define DEPTH 1 /* hloubla prohledavani >=1 !!! */
#define SDEPTH 0 /* hloubka dohledavani do tichych pozic */
/* <= 0 */

/* pieces na sachovnici */
#define EMPT  0
#define WPWN  1
#define WBSP  2
#define WKNT  3
#define WROK  4
#define WQEN  5
#define WKNG  6
#define BPWN -1
#define BBSP -2
#define BKNT -3
#define BROK -4
#define BQEN -5
#define BKNG -6
/* HODNOTY FIGUR */
#define PWNV 10
#define BSPV 30
#define KNTV 30
#define ROKV 50
#define QENV 90
#define MATV 10000

typedef char TBoard[8][8]; /* board */

typedef struct /* POZICE */
{
  unsigned char x, y;
} TSquare;

typedef struct /* TAH */
{
  TSquare from, to;
  char promotion;
} TMove;

typedef struct /* FIGURA */
{
  char fig;
  TSquare pos;
} TPieceData;

typedef struct /* HRAC */
{
  TPieceData pieces[16];
  char rochada[3];  /* kterou jeste muze - malou, velkou */
  /* [2] -> zda byla nejaka provedena */
} TPlayer;

/* typedef TMove THistorie[MAXPOCTAH]; HISTORIE TAHU */

typedef struct /* HRA */
{
  TBoard board;
  TPlayer whiteplayer, blackplayer;
  char sidetomove; /* BILY / CERNY */
  char enpas; /* sloupec s pescem */
  int fHalfMovesCount; /* pocet hranych (pul)tahu */
  int tmoves, tseconds, tclockmode; /* level command values */
  /* tclockmode = 0 only supported */
  int tleft; /* time left to next time control */
} TGame;

typedef TMove TMoveList[MAXMOVGEN];

typedef struct /* zaznam v knihovne zahajeni */
{
  int poctah;
  char tahy[BOOKWIDTH][5]; /* jake tahy jsou mozne */
  int to[BOOKWIDTH]; /* to vedou prislusne tahy (node) */
} TBookPos;

typedef struct /* knihovna zahajeni */
{
  TBookPos p[BOOKDEPTH];
  int last, aktual; /* last -> posledni prvek */
} TBook;

/* FUNKCE */

/* interface.c */
int ReadConfig(void); /* nacte konfiguraci */
void SaveFile(TGame *, char *); /* ulozi hru do souboru */
void OpenFile(TGame *, char *); /* InitGame + nacteni hry ze souboru */
void PrintBoard(TGame); /* vytiskne sachovnici */
void PrintHist(TGame); /* vytiskne historii tahu */
void PrintMoves(TMoveList, int); /* vytiskne vsechny mozne tahy */
void PrintPieces(TGame); /* vytiskne pozice vsech figur */
void SqrToStr(TSquare, char *); /* Pozice -> e4 atd */
void StrToSqr(char *, TSquare *); /* e4 -> Pozice ... */
void PrmToStr(char, char *); /* Promotion to string */
int StrToPrm(char *); /* String to promotion */

/* rules.c */
int IsMoveStr(char *); /* zda to je e1e3,d4d5,b1c3 ... proste tah */
int IsPossible(char *, TGame *);
int IsLegal(char *, TGame *, int); /* vrati, zda je tah("aXfY") podle pravidel */
int PawnCanMove(TMove, TGame *); /* muze takto pesec tahnout? */
int EmptyPath(TMove, TBoard); /* ma figura volnou cestu? */
int PieceCanMove(TMove, TGame *); /* zjisti, zda dana figurka takto tahne */
int InCheck(int, TGame *); /* zda je hrac v sachu */
int WhitePiece(int); /* zda figurka F je bila */
int BlackPiece(int);
int FindPiece(TSquare, TBoard, TPlayer *, int *); /* dostane pozici na
                 sachovnici a vrati prislusnou figuru, ktera se tam
                 naleza */
void RemovePiece(TPlayer *); /* odeber EMPT z figurek a shiftne je */
int IsMate(TGame *); /* vrati, zda je hrac na tahu v matove pozici */
int IsStaleMate(TGame *); /* vrati, zda je hrac na tahu v patove pozici */
int SMove(TGame *, TMove); /* zda je tah pro dohl. do tichych pozic */
/* zatim jenom braci tahy */

/* MakeMoveStr.c */
void MakeMove(TMove, TGame *); /* defakto provede tah ... */
void MakeMoveStr(char *, TGame *); /* defakto provede tah ... */
//int BestMove(TGame *, char *, int); /* najde a provede nejlepsi tah */
int BestMove2(TGame *, char *, int); /* najde a provede nejlepsi tah */

/* movegen.c */
int MoveGen(TGame *, TMoveList, int); /* naplni pole TMoveList a vrati jejich pocet ... */
/* pokud je INT 1, tak nas pouze zajima, zda existuje tah, ci ne
   - pri testovani matove pozice (obvykle mat neni) */
int SMoveGen(TGame *, TMoveList); /* pouze tahy pro dohledavani do
           tichych pozic */

/* search.c */
int AlphaBeta(TGame *, int, int, int);
/* ... proste alfabeta => value */
/* posledni parametr 0..normal 1..dohl. tichych pozic */

/* inic.c */
void InitGame(TGame *); /* nastavi prvotni pozici */
void InitGameFromFen(TGame *, char *, char *, char *, char *, char *, char *);

/* value.c */
int Evaluate(TGame *); /* vrati hodnotu pozice ... */
int EvaluateM(TGame *); /* ohodnoti pouze material */

/* test.c */
int Benchmark(TGame *); /* pocet testu za sekundu (orientacni) */
int Benchmark2(TGame *); /* pocet testu za sekundu (orientacni) */

/* sort.c */
void Sort(TMoveList, int[], int);

/* book.c */
int OpenBook(TBook *); /* nacte soubor BOOKFILE do TBook */
void PrintBook(TBook *); /* vytiskne TBook na obrazovku */

/* Log files */

extern FILE *f1;
extern FILE *f2;

void ToLog(const char *aText, const int aSecondFile);
