#ifndef UTILS_H
#define UTILS_H
#define FIFO_SRV "pipe_tubo"
#define FIFO_CLI "pipe_cli%s"


#define TRUE 0
#define FALSE 1
// Inicializa o gerador de numeros aleatorios.
// Esta funcao deve ser chamada apenas uma vez no inicio da execucao do programa
void initRandom();

//Devolve um valor inteiro aleatorio distribuido uniformemente entre [a, b]
int intUniformRnd(int a, int b);

// Devolve o valor 1 com probabilidade prob. Caso contrario, devolve 0
int probEvento(float prob);

typedef struct{
    char nome[20];
    char comando[20];
} PEDIDO;

typedef struct{
    char nome[20];
    char code[20]; // Response code ex: _connection_failed_
    char desc[100]; // Response description ex. "_max_players_"
} RESPONSE;


#endif /* UTILS_H */