#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../utils.h"

void processRepsonse(RESPONSE resp, char *fifo) {
    if(strcmp(resp.code, "_connection_failed_") == TRUE) {
        if(strcmp(resp.desc, "_max_players_") == TRUE)
            printf("[ERRO] O numero maximo de jogadores foi atingido!\n");

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

void main(){
    int fd, n, fdr;
    char fifo[40];
    char str[40];
    PEDIDO p;
    RESPONSE resp;

    if(access(FIFO_SRV, F_OK) == -1) {
        fprintf(stderr, "[ERR] O servidor não está a correr\n");
        exit(6);
    }

    do {
        printf("Nome de jogador: ");
        scanf("%s", p.nome);

        sprintf(fifo, FIFO_CLI, p.nome);

        if(access(fifo, F_OK) == 0) {
            fprintf(stderr, "[ERR] Nome de jogador já existe\n");
        } else break;
    } while(1);

    mkfifo(fifo, 0600);
    printf("Criei o meu fifo\n");

    fd = open(FIFO_SRV, O_WRONLY);
    strcpy(p.comando, "_connect_");
    n = write(fd, &p, sizeof(PEDIDO));
    getResponse(fifo); 

    do {
        printf("Comando => ");
        scanf("%s", p.comando);

        n = write(fd, &p, sizeof(PEDIDO));

        printf("Enviado %s %s\n", p.nome, p.comando);
        getResponse(fifo);
    } while(1);

    printf("Terminei %s\n", fifo);
    close(fd);
    unlink(fifo);
    exit(0);
}