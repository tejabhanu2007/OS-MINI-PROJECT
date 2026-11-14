/* Compatibility entry point retained from the original single-file project.
 * Feature implementations now live in the focused modules listed in Makefile. */
#include "campuscompute.h"
#include <stdio.h>
#include <string.h>

void print_help(void)
{
	printf("CampusCompute - Intelligent Job Scheduling and Resource Management\n");
	printf("Usage:\n");
	printf("  ./campuscompute help\n");
	printf("  ./campuscompute fcfs jobs.csv\n");
	printf("  ./campuscompute rr QUANTUM jobs.csv\n");
	printf("  ./campuscompute priority jobs.csv\n");
	printf("  ./campuscompute compare jobs.csv\n");
	printf("  ./campuscompute logger EVENTS\n");
	printf("  ./campuscompute ipc\n");
	printf("  ./campuscompute banker safe|unsafe\n");
	printf("  ./campuscompute worker normal|fail\n");
	printf("  ./campuscompute limit\n");
	printf("  ./campuscompute cleanup\n");
	printf("  ./campuscompute all\n");
}

int main(int argc, char **argv)
{
	int value;

	if (argc == 2 && !strcmp(argv[1], "help"))
	{
		print_help();
		return 0;
	}

	if (argc == 3 && !strcmp(argv[1], "fcfs"))
	{
		return run_schedule("fcfs", 0, argv[2]);
	}

	if (argc == 4 && !strcmp(argv[1], "rr") && !parse_int(argv[2], 1, 100000, &value))
	{
		return run_schedule("rr", value, argv[3]);
	}

	if (argc == 3 && !strcmp(argv[1], "priority"))
	{
		return run_schedule("priority", 0, argv[2]);
	}

	if (argc == 3 && !strcmp(argv[1], "compare"))
	{
		return run_compare(argv[2]);
	}

	if (argc == 3 && !strcmp(argv[1], "logger") && !parse_int(argv[2], 1, 1000, &value))
	{
		return run_logger(value);
	}

	if (argc == 2 && !strcmp(argv[1], "ipc"))
	{
		return run_ipc();
	}

	if (argc == 3 && !strcmp(argv[1], "banker"))
	{
		return run_banker(argv[2]);
	}

	if (argc == 3 && !strcmp(argv[1], "worker"))
	{
		return run_worker(argv[2]);
	}

	if (argc == 2 && !strcmp(argv[1], "limit"))
	{
		return run_limit();
	}

	if (argc == 2 && !strcmp(argv[1], "cleanup"))
	{
		return run_cleanup();
	}

	if (argc == 2 && !strcmp(argv[1], "all"))
	{
		int failed = 0;

		failed |= run_schedule("fcfs", 0, "jobs.csv");
		failed |= run_schedule("rr", 2, "jobs.csv");
		failed |= run_schedule("priority", 0, "jobs.csv");
		failed |= run_logger(4);
		failed |= run_ipc();
		failed |= run_banker("safe");
		failed |= run_banker("unsafe");
		failed |= run_worker("normal");
		failed |= run_worker("fail");
		failed |= run_limit();
		failed |= run_cleanup();

		printf("Integrated summary: %s\n", failed ? "FAIL" : "PASS");
		return failed;
	}

	fprintf(stderr, "Error: invalid command or arguments.\n");
	print_help();
	return 2;
}
