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
void hideCursor() {	printf("\x1B[?25l"); }
void showCursor() {	printf("\x1B[?25h"); }
void resetCursor() { printf("\x1B[1;1H"); }
void highlight() { printf("\x1B[7m"); }
void unhighlight() { printf("\x1B[0m"); }
#else
void hideCursor() { printf("\33[?25l"); }
void showCursor() { printf("\33[?25h"); }
void resetCursor() { printf("\33[1;1H"); }
void highlight() { printf("\33[7m"); }
void unhighlight() { printf("\33[0m"); }
#endif


int lineStart;
int current = 0;
int N;
#define LABELCOUNT LINECOUNT + 1
#define VARCOUNT 100
#define START(n_) N = n_; lineStart = __LINE__ + 1;

int lines[LINECOUNT] = { 0 };
const char* lineStr[LINECOUNT];
const char* labels[LABELCOUNT] = { "" };

void lineInc(int line)
{
	lines[line]++;
}

int* varPtrs[VARCOUNT] = { 0 };
const char* varNames[VARCOUNT] = { 0 };
int varArrCounts[VARCOUNT] = { 0 };
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

void printLines()
{
	hideCursor();
	resetCursor();
	printf("n=%d                                                                    \n", N);

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
	printf("-------------------------------------------------------------------------\n");

	for (int i = 0; i < LINECOUNT; i++)
	{
		if (labels[i])
			printf("%-14s ", labels[i]);
		else
			printf("               ");

		if (current == i)
			highlight();
		printf("%3d %-50s %3d\n",
			i,
			lineStr[i] ? lineStr[i] : "------- NOT EXECUTED -------",
			lines[i]);
		unhighlight();
	}
	if (labels[LINECOUNT])
		printf("%-14s\n", labels[LINECOUNT]);

	printf("                                                              \n");
	step(sleepTime);
}

#define L(line) lineStr[__LINE__ - lineStart] = #line; lineInc(__LINE__ - lineStart); current = __LINE__ - lineStart; printLines(); line;
#define LABEL(label) label: labels[__LINE__ - lineStart] = #label ":";
#define END current = __LINE__ - lineStart; printLines();

#define IF(cond) if (cond) {
#define IF_GOTO(label) goto label;}
#define GOTO(label) goto label;
#define IS_NOT !=
#define IS ==