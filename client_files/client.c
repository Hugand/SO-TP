#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../utils.h"
#include <signal.h>

// Global vars to be accessible to the sigint_handler()
char fifo[40];
char playerName[40];

void sigint_handler(int s) {
    int fd, n;
    PEDIDO p;

    if(access(fifo, F_OK) == 0) {
        fd = open(FIFO_SRV, O_WRONLY);
        strcpy(p.nome, playerName); 
        strcpy(p.comando, "_disconnect_"); 
        n = write(fd, &p, sizeof(PEDIDO));
    }

    putchar('\n');
    close(fd);
    unlink(fifo);
    exit(0);
}

void processRepsonse(RESPONSE resp, char *fifo) {
    if(strcmp(resp.code, "_connection_failed_") == TRUE) {
        if(strcmp(resp.desc, "_max_players_") == TRUE)
            printf("[ERRO] O numero maximo de jogadores foi atingido!\n");
        else
            printf("[ERRO] Erro ao conectar ao arbitro!\n");

        unlink(fifo);
        exit(0);
    }
}

void getResponse(char *fifo) {
    RESPONSE resp;
    int fdr = open(fifo, O_RDONLY);
    int n = read(fdr, &resp, sizeof(RESPONSE));
    close(fdr);
    printf("Recebido %s %s %s\n", resp.nome, resp.code, resp.desc);
    processRepsonse(resp, fifo);
}

void connect_to_arbitro() {
    int fd, n;
    PEDIDO p;
    
    mkfifo(fifo, 0600);
    fd = open(FIFO_SRV, O_WRONLY);
    strcpy(p.nome, playerName);
    strcpy(p.comando, "_connect_");
    n = write(fd, &p, sizeof(PEDIDO));
    getResponse(fifo); 
}

void main(){
    int fd, n, fdr;
    PEDIDO p;
    RESPONSE resp;

    signal(SIGINT, sigint_handler); // Ignore ^C

    if(access(FIFO_SRV, F_OK) == -1) {
        fprintf(stderr, "[ERR] O servidor não está a correr\n");
        exit(6);
    }

    while(1) {
        printf("Nome de jogador: ");
        scanf("%s", playerName);

        sprintf(fifo, FIFO_CLI, playerName);

        if(access(fifo, F_OK) == 0) {
            fprintf(stderr, "[ERR] Nome de jogador já existe\n");
        } else break;
    }

    connect_to_arbitro();

    while(1) {
        printf("Comando => ");
        scanf("%s", p.comando);

        n = write(fd, &p, sizeof(PEDIDO));

        printf("Enviado %s %s\n", p.nome, p.comando);
        getResponse(fifo);
    }

    printf("Terminei %s\n", fifo);
    close(fd);
    unlink(fifo);
    exit(0);
}