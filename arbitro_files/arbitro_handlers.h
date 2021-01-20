#include <stdio.h>
#include "../utils.h"
#include "client_handlers.h"
#include "../general.h"

#ifndef ARBITRO_HANDLERS_H
#define ARBITRO_HANDLERS_H



void commandArbitroPlayers(Arbitro *arbitro);
void commandArbitroGames(Arbitro *arbitro);
void commandArbitroK(Arbitro *arbitro, char* adminCommand, char* word);
void commandArbitroExit(Arbitro *arbitro);


#endif
