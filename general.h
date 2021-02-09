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
    pthread_t gameThread;
    pthread_t readThread, writeThread;
    int readPipe[2], writePipe[2];
    char gameCommand[50];
    pid_t gamePID;
} Jogo;

typedef struct Cliente{
    Jogador jogador;
    Jogo jogo;
    pid_t pid;
    int isConnectionSuspended;
    char fifo[30];
} Cliente;

typedef struct Arbitro{
    int DURACAO_CAMPEONATO;
    int TEMPO_ESPERA;
    char *GAMEDIR;
    int MAXPLAYERS;
    char* jogos[TAM];                //Lista de Jogos
    Cliente* clientes;              //lista de Clientes
    Cliente* winner;
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
    char desc[3000]; // Response description ex. "_max_players_"
} RESPONSE;


typedef struct THREAD_CLI_MSG {
    int fd;
    char fifo[40];
    int stop;
} THREAD_CLI_MSG;


typedef struct GAME_COMM_THRD_DATA {
    int *pipe;
    Cliente *cliente;
    int *isThreadRunning;
} GAME_COMM_THRD_DATA;


#endif // GENERAL_

