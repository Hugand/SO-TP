#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../utils.h"

void processRepsonse(PEDIDO resp, char *fifo) {
    if(strcmp(resp.comando, "_connection_failed_") == TRUE) {
        unlink(fifo);
        printf("[ERRO] Falha ao ligar ao servidor\n");
        exit(0);
    }
}

void getResponse(char *fifo) {
    PEDIDO resp;
    int fdr = open(fifo, O_RDONLY);
    int n = read(fdr, &resp, sizeof(PEDIDO));
    close(fdr);
    printf("Recebido %s %s\n", resp.nome, resp.comando);
    processRepsonse(resp, fifo);
}

void main(){
    int fd, n, fdr;
    char fifo[40];
    char str[40];
    PEDIDO p;

    if(access(FIFO_SRV, F_OK) == 1) {
        fprintf(stderr, "[ERR] O servidor não está a correr\n");
        exit(6);
    }

    printf("Nome de jogador: ");
    scanf("%s", p.nome);

    sprintf(fifo, FIFO_CLI, p.nome);
    mkfifo(fifo, 0600);
    printf("Criei o meu fifio\n");

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