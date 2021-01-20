#include <stdio.h> 
#include <sys/stat.h> 
#include <sys/wait.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../utils.h"

#include "../general.h"
#include "client_handlers.h"
#include "arbitro_handlers.h"
#include "request_functions.h"

pid_t childPid;

void despertar(int sinal){
	kill(childPid, SIGUSR1);
}

/*
    Read environment variables GAMEDIR and MAXPLAYER
*/
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
    Read cli arguments
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

/*
    Get env variables, cli args and initialize arbitro.
*/
void initArbitro(Arbitro *arbitro, int argc, char* argv[]) {
    getEnvironmentVariables(arbitro);
    getArgs(arbitro, argc, argv);

    arbitro->nClientes = 0;
    arbitro->nJogos = 0;
    arbitro->clientes = NULL;
    arbitro->jogos = NULL;
}
/*
    Handle connection request: #_connect_ command
*/
void handleConnectRequest(Arbitro *arbitro, PEDIDO p, char *fifo, int n) {
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
}


/*
    Handle command for the arbitro internal usage
*/
void handleClientCommandsForArbitro(Arbitro *arbitro, PEDIDO p, char *fifo, int n) {
    // Check for #_connect_ command and if client isnt already connected
    // When a client make a connection request
    if(strcmp(p.comando, "#_connect_") == TRUE && validate_client_connected(arbitro, p.pid) == TRUE) { 
        handleConnectRequest(arbitro, p, fifo, n);
    } else if(strcmp(p.comando, "#quit") == TRUE)
        commandClientQuit(arbitro, &p);
    else if(strcmp(p.comando, "#mygame") == TRUE)
        commandClientMyGame(arbitro, &p, fifo, n);
    else sendResponse(p, "_error_", "_invalid_command_", fifo, n);
}

int handleArbitroCommands(Arbitro *arbitro, PEDIDO p) {
    char word[] = "";
    char adminCommand[40];
    scanf("%s", adminCommand);
    printf("=> %s\n", adminCommand);
    if(strcmp(adminCommand, "players") == TRUE){
        commandArbitroPlayers(arbitro);
    }else if(strcmp(adminCommand, "games") == TRUE){
        commandArbitroGames(arbitro);
    }else if(adminCommand[0] == 'k'){
        commandArbitroK(arbitro, adminCommand, word);
    } else if(strcmp(adminCommand, "exit") == TRUE){
        commandArbitroExit(arbitro);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]){
    Arbitro arbitro;
    PEDIDO p;
    RESPONSE resp;
    int fd, n, fdr, fdlixo, res;
    char fifo[40];
    fd_set fds;

    initArbitro(&arbitro, argc, argv);
    printf("ARBITRO\n");

    if(access(FIFO_SRV, F_OK)) {
        mkfifo(FIFO_SRV, 0600);
        printf("FIFO Criado...\n");
    }

    fd = open(FIFO_SRV, O_RDWR);
    printf("FIFO aberto: '%s'\n", FIFO_SRV);
    printClientes(&arbitro);
    
    do {
        printf("\n[ADMIN]: ");
        fflush(stdout);

        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(fd, &fds);
        res = select(fd + 1, &fds, NULL, NULL, NULL);

        if(res == 0) printf("Nada pra ler\n");
        else if(res > 0 && FD_ISSET(0, &fds)) { // Admin
            if(handleArbitroCommands(&arbitro, p) == 1) break;
        } else if(res > 0 && FD_ISSET(fd, &fds)) { // Clients
            n = read(fd, &p, sizeof(PEDIDO));

            if(p.comando[0] == '#') // Command for arbitro
                handleClientCommandsForArbitro(&arbitro, p, fifo, n);
            else { // Command for game...
                printf("To be processed by the game\n");
                sendResponse(p, "_success_game_", "output do jogo...", fifo, n);
            }
        }
    } while(1);

    close(fd);
    unlink(FIFO_SRV);
    exit(0);
}
