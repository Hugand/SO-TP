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
#include "../utils.h"
#include <dirent.h>
#include "../general.h"
#include "client_handlers.h"
#include "arbitro_handlers.h"
#include "request_functions.h"
#include "cli_games_handlers.h"

/* VARIAVEIS GLOBAIS*/
int readyToStart = 0;
int gameStarted = FALSE;
int serverFd;
pid_t childPid;
Arbitro arbitro;            //Arbitro como variavel global

void sorteioJogos();

// void despertar(int sinal){
// 	kill(childPid, SIGUSR1);
// }

void sigint_handler(int s) {
    printf("\n");
    close(serverFd);
    unlink(FIFO_SRV);
    exit(0);
}
/*
    Read environment variables GAMEDIR and MAXPLAYER
*/
void getEnvironmentVariables() {
    char *maxplayerBuff;

    if((arbitro.GAMEDIR = getenv("GAMEDIR")) == NULL) {
        printf("GAMEDIR NULLL\n");
    }else {
        printf("%s\n", arbitro.GAMEDIR);
    }

    if((maxplayerBuff = getenv("MAXPLAYER")) == NULL) {
        printf("MAXPLAYERS NULLL\n");
    } else {
        arbitro.MAXPLAYERS = atoi(maxplayerBuff);
        printf("%d\n", arbitro.MAXPLAYERS);
    }
}

/*
    Read cli arguments
    ./arbitro.o -d xxx -t xxx
    -d => Championship duration
    -t => Waiting time
*/
void getArgs(int argc, char *argv[]) {
    int opt;
    char *subopts, *value;

    // Make them negative for later validation
    arbitro.DURACAO_CAMPEONATO = -1;
    arbitro.TEMPO_ESPERA = -1;

    while((opt = getopt(argc, argv, "d:t:")) != -1) {  
        switch(opt) {
            case 'd':
                printf("%s\n", optarg);
                arbitro.DURACAO_CAMPEONATO = atoi(optarg);
                break;
            case 't':
                printf("%s\n", optarg);
                arbitro.TEMPO_ESPERA = atoi(optarg);
                break;
            default:
                printf("Invalid argument: %s\n", opt);
        }
    }

    // Values validation. If they're <= 0, then they're invalid and the program ends
    if(!arbitro.DURACAO_CAMPEONATO || arbitro.DURACAO_CAMPEONATO <= 0) {
        printf("[ERRO] - Duracao do campeonato em falta/invalida. Utilize o argumento '-d' para passar um valor válido\n");
        exit(1);
    }

    if(!arbitro.TEMPO_ESPERA || arbitro.TEMPO_ESPERA <= 0) {
        printf("[ERRO] - Tempo de espera em falta/invalida. Utilize o argumento '-t' para passar um valor válido\n");
        exit(1);
    }
}

int countGames(){
    DIR *directory = opendir(arbitro.GAMEDIR);
    struct dirent *entry;
    int count=0;

    while((entry = readdir(directory)) != NULL){
        if(entry->d_name[0] == 'g' && entry->d_name[1] == '_'){
            arbitro.jogos[count] = entry->d_name;
            count++;
        }
    }
    return count;
}

