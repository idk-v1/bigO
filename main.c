#include "bigO.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

void bubblesort(int x[], int n)
{
	int i = 0, ii = 0, temp = 0;
	INIT();
	TRACK_VAR(n);
	TRACK_VAR(i);
	TRACK_VAR(ii);
	TRACK_VAR(temp);
	TRACK_VAR(x, n);

						START();
L(						i = 0							);
LABEL(OuterLoop) L(		IF (i >= n)						);
L(						IF_GOTO(OuterLoopEnd)			);
L(						ii = 0							);
LABEL(InnerLoop) L(		IF (ii >= n - 1 - i)			);
L(						IF_GOTO(InnerLoopEnd)			);
L(						IF (x[ii] <= x[ii + 1])			);
L(						IF_GOTO(EndIf)					);
L(						temp = x[ii + 1]				);
L(						x[ii + 1] = x[ii]				);
L(						x[ii] = temp					);
LABEL(EndIf) L(			ii = ii + 1						);
L(						GOTO(InnerLoop)					);
LABEL(InnerLoopEnd)	L(	i = i + 1						);
L(						GOTO(OuterLoop)					);
LABEL(OuterLoopEnd)		END();
}

void selectionsort(int x[], int n)
{
	int smallest = 0, index = 0, temp = 0, i = 0, ii = 0;
	INIT();
	TRACK_VAR(n);
	TRACK_VAR(smallest);
	TRACK_VAR(index);
	TRACK_VAR(temp);
	TRACK_VAR(i);
	TRACK_VAR(ii);
	TRACK_VAR(x, n);

						START();
L(						i = 0							);
LABEL(OuterLoop) L(		IF (i >= n - 1)					);
L(						IF_GOTO(EndOuterLoop)			);
L(						smallest = INT_MAX				);
L(						ii = i							);
LABEL(InnerLoop) L(		IF(ii >= n)						);
L(						IF_GOTO(EndInnerLoop)			);
L(						IF (x[ii] >= smallest)			);
L(						IF_GOTO(EndIf)					);
L(						smallest = x[ii]				);
L(						index = ii						);
LABEL(EndIf) L(			ii = ii + 1						);
L(						GOTO(InnerLoop)					);
LABEL(EndInnerLoop)	L(	temp = x[i]						);
L(						x[i] = x[index]					);
L(						x[index] = temp					);
L(						i = i + 1						);
L(						GOTO(OuterLoop)					);
LABEL(EndOuterLoop)		END();
}

int main(int argc, char** argv)
{
	enableVT();
	hideCursor();
	clearScreen();
	srand(time(NULL));

	int n = 30;
	int* x = zalloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		x[i] = rand() % 20;
	sleepTime = 100;
	bubblesort(x, n);
	//selectionsort(x, n);

	free(x);

	showCursor();
	return 0;
}