// #include "client_handlers.h"
// #include "arbitro_handlers.h"
#include "request_functions.h"
#include <stdio.h> 
#include <sys/stat.h> 
#include <sys/wait.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include "../general.h"

void *gameCommReadThread(void *arg);
void *gameCommWriteThread(void *arg);
void handleClientGameCommunication(int readPipe[2], int writePipe[2], Cliente *cliente, int *gameStarted);
void initJogo(Cliente* cliente, int *gameStarted);