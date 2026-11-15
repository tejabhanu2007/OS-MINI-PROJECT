#include "campuscompute.h"
#include <stdio.h>
#include <string.h>

int run_banker(const char *mode)
{
    enum { MAX_PROCESSES = 5, RESOURCES = 3 };

    int allocation[MAX_PROCESSES][RESOURCES] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int maximum[MAX_PROCESSES][RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    int available[RESOURCES] = {3, 3, 2};
    int need[MAX_PROCESSES][RESOURCES];
    int work[RESOURCES];
    int finished[MAX_PROCESSES] = {0};
    int sequence[MAX_PROCESSES];
    int process_count;
    int completed = 0;
    int safe_mode;

    if (mode == NULL ||
        (strcmp(mode, "safe") != 0 &&
         strcmp(mode, "unsafe") != 0))
    {
        fputs("Error: banker mode must be safe or unsafe.\n", stderr);
        return 2;
    }

    safe_mode = strcmp(mode, "safe") == 0;

    if (safe_mode)
    {
        process_count = 5;
    }
    else
    {
        process_count = 3;

        for (int j = 0; j < RESOURCES; j++)
        {
            available[j] = 0;
        }
    }

    for (int i = 0; i < process_count; i++)
    {
        for (int j = 0; j < RESOURCES; j++)
        {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    for (int j = 0; j < RESOURCES; j++)
    {
        work[j] = available[j];
    }

    while (completed < process_count)
    {
        int progress = 0;

        for (int i = 0; i < process_count; i++)
        {
            if (finished[i])
            {
                continue;
            }

            int can_finish = 1;

            for (int j = 0; j < RESOURCES; j++)
            {
                if (need[i][j] > work[j])
                {
                    can_finish = 0;
                    break;
                }
            }

            if (can_finish)
            {
                for (int j = 0; j < RESOURCES; j++)
                {
                    work[j] += allocation[i][j];
                }

                finished[i] = 1;
                sequence[completed++] = i;
                progress = 1;
            }
        }

        if (!progress)
        {
            break;
        }
    }

    printf("Banker's Algorithm - %s State\n",
           safe_mode ? "Safe" : "Unsafe");
    printf("Need matrix calculated from Maximum - Allocation.\n");

    if (completed == process_count)
    {
        printf("SAFE STATE.\n");
        printf("Safe sequence:");

        for (int i = 0; i < completed; i++)
        {
            printf(" P%d", sequence[i]);
        }

        printf("\n");
    }
    else
    {
        printf("UNSAFE STATE.\n");
        printf("No complete safe sequence; unfinished processes:");

        for (int i = 0; i < process_count; i++)
        {
            if (!finished[i])
            {
                printf(" P%d", i);
            }
        }

        printf(".\n");
    }

    return 0;
}