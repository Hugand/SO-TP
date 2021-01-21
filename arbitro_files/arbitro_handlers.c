#include "arbitro_handlers.h"
#include <string.h>

void commandArbitroPlayers(Arbitro *arbitro) {
    printf("\nLista de Jogadores:\n");
    for(int j=0; j<arbitro->nClientes; j++){
        printf("\t-> %s\n", arbitro->clientes[j].jogador.nome);
    }
}

void commandArbitroGames(Arbitro *arbitro) {
    printf("\nLista de Jogos disponiveis:\n");
    for(int j=0; j<arbitro->nJogos; j++){
        printf("\t-> %s\n", arbitro->jogos[j].nome);
    }
}

void commandArbitroK(Arbitro *arbitro, char* adminCommand, char* word) {
    PEDIDO ptmp;
    int wordSize = strlen(adminCommand);
    int doesNameExist = 0;
    for(int j=1; j<wordSize; j++){
        strncat(word, &adminCommand[j],1);
    }
    for(int i=0; i<arbitro->nClientes; i++){
        if(strcmp(arbitro->clientes[i].jogador.nome, word) == TRUE){
            strcpy(ptmp.nome,arbitro->clientes[i].jogador.nome);
            ptmp.pid = arbitro->clientes[i].pid;
            commandClientQuit(arbitro, &ptmp);
            doesNameExist = 1;
            break;
        }
    }
    //Caso o nome nao exista
    if(!doesNameExist){
        printf("\nErro no comando! Digite um nome existente (knome)\n");
    }
}

void commandArbitroExit(Arbitro *arbitro) {
    PEDIDO ptmp;
    for(int i = 0; i < arbitro->nClientes; i++){
        strcpy(ptmp.nome,arbitro->clientes[i].jogador.nome);
        ptmp.pid = arbitro->clientes[i].pid;
        commandClientQuit(arbitro, &ptmp);
    }
}

void commandArbitroS(Arbitro *arbitro, char* adminCommand, char* word) {
    PEDIDO ptmp;
    int wordSize = strlen(adminCommand);
    int doesNameExist = 0;
    for(int j=1; j<wordSize; j++){
        strncat(word, &adminCommand[j],1);
    }
    for(int i=0; i<arbitro->nClientes; i++){
        if(strcmp(arbitro->clientes[i].jogador.nome, word) == TRUE){
            arbitro->clientes[i].isConnectionSuspended = TRUE;
            doesNameExist = 1;
            break;
        }
    }
    //Caso o nome nao exista
    if(!doesNameExist){
        printf("\nErro no comando! Digite um nome existente (snome)\n");
    }
}
