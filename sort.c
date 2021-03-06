/*
 *  sort.c
 *  Setridi tahy podle jejich hodnot
 */
 
#include "data.h"

void Sort(TMoveList aList, int h[], int aCount)
{
  int first, max, i, t;
  TMove lMove;
  for (first = 0; first < aCount - 1; first++)
  {
    max = first;
    for (i = first + 1; i < aCount; i++)
      if (h[i] > h[max])
        max = i;
    if (max != first)
    {
      lMove = aList[first];
      aList[first] = aList[max];
      aList[max] = lMove;
      t = h[first];
      h[first] = h[max];
      h[max] = t;
    }
  }
}