/*
    Get env variables, cli args and initialize arbitro.
*/
void initArbitro(int argc, char* argv[]) {
    getEnvironmentVariables(arbitro);
    getArgs(argc, argv);

    arbitro.nClientes = 0;
    arbitro.nJogos = 0;
    arbitro.clientes = NULL;
    arbitro.nJogos = countGames();
}
/*
    Handle connection request: #_connect_ command
*/
void handleConnectRequest(PEDIDO p, char *fifo, int n) {
    if(gameStarted == TRUE) {
        printf("[WARNING] Coneccao do jogador %s recusada. O campeonato ja comecou.\n", p.nome);
        sendResponse(p, "_connection_failed_", "_game_started_", fifo, n);
        return;
    }

    switch(add_cliente(&arbitro, &p)) {
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
    printClientes(&arbitro);
}


/*
    Handle command for the arbitro internal usage
*/
void handleClientCommandsForArbitro(PEDIDO p, char *fifo, int n) {
    // Check for #_connect_ command and if client isnt already connected
    // When a client make a connection request
    // sendResponse(p, "_test_command_", "", fifo, sizeof(p));
    if(strcmp(p.comando, "#_connect_") == TRUE && validate_client_connected(&arbitro,p.pid) == TRUE) { 
        handleConnectRequest(p, fifo, n);
    } else if(strcmp(p.comando, "#quit") == TRUE) {
        commandClientQuit(&arbitro, &p);
    } else if(strcmp(p.comando, "#mygame") == TRUE)
        commandClientMyGame(&arbitro, &p, fifo, n);
    else sendResponse(p, "_error_", "_invalid_command_", fifo, n);
}

int handleArbitroCommands(char *fifo) {
    PEDIDO p;
    char adminCommand[40];
    scanf("%s", adminCommand);

    if(strcmp(adminCommand, "players") == TRUE){
        commandArbitroPlayers(&arbitro);
    }else if(strcmp(adminCommand, "games") == TRUE){
        commandArbitroGames(&arbitro);
    } else if(strcmp(adminCommand, "stop") == TRUE){
        stopGames(&arbitro, &readyToStart);
    } else if(adminCommand[0] == 'k'){
        commandArbitroK(&arbitro, adminCommand);
    } else if(adminCommand[0] == 's'){
        commandArbitroConSuspensa(&arbitro, adminCommand, &p, TRUE);
        sendResponse(p, "_con_suspensa_", "", fifo, sizeof(p));
    } else if(adminCommand[0] == 'r'){
        commandArbitroConSuspensa(&arbitro, adminCommand, &p, FALSE);
        sendResponse(p, "_con_retomada_", "", fifo, sizeof(p));
    } else if(strcmp(adminCommand, "exit") == TRUE){
        commandArbitroExit(&arbitro);
        return 1;
    }

    return 0;
}

void handleClientsMessages(int fd, char *fifo) {
    PEDIDO p;
    Cliente *client;
    int n;

    n = read(fd, &p, sizeof(PEDIDO));
    printf("COMANDO PLAYER: %s\n", p.comando);
    if(p.comando[0] == '#') // Command for arbitro
        handleClientCommandsForArbitro(p, fifo, n);
    else { // Command for game...
        client = getClienteByName(&arbitro, p.nome);

        if(client->isConnectionSuspended == TRUE) {
            printf("Comunicação de %s suspensa\n", client->jogador.nome);
            sendResponse(p, "_con_suspensa_", "[WARNING] Comunicacao jogador-jogo foi suspensa.", fifo, n);
        } else {
            strcpy(client->jogo.gameCommand, p.comando);
            printf("To be processed by the game: %s\n", client->jogo.gameCommand);
        }
    }
}

void *runClientMessagesThread(void *arg) {
    THREAD_CLI_MSG *tcm = (THREAD_CLI_MSG *)arg;
    char *fifo = tcm->fifo;
    int fd = tcm->fd;

    while(tcm->stop == 0) {
        handleClientsMessages(fd, fifo);
        printf("\n[ADMIN]: ");
    }
}

void* gameThread(void* arg){
    Cliente *cliente = (Cliente *) arg;
    printf("====> %d\n", gameStarted);
    initJogo(cliente, &gameStarted, &arbitro);
}

void sorteioJogos() {
    if(gameStarted == FALSE) {
        printf("[ SORTEANDO JOGOS ] -- %d\n", arbitro.nClientes);
        gameStarted = TRUE;
        int num;
        
        for(int i = 0; i < arbitro.nClientes; i++) {
            printf("Starting game for jogador %s\n", arbitro.clientes[i].jogador.nome);
            num = intUniformRnd(1,arbitro.nJogos);
            strcpy(arbitro.clientes[i].jogo.nome , arbitro.jogos[num-1]) ;

            pthread_create(&arbitro.clientes[i].jogo.gameThread, NULL, &gameThread ,&arbitro.clientes[i]);
        }
    }
}

void espera(int sinal){
    setbuf(stdout, NULL);
    printf("\nVai se dar inicio aos jogos!");
    sorteioJogos();                 //Sorteia os jogos e inica os mesmos
}

void* iniciaEspera(void* arg){
    signal(SIGALRM, espera);
    printf("Aguarde por outros jogadores...");
    while(1){
        if(arbitro.nClientes >= 2){
            alarm(arbitro.TEMPO_ESPERA);
            sleep(12);
            break;
        }
    }  
    pthread_exit(NULL);      
}


int main(int argc, char *argv[]){
    pthread_t clientMessagesThread, arbitroCommandsThread, gameT, waitThread;
    PEDIDO p;
    RESPONSE resp;
    int n, res;
    char fifo[40];
    fd_set fds;
    THREAD_CLI_MSG thread_cli_msg;
    initRandom();
    initArbitro(argc, argv);
    printf("ARBITRO\n");

    signal(SIGINT, sigint_handler);

    if(access(FIFO_SRV, F_OK)) {
        mkfifo(FIFO_SRV, 0600);
        printf("FIFO Criado...\n");
    }

    serverFd = open(FIFO_SRV, O_RDWR);
    printf("FIFO aberto: '%s'\n", FIFO_SRV);
    printClientes(&arbitro);

    thread_cli_msg.fd = serverFd;
    thread_cli_msg.stop = 0;
    strcpy(thread_cli_msg.fifo, fifo);

    pthread_create(&clientMessagesThread, NULL, &runClientMessagesThread, &thread_cli_msg);
    // pthread_create(&gameT, NULL, &gameThread, NULL);
    pthread_create(&waitThread, NULL, &iniciaEspera, &arbitro);
    //iniciaEspera(&arbitro);
    do {
        printf("\n[ADMIN]: ");
        fflush(stdout);
        
        if(handleArbitroCommands(fifo) == 1) break;
    } while(1);

    thread_cli_msg.stop = 1;

    pthread_join(clientMessagesThread, NULL);
    // pthread_join(gameT, NULL);

    close(serverFd);
    unlink(FIFO_SRV);
    exit(0);
}
