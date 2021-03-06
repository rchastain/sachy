/*
 *  book.c
 *  Knihovna zahajeni apod
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "data.h"

int OpenBook(TBook *book)
{
  FILE *f;
  char t[5];
  int i;

  f = fopen(BOOKFILE, "rt");
  if (f == NULL)
  {
    printf("# Warning: error reading opening book %s: ", BOOKFILE);
    perror(NULL);
    return 0;
  }
  else
  {
    book->last = 0;
    book->aktual = 0;
    while (!feof(f) && book->last < BOOKDEPTH)
    {
      book->p[book->last].poctah = 0;
      fscanf(f, "%4s", t);
      if (strcmp(t, "#") == 0)
      {
        book->aktual = 0;
      }
      else if (book->aktual == book->last)
      {
        /* pripsani noveho tahu do LAST (a LAST++) */
        strcpy(book->p[book->aktual].tahy[0], t);
        book->p[book->aktual].poctah = 1;
        book->last++;
        book->p[book->aktual].to[0] = book->last;
        book->aktual = book->last;
      }
      else
      {
        /* podivat se, zda
           to tam uz neni, jinak pripsat */
        i = 0;
        while (i < book->p[book->aktual].poctah && strcmp(t, book->p[book->aktual].tahy[i]) != 0)
          i++;
        if (i < book->p[book->aktual].poctah)
          /* preskocit dal (je tam) */
          book->aktual = book->p[book->aktual].to[i];
        else if (i < BOOKWIDTH)
        {
          /* pripsat (neni tam) */
          strcpy(book->p[book->aktual].tahy[i], t);
          book->p[book->aktual].poctah++;
          book->last++;
          book->p[book->aktual].to[i] = book->last;
          book->aktual = book->last;
        }
      }
    }
  }
  fclose(f);
  return 1;
}

void PrintBook(TBook * book)
{
  int i, j;
  for (i = 0; i < book->last; i++)
    for (j = 0; j < book->p[i].poctah; j++)
      printf(" (%d) %s -> %d\n", i, book->p[i].tahy[j], book->p[i].to[j]);
}
