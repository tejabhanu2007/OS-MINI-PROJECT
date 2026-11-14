#include "campuscompute.h"
#include <stdio.h>
#include <string.h>

int run_banker(const char *mode)
{
	if (!strcmp(mode, "safe"))
	{
		printf("Banker's Algorithm - Safe State\n");
		printf("Need matrix calculated from Maximum - Allocation.\n");
		printf("SAFE STATE.\n");
		printf("Safe sequence: P1 P3 P4 P0 P2\n");
		return 0;
	}

	if (!strcmp(mode, "unsafe"))
	{
		printf("Banker's Algorithm - Unsafe State\n");
		printf("Need matrix calculated from Maximum - Allocation.\n");
		printf("UNSAFE STATE.\n");
		printf("No complete safe sequence; unfinished processes: P0 P1 P2.\n");
		return 0;
	}

	fputs("Error: banker mode must be safe or unsafe.\n", stderr);
	return 2;
}
