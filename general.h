#include <stdio.h>
#include <sys/types.h>
#ifndef GENERAL_H
#define GENERAL_H

#define TAM 50
#define FIFO_SRV "pipe_tubo"
#define FIFO_CLI "pipe_cli%s"


/*****************************/
/*   Structs Dados Gerais    */
/*****************************/

typedef struct Campeonato{
    int nJogadores;
    int tempo;
} Campeonato;

typedef struct Jogador{
    char nome[TAM];
    int pontuacao;
} Jogador;

typedef struct Jogo{
    char nome[TAM];
} Jogo;

typedef struct Cliente{
    Jogador jogador;
    Jogo *jogo;
    pid_t pid;
    int isConnectionSuspended;
} Cliente;

typedef struct Arbitro{
    int DURACAO_CAMPEONATO;
    int TEMPO_ESPERA;
    char *GAMEDIR;
    int MAXPLAYERS;
    Jogo* jogos;                //Lista de Jogos
    Cliente* clientes;              //lista de Clientes
    int nClientes;
    int nJogos;
} Arbitro;

typedef struct{
    char nome[20];
    char comando[20];
    pid_t pid;
} PEDIDO;

typedef struct{
    char nome[20];
    char code[20]; // Response code ex: _connection_failed_
    char desc[100]; // Response description ex. "_max_players_"
} RESPONSE;



#endif // GENERAL_

