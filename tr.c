#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

static void debug(const char *msg) {
	time_t t = time(NULL);
	fprintf(stderr, "%s%s\n\n", ctime(&t), msg);
}

static void handler(int signum)
{
	pthread_exit(NULL);
}

void *thread(void *arg)
{
	// static sigset_t mask;

	// sigemptyset(&mask);
	// sigaddset(&mask, SIGUSR1);

	// debug("Blocking SIGUSR1 in thread...");

	// if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
	// 	perror("pthread_sigmask");
	// 	exit(1);
	// }

	debug("Do something...");

	sleep(10);

	// debug("Unblocking SIGUSR1 in thread...");

	// if (pthread_sigmask(SIG_UNBLOCK, &mask, NULL) != 0) {
	// 	perror("pthread_sigmask");
	// 	exit(1);
	// }

	debug("Will not reach here...");

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid;

	signal(SIGUSR1, handler);

	if (pthread_create(&tid, NULL, thread, NULL) != 0) {
		perror("pthread_create");
		exit(1);
	}

	sleep(5);

	debug("Sending SIGUSR1 to thread...");

	pthread_kill(tid, SIGUSR1);

	pthread_join(tid, NULL);

	debug("Thread exited.");

	return 0;
}