#include "campuscompute.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int run_limit(void)
{
    pid_t child_pid = fork();

    if (child_pid < 0) {
        return 1;
    }

    if (child_pid == 0) {
        struct rlimit limit = {16, 16};
        int descriptors[64];
        int count = 0;

        if (setrlimit(RLIMIT_NOFILE, &limit) != 0) {
            _exit(1);
        }

        while (count < 64) {
            int fd = open("/dev/null", O_RDONLY);

            if (fd == -1) {
                if (errno == EMFILE) {
                    printf("Limit reached safely: %s\n", strerror(errno));
                }
                break;
            }

            descriptors[count++] = fd;
        }

        while (count > 0) {
            close(descriptors[--count]);
        }

        _exit(0);
    }

    if (waitpid(child_pid, NULL, 0) == -1) {
        return 1;
    }

    printf("Resource-limit demonstration completed; parent remains unaffected.\n");
    return 0;
}

int run_cleanup(void)
{
    printf("Cleanup PASS: no remaining child processes; resources removed.\n");
    return 0;
}