#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h> 
#include <unistd.h>
#include <pthread.h>
#include "../utils.h"
#include "client_handlers.h"
#include "../general.h"

#ifndef ARBITRO_HANDLERS_H
#define ARBITRO_HANDLERS_H

void commandArbitroPlayers(Arbitro *arbitro);
void commandArbitroGames(Arbitro *arbitro);
void commandArbitroK(Arbitro *arbitro, char* adminCommand);
void commandArbitroExit(Arbitro *arbitro);
void commandArbitroS(Arbitro *arbitro, char* adminCommand, PEDIDO *p);
void commandArbitroConSuspensa(Arbitro *arbitro, char* adminCommand, PEDIDO *p, int isConSuspended);
void stopGames(Arbitro *arbitro, int *gameStarted);

#endif
