// Very hacky, you NEED to set line count manually
// Empty lines are counted in line count and will always show up as ------- NOT EXECUTED -------
// LABELS do NOT count as lines, do NOT put them on their own line

// To display variables, call TRACK_VAR(var, [opt]size) on them
// Currently only ints are supported
// To use an array, specify a size after the var
// Do NOT use specify a size on a non array

// Call START(n) with n being array size. IDK why I did this, its kinda an auto TRACK_VAR(n)

// Each line should be wrapped in L()
// ifs should be specified with IF(cond)
// gotos after an if NEED to be IF_GOTO(label) because of bracket hacks
// normal gotos are specified by GOTO(label)

// Other limitations:
// Lines not visited yet will be displayed as ------- NOT EXECUTED -------
// REMEMBER TO SET LINE COUNT MANUALLY

// You can set the step delay by setting sleepTime in milliseconds

#define LINECOUNT 180
#include "bigO.h"

void bubblesort(int x[], int n)
{
	int i = 0, ii = 0, temp = 0;
	TRACK_VAR(i);
	TRACK_VAR(ii);
	TRACK_VAR(temp);
	TRACK_VAR(x, n);

						START(n);
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

	TRACK_VAR(smallest);
	TRACK_VAR(index);
	TRACK_VAR(temp);
	TRACK_VAR(i);
	TRACK_VAR(ii);
	TRACK_VAR(x, n);

					START(n)
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

	int x[] = { 57,37,8,7,6,};
	int n = sizeof(x) / sizeof(int);
	sleepTime = 200;
	//bubblesort(x, n);
	selectionsort(x, n);

	return 0;
}