#include "campuscompute.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int run_worker(const char *mode)
{
	pid_t p;
	int s;

	if (strcmp(mode, "normal") && strcmp(mode, "fail"))
	{
		fputs("Error: worker mode must be normal or fail.\n", stderr);
		return 2;
	}

	p = fork();

	if (p < 0)
	{
		perror("fork");
		return 1;
	}

	if (!p)
	{
		_exit(!strcmp(mode, "fail") ? 7 : 0);
	}

	waitpid(p, &s, 0);

	if (!strcmp(mode, "normal"))
	{
		printf("Parent created worker PID=%ld.\n", (long)p);
		printf("waitpid detected normal worker completion.\n");
	}
	else
	{
		printf("Parent created worker PID=%ld.\n", (long)p);
		printf("waitpid detected controlled simulated failure: exit status 7.\n");
	}

	return 0;
}
