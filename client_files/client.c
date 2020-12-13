#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../utils.h"
#include "../general.h"
#include <signal.h>

// Global vars to be accessible to the sigint_handler()
char fifo[40];
char playerName[40];

void sigint_handler(int s) {
    int fd, n;
    PEDIDO p;

    // Send disconnect message to arbitro
    if(access(fifo, F_OK) == 0) {
        fd = open(FIFO_SRV, O_WRONLY);
        strcpy(p.nome, playerName); 
        strcpy(p.comando, "#quit"); 
        n = write(fd, &p, sizeof(PEDIDO));
    }

    putchar('\n');
    close(fd);
    unlink(fifo);
    exit(0);
}

void sigusr2_handler(int s) {
	printf("\n%s saiu do campeonato!\n", playerName);
	sigint_handler(s);
}

void processResponse(RESPONSE resp, char *fifo) {
    if(strcmp(resp.code, "_connection_failed_") == TRUE) {
        if(strcmp(resp.desc, "_max_players_") == TRUE)
            printf("[ERRO] O numero maximo de jogadores foi atingido!\n");
        else
            printf("[ERRO] Erro ao conectar ao arbitro!\n");

        unlink(fifo);
        exit(0);
    } else if(strcmp(resp.code, "_success_arbitro_") == TRUE)
        printf("[ARBITRO]: %s\n", resp.desc);
    else if(strcmp(resp.code, "_success_game_") == TRUE)
        printf("[GAME]: %s\n", resp.desc);
    else if(strcmp(resp.code, "_error_") == TRUE)
        if(strcmp(resp.desc, "_no_game_assigned_") == TRUE)
            printf("[ERROR/ARBITRO]: Nao existe nenhum jogo associado a este cliente\n");
        else if(strcmp(resp.desc, "_invalid_command_") == TRUE)
            printf("[ERROR/ARBITRO]: Comando invalido\n");
        else
            printf("[ERROR/ARBITRO]: Ocorreu um erro\n");
}

/*
    Get response from arbitro
*/
void getResponse(char *fifo) {
    RESPONSE resp;
    int fdr = open(fifo, O_RDONLY);
    int n = read(fdr, &resp, sizeof(RESPONSE));
    close(fdr);
    processResponse(resp, fifo);
}

/*
    Make connection request to arbitro and process it.
*/
void connect_to_arbitro(PEDIDO p, int *fd) {
    int n;
    
    mkfifo(fifo, 0600);
    *fd = open(FIFO_SRV, O_WRONLY);
    strcpy(p.comando, "#_connect_");
    p.pid = getpid();
    n = write(*fd, &p, sizeof(PEDIDO));
    getResponse(fifo); 
}

void main(){
    int fd, n, fdr;
    PEDIDO p;
    RESPONSE resp;
    printf("\n\nPID <%d>\n\n", getpid());

    signal(SIGINT, sigint_handler); // Ignore ^C
	  signal(SIGUSR2, sigusr2_handler);

    if(access(FIFO_SRV, F_OK) == -1) {
        fprintf(stderr, "[ERR] O servidor não está a correr\n");
        exit(1);
    }

    while(1) {
        printf("Nome de jogador: ");
        scanf("%s", playerName);

        sprintf(fifo, FIFO_CLI, playerName);

        if(access(fifo, F_OK) == 0) {
            fprintf(stderr, "[ERR] Nome de jogador já existe\n");
        } else break;
    }

    strcpy(p.nome, playerName);
    connect_to_arbitro(p, &fd);

    while(1) {
        printf("Comando => ");
        scanf("%s", p.comando);
        p.pid = getpid();
        
        n = write(fd, &p, sizeof(PEDIDO));

        getResponse(fifo);
    }

    close(fd);
    unlink(fifo);
    exit(0);
}
