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

int validate_connection(Arbitro *arbitro) {
    if(arbitro->nClientes >= arbitro->MAXPLAYERS) return FALSE; // Cant connect
    else return TRUE; // Can connenct
}

int add_cliente(Arbitro *arbitro, PEDIDO *p) {
    // Check if the MAXPLAYERS limit is
    int newClientIndex;
    if(validate_connection(arbitro) == FALSE) return FALSE;
    
    Cliente newCliente;
    Cliente* tmpClientes;
    Jogador newPlayer;
    strcpy(newPlayer.nome, p->nome);
    newPlayer.pontuacao = 0;
    newCliente.jogador = newPlayer;
    newClientIndex = arbitro->nClientes;

    tmpClientes = realloc(arbitro->clientes, (arbitro->nClientes+1)*sizeof(Cliente));

    if(tmpClientes == NULL) return FALSE;

    free(arbitro->clientes);
    arbitro->clientes = tmpClientes;
    arbitro->clientes[newClientIndex] = newCliente;

    arbitro->nClientes++;

    return TRUE;
}

void printClientes(Arbitro *arbitro) {
    int i;
    printf("CLIENTS %d==> ", arbitro->nClientes);

    for(int i = 0; i < arbitro->nClientes; i ++) {
        printf("%s ", arbitro->clientes[0].jogador.nome);
    }

    printf("\n");
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
        // printf("RESPONSE [%s] => %s\n", resp.nome, resp.comando);
        sprintf(fifo, FIFO_CLI, p.nome);
        fdr = open(fifo, O_WRONLY);
        strcpy(resp.nome, p.nome);
        strcpy(resp.code, code);
        strcpy(resp.desc, desc);
        n = write(fdr, &resp, sizeof(RESPONSE));
        close(fdr);
        printf("Written %s %s %s\n", resp.nome, resp.code, resp.desc);
    }
}


int main(int argc, char *argv[]){
    Arbitro arbitro;
    PEDIDO p;
    RESPONSE resp;
    int fd, n, fdr, fdlixo, res;
    char fifo[40], cmd[40], op[1];
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

    do {
        // printf("COMANDO: ");
        // fflush(stdout);
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(fd, &fds);
        tempo.tv_sec = 10;
        tempo.tv_usec = 0;
        res = select(fd + 1, &fds, NULL, NULL, NULL);

        if(res == 0) printf("Nada pra ler\n");
        else if(res > 0 && FD_ISSET(0, &fds)) {
            scanf("%s", cmd);
            printf("O administrado pediu o comando '%s'\n", cmd);
        } else if(res > 0 && FD_ISSET(fd, &fds)) {
            n = read(fd, &p, sizeof(PEDIDO));

            printf("Recebi %s %s\n", p.nome, p.comando);

            if(strcmp(p.comando, "_connect_") == TRUE) {
                if(add_cliente(&arbitro, &p) == TRUE) {
                    sendResponse(p, "_connection_accept_", "", fifo, n);
                } else {
                    sendResponse(p, "_connection_failed_", "_max_players_", fifo, n);
                }
                printClientes(&arbitro);
            } else {
                sendResponse(p, "mudei_bitch", "", fifo, n);
            }


        }
    } while(p.comando != "m");

    close(fd);
    unlink(FIFO_SRV);
    exit(0);
}
