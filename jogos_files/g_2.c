/*
    Scrambled words
*/

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils.h"
#include <unistd.h>

#define MAX_WORD_TAM 20
#define WORD_DICT_SIZE 21
int pontuacao=0;

void trataSinal(int sinal){
	exit(pontuacao);
}

void showWelcomeMessage() {
    printf("#########################################################################\n");
    printf("                      Bem vindo ao scrambled words!!                     \n");
    printf("             -----------------------------------------------             \n");
    printf("  Neste jogo serão apresentadas varias palavras, uma de cada vez, cujas  \n");
    printf("  letras estao baralhadas.  O teu objetivo e descobrir a palavra certa.  \n");
    printf("  e  ganhar  o maior numero de pontos possivel. Sempre que acertes numa  \n");
    printf("  palavra, ganhas 10 pontos e uma nova e te apresentada.                 \n");
    printf("                                                                         \n");
    printf("  Quando decidires terminar o jogo, escreve 'sair'.                      \n");
    printf("                                                                         \n");
    printf("  Boa sorte jogador, e que a forca esteja contigo!                       \n");
    printf("#########################################################################\n");
}

void scrambleWord(char word[MAX_WORD_TAM]) {
    int wordSize = strlen(word);
    int currentPos, randomNextPos;
    char tmpChar;

    currentPos = intUniformRnd(0, wordSize - 1);

    for(int i = 0; i < wordSize; i++) {
        randomNextPos = intUniformRnd(0, wordSize - 1);
        tmpChar = word[currentPos];
        word[currentPos] = word[randomNextPos];
        word[randomNextPos] = tmpChar;
        currentPos = intUniformRnd(0, wordSize - 1);
    }
}

int main(int argv, char *arg[]) {
    if(argv == 2) sleep(3);
    initRandom();
    setbuf(stdout, NULL);
    char wordDict[WORD_DICT_SIZE][MAX_WORD_TAM] = { 
        "carro", "animal", "tigre",
        "suposto","lisboa", "coimbra",
        "natal", "inverno", "neve",
        "computador", "festival", "porto",
        "aveiro", "lapis", "peixe",
        "vaca", "vassoura", "livro",
        "caderno", "caneta", "sebenta"
    };
    int isLoopingWordInput = TRUE;
    int isLoopingNewWord = TRUE;
    char wordInput[MAX_WORD_TAM];
    int randomPos;
    char randomWord[MAX_WORD_TAM];


    showWelcomeMessage();

    printf("<%d> \n", getpid());
    signal(SIGUSR1, trataSinal);	//Quando se receber um sinal
					//SIGUSR1 ele executa o trataSinal

    do {
        randomPos = intUniformRnd(0, WORD_DICT_SIZE - 1);
        strcpy(randomWord, wordDict[randomPos]);
        scrambleWord(randomWord);

        printf("\nNOVA PALAVRA!!!\n");
        isLoopingWordInput = TRUE;

        do {
            printf("Que palavra é esta? -> %s  --  %s\n", randomWord, wordDict[randomPos] );
            printf("$ ");
            fflush(stdin);
            scanf("%s", wordInput);

            if(strcmp(wordInput, "sair") == TRUE) {
                isLoopingWordInput = FALSE;
                printf("\nA tua pontuação foi %d. Ate a proxima!\n", pontuacao);
                exit(pontuacao);
            } else if(strcmp(wordInput, wordDict[randomPos]) == TRUE) {
                pontuacao += 10;
                isLoopingWordInput = FALSE;
                printf("PALAVRA CORRETA!!!\n");
                printf("SCORE: %d\n", pontuacao);   
            } else {
                printf("\nPalavra incorreta...\n");
            }

        } while (isLoopingWordInput == TRUE);

    } while (isLoopingNewWord == TRUE);

    return 0;
}
