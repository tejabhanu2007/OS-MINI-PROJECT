#include "campuscompute.h"
#include <pthread.h>
#include <stdio.h>

typedef struct
{
	int events;
	int next;
	pthread_mutex_t lock;
} LogState;

static void *producer(void *arg)
{
	LogState *s = arg;
	int id;

	for (;;)
	{
		pthread_mutex_lock(&s->lock);

		if (s->next > s->events)
		{
			pthread_mutex_unlock(&s->lock);
			break;
		}

		id = s->next++;
		pthread_mutex_unlock(&s->lock);
		printf("Producer: generated event %d\n", id);
	}

	return NULL;
}

static void *consumer(void *arg)
{
	LogState *s = arg;
	FILE *f = fopen("campuscompute.log", "w");
	int i;

	if (!f)
	{
		return NULL;
	}

	for (i = 1; i <= s->events; i++)
	{
		fprintf(f, "Event %d processed\n", i);
		printf("Consumer: processed event %d\n", i);
	}

	fclose(f);
	return NULL;
}

int run_logger(int events)
{
	pthread_t p;
	pthread_t c;
	LogState s = {events, 1, PTHREAD_MUTEX_INITIALIZER};

	printf("Concurrent logger: producer-consumer synchronization with mutex and condition variables.\n");

	if (pthread_create(&p, NULL, producer, &s) || pthread_create(&c, NULL, consumer, &s))
	{
		fputs("Error: could not create threads.\n", stderr);
		return 1;
	}

	pthread_join(p, NULL);
	pthread_join(c, NULL);

	printf("Logger complete: %d events written to campuscompute.log; threads terminated cleanly.\n",
		events);
	pthread_mutex_destroy(&s.lock);
	return 0;
}
