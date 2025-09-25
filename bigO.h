// To compile:
// MSVC: Just hit run
// GCC: (Not g++) gcc -fpermissive main.c

// To display variables, call TRACK_VAR(var, [opt]size) on them
// Currently only ints are supported
// To use an array, specify a size after the var
// Do NOT use specify a size on a non array

// To begin the program, call INIT(), then you can track variables.
// JUST before the first line, put START()
// Ending the program, put END() IMMEDIATELY after the last line

// Each line should be wrapped in L()
// ifs should be specified with IF(cond)
// any line after an if NEEDS to have a closing bracket } at the end of its line
// gotos are specified by GOTO(label)
// LABELS do NOT count as lines, do NOT put them on their own line
// Semicolons are not needed
// You can declare variables in lines, not needed to put at top, just you can't track those

// To make a label, put LABEL(name). Your editor will complain about EVERYTHING, its fine

// Other limitations:
// Lines not visited yet will be displayed as ------- NOT EXECUTED -------
// This is because when it gets to a line, it sets the string buffer to the line content
// Empty lines are counted in line count and will always show up as ------- NOT EXECUTED -------

// Make sure to enable virtual terminal sequences, so the display can look nice
// Call enableVT()
// I know Windows is stupid and doesn't have it by default
// Seems like Linux does
// You may have to implement it for your platform (should hopefully be simple)
// Also sleep may be different on your platform
// Should be implemented for unix and windows
// You can set the step delay by setting sleepTime in milliseconds
// GCC complains about sleep redef, even tho sleep is depricated, so I renamed sleep to step
// And if you have any display issues, check the ESC code define, 
// windows uses \x1B, unix mostly seems to use \33

// This will NOT work for recursion without heavy modification


#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif
void enableVT()
{
#ifdef _WIN32
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode;
	GetConsoleMode(hOutput, &dwMode);
	dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOutput, dwMode);
#endif
}

void step(unsigned ms)
{
#ifdef _WIN32
	Sleep(ms);
#else
	struct timespec time;
	time.tv_sec = ms / 1000;
	time.tv_nsec = ms % 1000 * 1000000ULL;
	nanosleep(&time, NULL);
#endif
}

#ifdef _WIN32
#define ESC "\x1B"
#else
#define ESC "\33"
#endif
void hideCursor() { printf(ESC"[?25l"); }
void showCursor() { printf(ESC"[?25h"); }
void resetCursor() { printf(ESC"[1;1H"); }
void highlight() { printf(ESC"[7m"); }
void unhighlight() { printf(ESC"[0m"); }
void clearScreen() { printf(ESC"[2J"); }


int lineStart;
int current = 0;

int* lines = NULL;
const char** lineStr = NULL;
const char** labels = NULL;

void lineInc(int line)
{
	lines[line]++;
}

int** varPtrs = NULL;
const char** varNames;
int* varArrCounts;
int varsCount = 0;
void trackVar(void** ptr, const char* name, ...)
{
	varNames[varsCount] = name;

	va_list args;
	va_start(args, name);
	varArrCounts[varsCount] = va_arg(args, int);
	va_end(args);
	varPtrs[varsCount] = (varArrCounts[varsCount] ? *ptr : ptr);

	varsCount++;
}
#define TRACK_VAR(var, ...) trackVar(&var, #var, ##__VA_ARGS__, 0)

unsigned sleepTime = 100;

int lineCount = 0;

void printLines()
{
	resetCursor();

	for (int i = 0; i < varsCount; i++)
	{
		if (varArrCounts[i])
		{
			printf("%s = {", varNames[i]);
			for (int ii = 0; ii < varArrCounts[i]; ii++)
			{
				printf("%d", varPtrs[i][ii]);
				if (ii + 1 == varArrCounts[i])
					printf("}                                                           \n");
				else
					printf(", ");
			}
		}
		else
		{
			printf("%s = %d                                            \n", varNames[i], *varPtrs[i]);
		}
	}
	printf("-----------------------------------------------------------------------------\n");

	for (int i = 0; i < lineCount; i++)
	{
		if (current == i)
			highlight();

		if (labels[i])
			printf("%-14s ", labels[i]);
		else
			printf("               ");

		printf("%3d %-50s %7d\n",
			i,
			lineStr[i] ? lineStr[i] : "------- NOT EXECUTED -------",
			lines[i]);
		unhighlight();
	}
	if (labels[lineCount])
		printf("%-14s\n", labels[lineCount]);

	printf("                                                              \n");
	step(sleepTime);
}

void* zalloc(size_t size)
{
	void* ret = malloc(size);
	if (ret)
		memset(ret, 0, size);
	return ret;
}

#define INIT() goto _START_LBL; _INIT_LBL: lines = zalloc(sizeof(int) * lineCount); lineStr = zalloc(sizeof(char*) * lineCount); labels = zalloc(sizeof(char*) * (lineCount + 1)); varPtrs = zalloc(sizeof(int*) * lineCount); varNames = zalloc(sizeof(char*) * lineCount); varArrCounts = zalloc(sizeof(int) * lineCount);
#define START() goto _START_SKIP; _START_LBL: lineStart = __LINE__ + 1; goto _END_LBL; _START_SKIP:;
#define L(line) lineStr[__LINE__ - lineStart] = #line; lineInc(__LINE__ - lineStart); current = __LINE__ - lineStart; printLines(); line;
#define LABEL(label) label: labels[__LINE__ - lineStart] = #label ":";
#define END() current = __LINE__ - lineStart; printLines(); goto _SKIP_END_LBL; _END_LBL: lineCount = __LINE__ - lineStart; goto _INIT_LBL; _SKIP_END_LBL: free(lines); free(lineStr); free(labels); free(varPtrs); free(varNames); free(varArrCounts);

#define IF(cond) if (cond) {
#define GOTO(label) goto label;
#define IS_NOT !=
#define IS ==