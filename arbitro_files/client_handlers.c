#include <stdlib.h>

#include <string.h>
#include "client_handlers.h"

int add_cliente(Arbitro *arbitro, PEDIDO *p) {
    // Check if the MAXPLAYERS limit is
    if(validate_connection(arbitro) == FALSE) return MAX_PLAYER_ERR;
    int newClientIndex;

    // Attributed game to be used only for demo of the #mygame command
    // To be removed in meta 3
    Jogo *mockGame = malloc(sizeof(Jogo));
    strcpy(mockGame->nome, "Mock game!");
    
    Cliente newCliente;
    Cliente* tmpClientes;

    strcpy(newCliente.jogador.nome, p->nome);
    newCliente.jogador.pontuacao = 0;
    newCliente.jogo = mockGame;
    newClientIndex = arbitro->nClientes;

    tmpClientes = realloc(arbitro->clientes, (arbitro->nClientes+1)*sizeof(Cliente));
    if(tmpClientes == NULL) return FALSE;

    arbitro->clientes = tmpClientes;
    arbitro->clientes[newClientIndex] = newCliente;
    arbitro->nClientes++;

    return TRUE;
}

int remove_cliente(Arbitro *arbitro, PEDIDO *p) {
    int i;
    Cliente *tmpClientes;

    // If it is not removing the last remaining client
    if(arbitro->nClientes > 1)
        for(i = 0; i < arbitro->nClientes; i++) {
            if(strcmp(arbitro->clientes[i].jogador.nome, p->nome) == TRUE) {
                arbitro->clientes[i] = arbitro->clientes[arbitro->nClientes-1];

                tmpClientes = realloc(arbitro->clientes, (arbitro->nClientes-1)*sizeof(Cliente));
                if(tmpClientes == NULL) return FALSE;

                arbitro->clientes = tmpClientes;
                arbitro->nClientes--;

                return TRUE;
            }
        }
    else { // If there is only one client remaing to remove
        free(arbitro->clientes);
        arbitro->clientes = NULL;
        arbitro->nClientes = 0;
        return TRUE;
    }

    return FALSE;
}

void printClientes(Arbitro *arbitro) {
    int i;
    printf("CLIENTS %d==> ", arbitro->nClientes);

    for(int i = 0; i < arbitro->nClientes; i ++) {
        printf("%d:%s ", i, arbitro->clientes[i].jogador.nome);
    }

    printf("\n");
}

int validate_connection(Arbitro *arbitro) {
    if(arbitro->nClientes >= arbitro->MAXPLAYERS) return FALSE; // Cant connect
    else return TRUE; // Can connenct
}

Jogo* getJogoByClienteName(Arbitro *arbitro, char *clienteName) {
    int i;

    for(i = 0; i < arbitro->nClientes; i++) {
        if(strcmp(arbitro->clientes[i].jogador.nome, clienteName) == TRUE) {
            return arbitro->clientes[i].jogo;
        }
    }

    return NULL;
}
 