#include <stdio.h> 
#include <string.h> 
#include <sys/stat.h> 
#include <sys/wait.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../utils.h"

#include "../general.h"
#include "client_handlers.h"

pid_t childPid;

void despertar(int sinal){
	kill(childPid, SIGUSR1);
}

void getEnvironmentVariables(Arbitro *arbitro) {
    char *maxplayerBuff;

    if((arbitro->GAMEDIR = getenv("GAMEDIR")) == NULL) {
        printf("GAMEDIR NULLL\n");
    }else {
        printf("%s\n", arbitro->GAMEDIR);
    }

    if((maxplayerBuff = getenv("MAXPLAYER")) == NULL) {
        printf("MAXPLAYERS NULLL\n");
    } else {
        arbitro->MAXPLAYERS = atoi(maxplayerBuff);
        printf("%d\n", arbitro->MAXPLAYERS);
    }
}

/*
    ./arbitro.o -d xxx -t xxx
    -d => Championship duration
    -t => Waiting time
*/
void getArgs(Arbitro *arbitro, int argc, char *argv[]) {
    int opt;
    char *subopts, *value;

    // Make them negative for later validation
    arbitro->DURACAO_CAMPEONATO = -1;
    arbitro->TEMPO_ESPERA = -1;

    while((opt = getopt(argc, argv, "d:t:")) != -1) {  
        switch(opt) {
            case 'd':
                printf("%s\n", optarg);
                arbitro->DURACAO_CAMPEONATO = atoi(optarg);
                break;
            case 't':
                printf("%s\n", optarg);
                arbitro->TEMPO_ESPERA = atoi(optarg);
                break;
            default:
                printf("Invalid argument: %s\n", opt);
        }
    }

    // Values validation. If they're <= 0, then they're invalid and the program ends
    if(!arbitro->DURACAO_CAMPEONATO || arbitro->DURACAO_CAMPEONATO <= 0) {
        printf("[ERRO] - Duracao do campeonato em falta/invalida. Utilize o argumento '-d' para passar um valor válido\n");
        exit(1);
    }

    if(!arbitro->TEMPO_ESPERA || arbitro->TEMPO_ESPERA <= 0) {
        printf("[ERRO] - Tempo de espera em falta/invalida. Utilize o argumento '-t' para passar um valor válido\n");
        exit(1);
    }
}


void initArbitro(Arbitro *arbitro, int argc, char* argv[]) {
    getEnvironmentVariables(arbitro);
    getArgs(arbitro, argc, argv);

    arbitro->nClientes = 0;
    arbitro->nJogos = 0;
    arbitro->clientes = NULL;
    arbitro->jogos = malloc(0);
}

void sendResponse(PEDIDO p, char *code, char* desc, char *fifo, int n) {
    int fdr;
    RESPONSE resp;
    
    if(n == sizeof(PEDIDO)) {
        sprintf(fifo, FIFO_CLI, p.nome);
        fdr = open(fifo, O_WRONLY);
        strcpy(resp.nome, p.nome);
        strcpy(resp.code, code);
        strcpy(resp.desc, desc);
        n = write(fdr, &resp, sizeof(RESPONSE));
        close(fdr);
    }
}

// When a client quits/closes the program
void commandQuit(Arbitro *arbitro, PEDIDO *p) {
    if(remove_cliente(arbitro, p) == FALSE)
        printf("[ERRO] Erro ao remover cliente\n");
    else
        printf("[INFO] Cliente %s foi removido\n", p->nome);
    printClientes(arbitro);
}

// Send game info to client
void commandMyGame(Arbitro *arbitro, PEDIDO *p, char *fifo, int n) {
    Jogo* clientGameInfo = getJogoByClienteName(arbitro, p->nome);
    
    if(clientGameInfo == NULL)
        sendResponse(*p, "_error_", "_no_game_assigned_", fifo, n);
    else
        sendResponse(*p, "_success_arbitro_", clientGameInfo->nome, fifo, n);
}

void handleCommandsForArbitro(Arbitro *arbitro, PEDIDO p, char *fifo, int n) {

    if(strcmp(p.comando, "#_connect_") == TRUE) { // When a client make a connection request
        switch(add_cliente(arbitro, &p)) {
            case TRUE:
                sendResponse(p, "_connection_accept_", "", fifo, n);
                break;
            case MAX_PLAYER_ERR:
                sendResponse(p, "_connection_failed_", "_max_players_", fifo, n);
                break;
            case FALSE:
            default:
                sendResponse(p, "_connection_failed_", "", fifo, n);
        }
        printClientes(arbitro);
    } else if(strcmp(p.comando, "#quit") == TRUE)
        commandQuit(arbitro, &p);
    else if(strcmp(p.comando, "#mygame") == TRUE)
        commandMyGame(arbitro, &p, fifo, n);
    else sendResponse(p, "_error_", "_invalid_command_", fifo, n);
}

int main(int argc, char *argv[]){
    Arbitro arbitro;
    PEDIDO p;
    RESPONSE resp;
    int fd, n, fdr, fdlixo, res;
    char fifo[40], adminCommand[40];
    fd_set fds;
    struct timeval tempo;

    initArbitro(&arbitro, argc, argv);
    printf("ARBITRO\n");

    if(access(FIFO_SRV, F_OK)) {
        mkfifo(FIFO_SRV, 0600);
        printf("FIFO Criado...\n");
    }

    fd = open(FIFO_SRV, O_RDWR);
    printf("FIFO aberto... '%s'\n", FIFO_SRV);
    printClientes(&arbitro);
    
    do {
        printf("[ADMIN]: ");
        fflush(stdout);

        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(fd, &fds);
        tempo.tv_sec = 10;
        tempo.tv_usec = 0;
        res = select(fd + 1, &fds, NULL, NULL, NULL);

        if(res == 0) printf("Nada pra ler\n");
        else if(res > 0 && FD_ISSET(0, &fds)) { // Admin
            scanf("%s", adminCommand);
            printf("=> %s\n", adminCommand);
			if(strcmp(adminCommand, "players") == TRUE){
				printf("\nLista de Jogadores:\n");
				for(int j=0; j<arbitro.nClientes; j++){
					printf("\t-> %s\n", arbitro.clientes[j].jogador.nome);
				}
			}else if(strcmp(adminCommand, "games") == TRUE){
				printf("\nLista de Jogos disponiveis:\n");
				for(int j=0; j<arbitro.nJogos; j++){
					printf("\t-> %s\n", arbitro.jogos[j].nome);
				}
			}
        } else if(res > 0 && FD_ISSET(fd, &fds)) { // Clients
            n = read(fd, &p, sizeof(PEDIDO));

            printf("Recebi %s %s\n", p.nome, p.comando);

            if(p.comando[0] == '#') // Command for arbitro
                handleCommandsForArbitro(&arbitro, p, fifo, n);
            else { // Command for game...
                printf("To be processed by the game\n");
                sendResponse(p, "_success_game_", "output do jogo...", fifo, n);
            }
        }
    } while(p.comando != "m");

    close(fd);
    unlink(FIFO_SRV);
    exit(0);
}
