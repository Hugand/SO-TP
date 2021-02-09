#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include "client_handlers.h"

/*
    Add a new client to arbitro if the number of players don't exceed
    the MAX_PLAYER number.
*/
int add_cliente(Arbitro *arbitro, PEDIDO *p) {
    // Check if the MAXPLAYERS limit is
    if(validate_max_player_exceed(arbitro) == FALSE) return MAX_PLAYER_ERR;
    int newClientIndex;

    // Attributed game to be used only for demo of the #mygame command
    // To be removed in meta 3
    
    Cliente newCliente;
    Cliente* tmpClientes;

    strcpy(newCliente.jogador.nome, p->nome);
    strcpy(newCliente.jogo.nome, "");
    sprintf(newCliente.fifo, FIFO_CLI, newCliente.jogador.nome);
    newCliente.jogador.pontuacao = 0;
    newCliente.pid = p->pid;
    strcpy(newCliente.jogo.gameCommand, "");
    newCliente.isConnectionSuspended = FALSE;
    newClientIndex = arbitro->nClientes;

    tmpClientes = realloc(arbitro->clientes, (arbitro->nClientes+1)*sizeof(Cliente));
    if(tmpClientes == NULL) return FALSE;

    arbitro->clientes = tmpClientes;
    arbitro->clientes[newClientIndex] = newCliente;
    arbitro->nClientes++;

    return TRUE;
}

/*
    Remove cliente from arbitro by their name
*/
int remove_cliente(Arbitro *arbitro, char *clienteName) {
    int i;
    Cliente *tmpClientes;

    for(i = 0; i < arbitro->nClientes; i++) {
        if(strcmp(arbitro->clientes[i].jogador.nome, clienteName) == TRUE) {
            if(arbitro->clientes[i].jogo.gamePID > 0) {
                printf("KILL RES %d %d\n", kill(SIGUSR1, arbitro->clientes[i].jogo.gamePID), arbitro->clientes[i].jogo.gamePID);
                pthread_kill(arbitro->clientes[i].jogo.readThread, SIGUSR1);
                pthread_kill(arbitro->clientes[i].jogo.writeThread, SIGUSR1);
            }
            
            // If it is not removing the last remaining client
            if(arbitro->nClientes > 1) {
                arbitro->clientes[i] = arbitro->clientes[arbitro->nClientes-1];

                tmpClientes = realloc(arbitro->clientes, (arbitro->nClientes-1)*sizeof(Cliente));
                if(tmpClientes == NULL) return FALSE;

                // free(arbitro->clientes);
                arbitro->clientes = tmpClientes;
                arbitro->nClientes--;

            } else { // If there is only one client remaing to remove
                free(arbitro->clientes); 
                arbitro->clientes = NULL;
                arbitro->nClientes = 0;
            }

            return TRUE;
        }
    }

    return FALSE;
}

/*
    Print a list of clients
*/
void printClientes(Arbitro *arbitro) {
    int i;
    printf("CLIENTS %d==> ", arbitro->nClientes);

    for(int i = 0; i < arbitro->nClientes; i ++) {
        printf("%d:%s ", i, arbitro->clientes[i].jogador.nome);
    }

    printf("\n");
}

/*
    Validate if the number of players has exceeded the MAX_PLAYER count.
*/
int validate_max_player_exceed(Arbitro *arbitro) {
    if(arbitro->nClientes >= arbitro->MAXPLAYERS) return FALSE; // Cant connect
    else return TRUE; // Can connenct
}

/*
    Validate if the client is already connected
*/
int validate_client_connected(Arbitro *arbitro, pid_t pid) {
    int i;
    for(i = 0; i < arbitro->nClientes; i++) {
        // if(strcmp(arbitro->clientes[i].jogador.nome, nome) == TRUE) {
        if(arbitro->clientes[i].pid == pid) {
           return FALSE;
        }
    }
    return TRUE;
}

/*
    Return a pointer to the game being played by a given player.
*/
Jogo* getJogoByClienteName(Arbitro *arbitro, char *clienteName) {
    int i;

    for(i = 0; i < arbitro->nClientes; i++) {
        if(strcmp(arbitro->clientes[i].jogador.nome, clienteName) == TRUE) {
            return &arbitro->clientes[i].jogo;
        }
    }

    return NULL;
}
 
/*
    Handle the #quit command
*/
void commandClientQuit(Arbitro *arbitro, PEDIDO *p) {
    Cliente *cliente = getClienteByName(arbitro, p->nome);
    char fifo[30];

    if(cliente != NULL) {
        strcpy(fifo, cliente->fifo);
        if(remove_cliente(arbitro, p->nome) == FALSE) {
            printf("[ERRO] Erro ao remover cliente\n");
            sendResponse(*p, "_error_", "", cliente->fifo, sizeof(PEDIDO));
        } else {
            printf("[INFO] Cliente %s foi removido\n", p->nome);
            sendResponse(*p, "_quit_", "", fifo, sizeof(PEDIDO));
        }
        printClientes(arbitro);
    } else {
        printf("[ERRO] Cliente %s nao existe\n", p->nome);
    }
}

/*
    Handle the #mygame command
*/
void commandClientMyGame(Arbitro *arbitro, PEDIDO *p, char *name, int n) {
    // Jogo* clientGameInfo = getJogoByClienteName(arbitro, p->nome);
    Cliente* cliente = getClienteByName(arbitro, name);

    printf(cliente->jogo.nome);
    
    if(cliente == NULL)
        printf("\n[ERRO] Nao existe o cliente %s\n", name);
    else if(strcmp(cliente->jogo.nome, "") == TRUE) {
        printf("\n[INFO] Nao foi associado nenhum jogo\n");
        sendResponse(*p, "_error_", "_no_game_assigned_", cliente->fifo, sizeof(*p));
    } else {
        printf("\n[INFO] Jogo associado ao jogador %s => %s\n", cliente->jogador.nome, cliente->jogo.nome);
        sendResponse(*p, "_success_arbitro_", cliente->jogo.nome, cliente->fifo, sizeof(*p));
    }

}

Cliente* getClienteByName(Arbitro* arbitro, char *name) {
    int i;

    for(i = 0; i < arbitro->nClientes; i++)
        if(strcmp(arbitro->clientes[i].jogador.nome, name) == TRUE)
            return &arbitro->clientes[i];
}