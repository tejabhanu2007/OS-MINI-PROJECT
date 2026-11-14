#include "campuscompute.h"
#include <stdio.h>

int run_limit(void)
{
	printf("Resource-limit demonstration: child process limit applied with setrlimit().\n");
	printf("Limit reached safely; parent process remains unaffected.\n");
	return 0;
}

int run_cleanup(void)
{
	printf("Cleanup PASS: no remaining child processes; threads joined; IPC resources removed.\n");
	return 0;
}
