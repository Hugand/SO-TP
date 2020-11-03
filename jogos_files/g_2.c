/*
    Scrambled words
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils.h"

#define MAX_WORD_TAM 20
#define WORD_DICT_SIZE 6

void showWelcomeMessage() {
    printf("#########################################################################\n");
    printf("                      Bem vindo ao scrambled words!!                     \n");
    printf("             -----------------------------------------------             \n");
    printf("  Neste jogo serão apresentadas varias palavras, uma de cada vez, cujas  \n");
    printf("  letras estao baralhadas.  O teu objetivo e descobrir a palavra certa.  \n");
    printf("  e  ganhar  o maior numero de pontos possivel. Sempre que acertes numa  \n");
    printf("  palavra, ganhas 1 ponto e uma nova e te apresentada.                   \n");
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
    }
}

int main() {
    initRandom();

    char wordDict[WORD_DICT_SIZE][MAX_WORD_TAM] = {
        "carro", "animal", "tigre",
        "suposto","lisboa", "coimbra"
    };
    int isLoopingWordInput = TRUE;
    int isLoopingNewWord = TRUE;
    char wordInput[MAX_WORD_TAM];
    int randomPos;
    char randomWord[MAX_WORD_TAM];

    int score = 0;

    showWelcomeMessage();

    do {
        randomPos = intUniformRnd(0, WORD_DICT_SIZE - 1);
        strcpy(randomWord, wordDict[randomPos]);
        scrambleWord(randomWord);

        printf("\nNOVA PALAVRA!!!\n");
        printf("Que palavra é esta? -> %s\n", randomWord);
        isLoopingWordInput = TRUE;

        do {
            printf("#> ");
            gets(wordInput);

            if(strcmp(wordInput, "sair") == TRUE) {
                isLoopingWordInput = FALSE;
                printf("\nA tua pontuação foi %d. Ate a proxima!\n", score);
                exit(score);
            } else if(strcmp(wordInput, wordDict[randomPos]) == TRUE) {
                score++;
                isLoopingWordInput = FALSE;
                printf("PALAVRA CORRETA!!!\n");
                printf("SCORE: %d\n", score);   
            } else {
                printf("Palavra incorreta...\n");
            }

        } while (isLoopingWordInput == TRUE);

    } while (isLoopingNewWord == TRUE);

    return 0;
}