#include <stdio.h>
#include "../utils.h"
#include "../general.h"

#ifndef CLIENT_HANDLERS_H
#define CLIENT_HANDLERS_H

#define MAX_PLAYER_ERR 2

int add_cliente(Arbitro *arbitro, PEDIDO *p);
int remove_cliente(Arbitro *arbitro, PEDIDO *p);
void printClientes(Arbitro *arbitro);
int validate_max_player_exceed(Arbitro *arbitro);
Jogo* getJogoByClienteName(Arbitro *arbitro, char *clienteName);

#endif // GENERAL_

