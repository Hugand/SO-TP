#include <stdio.h>

#define TAM 50


/*****************************/
/*   Structs Dados Gerais    */
/*****************************/

typedef struct Campeonato{
    int nJogadores;
    int tempo;
}Campeonato;


typedef struct Jogador{
    char nome[TAM];
    int pontuacao=0;
}Jogador;


typedef struct Arbitro{
    Jogo* jogos;                //Lista de Jogos
    Cliente* clientes;              //lista de Clientes
    int nClientes=0;
    int nJogos=0;
}Arbitro;


typedef struct Cliente{
    Jogador jogador;
    Jogo jogo;
}Cliente;


typedef struct Jogo{
    char nome[TAM];
    Jogador jogador;
}Jogo;




