#include "campuscompute.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

int parse_int(const char *text, int low, int high, int *value)
{
	char *end;
	long n;

	errno = 0;
	n = strtol(text, &end, 10);

	if (errno || end == text || *end || n < low || n > high)
	{
		return -1;
	}

	*value = (int)n;
	return 0;
}

int load_jobs(const char *path, Job jobs[], int *count)
{
	FILE *file = fopen(path, "r");
	char line[256];
	int n = 0;

	if (!file)
	{
		perror(path);
		return 1;
	}

	while (fgets(line, sizeof line, file))
	{
		char *p = line;
		int values[4];

		while (isspace((unsigned char)*p))
		{
			p++;
		}

		if (!*p || *p == '#')
		{
			continue;
		}

		if (sscanf(p, "%d%*[, ]%d%*[, ]%d%*[, ]%d", &values[0], &values[1], &values[2], &values[3]) != 4 || n == MAX_JOBS || values[0] < 1 || values[1] < 0 || values[2] < 1 || values[3] < 0)
		{
			fprintf(stderr, "Error: invalid job data in %s.\n", path);
			fclose(file);
			return 1;
		}

		jobs[n] = (Job){values[0], values[1], values[2], values[3], values[2], 0, -1};
		n++;
	}

	fclose(file);

	if (!n)
	{
		fprintf(stderr, "Error: no jobs found in %s.\n", path);
		return 1;
	}

	*count = n;
	return 0;
}

static int earlier(const Job *a, const Job *b)
{
	return a->arrival < b->arrival || (a->arrival == b->arrival && a->id < b->id);
}

static void sort_jobs(Job j[], int n)
{
	int i;
	int k;

	for (i = 1; i < n; i++)
	{
		for (k = i; k > 0 && earlier(&j[k], &j[k - 1]); k--)
		{
			Job t = j[k];

			j[k] = j[k - 1];
			j[k - 1] = t;
		}
	}
}

static void seg(int a, int b, int id)
{
	printf(" [%d-%d: J%d]", a, b, id);
}

static void print_heading(const char *label)
{
	printf("Job statuses: NEW -> READY -> RUNNING -> COMPLETED (simulated CPU bursts)\n\n=== %s ===\n",
		label);
	printf("Tie breaking: arrival then job ID; smaller priority value = higher priority.\n");
	printf("Timeline:\n");
}

static void show_results(Job j[], int n)
{
	int i;
	double waiting = 0;
	double turnaround = 0;

	printf("\n Job  Arrival Burst Priority Completion Turnaround Waiting Response\n");

	for (i = 0; i < n; i++)
	{
		int tat = j[i].completion - j[i].arrival;
		int wait = tat - j[i].burst;

		waiting += wait;
		turnaround += tat;

		printf("%4d%8d%6d%9d%11d%11d%8d%9d\n",
			j[i].id, j[i].arrival, j[i].burst, j[i].priority,
			j[i].completion, tat, wait, j[i].first - j[i].arrival);
	}

	printf("Average waiting=%.2f | average turnaround=%.2f\n", waiting / n, turnaround / n);
}

/* FCFS: the sorted input order is the execution order. */
static void run_fcfs(Job j[], int n)
{
	int now = 0;
	int i;

	print_heading("FCFS");

	for (i = 0; i < n; i++)
	{
		if (now < j[i].arrival)
		{
			now = j[i].arrival;
		}

		j[i].first = now;
		seg(now, now + j[i].remaining, j[i].id);

		now += j[i].remaining;
		j[i].remaining = 0;
		j[i].completion = now;
	}

	show_results(j, n);
}

/* Round Robin: completed time slices are requeued after new arrivals. */
static void run_round_robin(Job j[], int n, int quantum)
{
	int now = 0;
	int done = 0;
	int q[MAX_JOBS];
	int head = 0;
	int tail = 0;
	int qty = 0;
	int next = 0;
	char label[64];

	snprintf(label, sizeof label, "Round Robin | quantum=%d", quantum);
	print_heading(label);

	while (done < n)
	{
		int k;
		int slice;

		if (!qty && now < j[next].arrival)
		{
			now = j[next].arrival;
		}

		while (next < n && j[next].arrival <= now)
		{
			q[tail++ % MAX_JOBS] = next++;
			qty++;
		}

		k = q[head++ % MAX_JOBS];
		qty--;

		if (j[k].first < 0)
		{
			j[k].first = now;
		}

		slice = j[k].remaining < quantum ? j[k].remaining : quantum;
		seg(now, now + slice, j[k].id);
		now += slice;
		j[k].remaining -= slice;

		while (next < n && j[next].arrival <= now)
		{
			q[tail++ % MAX_JOBS] = next++;
			qty++;
		}

		if (j[k].remaining)
		{
			q[tail++ % MAX_JOBS] = k;
			qty++;
		}
		else
		{
			j[k].completion = now;
			done++;
		}
	}

	show_results(j, n);
}

/* Non-preemptive Priority: smaller numeric priority is selected first. */
static void run_priority(Job j[], int n)
{
	int now = 0;
	int done = 0;
	int i;

	print_heading("Priority (Non-Preemptive)");

	while (done < n)
	{
		int k = -1;

		for (i = 0; i < n; i++)
		{
			if (j[i].remaining && j[i].arrival <= now && (k < 0 || j[i].priority < j[k].priority || (j[i].priority == j[k].priority && earlier(&j[i], &j[k]))))
			{
				k = i;
			}
		}

		if (k < 0)
		{
			int next = 1000000000;

			for (i = 0; i < n; i++)
			{
				if (j[i].remaining && j[i].arrival < next)
				{
					next = j[i].arrival;
				}
			}

			now = next;
			continue;
		}

		j[k].first = now;
		seg(now, now + j[k].remaining, j[k].id);

		now += j[k].remaining;
		j[k].remaining = 0;
		j[k].completion = now;
		done++;
	}

	show_results(j, n);
}

/* Public function retained so the existing CLI commands do not change. */
int run_schedule(const char *policy, int quantum, const char *path)
{
	Job j[MAX_JOBS];
	int n;

	if (load_jobs(path, j, &n))
	{
		return 1;
	}

	sort_jobs(j, n);

	if (!strcmp(policy, "fcfs"))
	{
		run_fcfs(j, n);
	}
	else if (!strcmp(policy, "rr"))
	{
		run_round_robin(j, n, quantum);
	}
	else if (!strcmp(policy, "priority"))
	{
		run_priority(j, n);
	}
	else
	{
		fprintf(stderr, "Error: unknown scheduling policy.\n");
		return 1;
	}

	return 0;
}

int run_compare(const char *path)
{
	int rc = run_schedule("fcfs", 0, path);

	if (!rc)
	{
		rc = run_schedule("rr", 2, path);
	}

	if (!rc)
	{
		rc = run_schedule("priority", 0, path);
	}

	return rc;
}
