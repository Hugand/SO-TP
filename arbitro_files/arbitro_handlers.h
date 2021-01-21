#include <stdio.h>
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

#endif
