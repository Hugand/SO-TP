#include <stdio.h>
#include "../utils.h"
#include "../general.h"
#include "request_functions.h"


#ifndef CLIENT_HANDLERS_H
#define CLIENT_HANDLERS_H

#define MAX_PLAYER_ERR 2

int add_cliente(Arbitro *arbitro, PEDIDO *p);
int remove_cliente(Arbitro *arbitro, char *clienteName);
void printClientes(Arbitro *arbitro);
int validate_max_player_exceed(Arbitro *arbitro);
int validate_client_connected(Arbitro *arbitro, pid_t pid);
Jogo* getJogoByClienteName(Arbitro *arbitro, char *clienteName);


void commandClientQuit(Arbitro *arbitro, PEDIDO *p);
void commandClientMyGame(Arbitro *arbitro, PEDIDO *p, char *fifo, int n);

#endif // GENERAL_

