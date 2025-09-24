// Very hacky, you NEED to set line count manually BEFORE INCLUDING "bigO.h"
// Empty lines are counted in line count and will always show up as ------- NOT EXECUTED -------

// To compile:
// MSVC: Just hit run
// GCC: (Not g++) gcc -fpermissive main.c

// To display variables, call TRACK_VAR(var, [opt]size) on them
// Currently only ints are supported
// To use an array, specify a size after the var
// Do NOT use specify a size on a non array

// To begin the program, before the first line, put START
// Ending the program, put END. Not necessary, just cleans up the highlight state

// Each line should be wrapped in L()
// ifs should be specified with IF(cond), IF_GOTO needs to come immediately after
// gotos after an if NEED to be IF_GOTO(label) because of bracket hacks
// normal gotos are specified by GOTO(label)
// LABELS do NOT count as lines, do NOT put them on their own line
// Semicolons are not needed, but shouldnt hurt
// You can declare variables in lines, not needed to put at top, just you can't track those

// To make a label, put LABEL(name). Your editor will complain about EVERYTHING, its fine

// Other limitations:
// Lines not visited yet will be displayed as ------- NOT EXECUTED -------
// This is because when it gets to a line, it sets the string buffer to the line content
// REMEMBER TO SET LINE COUNT MANUALLY BEFORE THE INCLUDE

// You can set the step delay by setting sleepTime in milliseconds
// GCC complains about sleep redef, even tho sleep is depricated, so I renamed sleep to step

// Make sure to enable virtual terminal sequences, so the display can look nice
// Call enableVT()
// I know Windows is stupid and doesn't have it by default
// You may have to implement it for your platform (should hopefully be simple)
// Also sleep may be different on your platform
// And if you have any display issues, check the ESC code define, 
// windows uses \x1B, unix mostly seems to use \33

#define LINECOUNT 18
#include "bigO.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void bubblesort(int x[], int n)
{
	int i = 0, ii = 0, temp = 0;
	TRACK_VAR(i);
	TRACK_VAR(ii);
	TRACK_VAR(temp);
	TRACK_VAR(x, n);

						START
						L(i = 0)
						L(ii = 0)
						L(temp = 0)
LABEL(OuterLoop)		L(IF (i >= n))
						L(IF_GOTO(OuterLoopEnd))
						L(ii = 0)
LABEL(InnerLoop)		L(IF (ii >= n - 1 - i))
						L(IF_GOTO(InnerLoopEnd))
						L(IF (x[ii] <= x[ii + 1]))
						L(IF_GOTO(EndIf))
						L(temp = x[ii + 1])
						L(x[ii + 1] = x[ii])
						L(x[ii] = temp)
LABEL(EndIf)			L(ii = ii + 1)
						L(GOTO(InnerLoop))
LABEL(InnerLoopEnd)		L(i = i + 1)
						L(GOTO(OuterLoop))
LABEL(OuterLoopEnd)		END
}

void selectionsort(int x[], int n)
{
	int smallest = 0, index = 0, temp = 0, i = 0, ii = 0;

	TRACK_VAR(n);
	TRACK_VAR(smallest);
	TRACK_VAR(index);
	TRACK_VAR(temp);
	TRACK_VAR(i);
	TRACK_VAR(ii);
	TRACK_VAR(x, n);

					START
					L(i = 0)
LABEL(OuterLoop)	L(IF (i >= n - 1))
					L(IF_GOTO(EndOuterLoop))
					L(smallest = INT_MAX)
					L(ii = i)
LABEL(InnerLoop)	L(IF (ii >= n))
					L(IF_GOTO(EndInnerLoop))
					L(IF (x[ii] >= smallest))
					L(IF_GOTO(EndIf))
					L(smallest = x[ii])
					L(index = ii)
LABEL(EndIf)		L(ii = ii + 1)
					L(GOTO(InnerLoop))
LABEL(EndInnerLoop)	L(temp = x[i])
					L(x[i] = x[index])
					L(x[index] = temp)
					L(i = i + 1)
					L(GOTO(OuterLoop))
LABEL(EndOuterLoop) END

	//for (i = 0; i < n - 1; i++)
	//{
	//	smallest = INT_MAX;
	//	for (ii = i; ii < n; ii++)
	//	{
	//		if (x[ii] < smallest)
	//		{
	//			smallest = x[ii];
	//			index = ii;
	//		}
	//	}

	//	temp = x[i];
	//	x[i] = x[index];
	//	x[index] = temp;
	//}
}

int main(int argc, char** argv)
{
	enableVT();
	clearScreen();

	int x[] = { 57,37,8,7,6,};
	int n = sizeof(x) / sizeof(int);
	sleepTime = 200;
	//bubblesort(x, n);
	selectionsort(x, n);

	showCursor();
	return 0;
}