#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include "../general.h"

int charToInt(char c){
	int num = 0;

	//Substract '0' from entered char to get
	//corresponding digit
	num = c - '0';
	return num;
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

void getArgs(Arbitro *arbitro, int argc, char *argv[]) {
    int opt;
    char *subopts, *value;

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
}

void main(int argc, char *argv[]){
    Arbitro arbitro;

    printf("ARBITRO\n");
    getEnvironmentVariables(&arbitro);
    getArgs(&arbitro, argc, argv);
}