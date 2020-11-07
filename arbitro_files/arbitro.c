#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

int main(int argc, char *argv[]){
    Arbitro arbitro;

    printf("ARBITRO\n");
    getEnvironmentVariables(&arbitro);
    getArgs(&arbitro, argc, argv);

    return 0;
}
