#include <stdio.h>

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
    Jogo jogo;
} Cliente;

typedef struct Arbitro{
    char *GAMEDIR;
    int MAXPLAYERS;
    Jogo* jogos;                //Lista de Jogos
    Cliente* clientes;              //lista de Clientes
    int nClientes;
    int nJogos;
} Arbitro;




