#include <stdio.h>

#ifndef GENERAL_H
#define GENERAL_H

#define TAM 50


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
    Jogador jogador;
} Jogo;

typedef struct Cliente{
    Jogador jogador;
    Jogo *jogo;
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

#endif // GENERAL_

