CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
LDFLAGS = -pthread
SOURCES = campuscompute.c scheduling.c logger.c ipc.c banker.c worker.c resource_limit.c
OBJECTS = $(SOURCES:.c=.o)

all: campuscompute

campuscompute: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c campuscompute.h
	$(CC) $(CFLAGS) -c $< -o $@

test: campuscompute
	./tests/regression.sh

clean:
	rm -f $(OBJECTS) campuscompute campuscompute.log

.PHONY: all test clean
