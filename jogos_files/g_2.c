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
    printf("-> %s\n", word);
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


    printf("Welcome puta\n");

    do {
        randomPos = intUniformRnd(0, WORD_DICT_SIZE - 1);
        strcpy(randomWord, wordDict[randomPos]);
        scrambleWord(randomWord);

        printf("\nNOVA PALAVRA!!!\n");
        printf("Que palavra é esta? -> %s\n", randomWord);
        isLoopingWordInput = TRUE;
        do {
            printf("#> ");
            // scanf("%s", wordInput);
            gets(wordInput);

            if(strcmp(wordInput, "sair") == TRUE) {
                isLoopingWordInput = FALSE;
                printf("A sair. Vá tchau!\n");
                return 0;
            } else if(strcmp(wordInput, wordDict[randomPos]) == TRUE) {
                score++;
                isLoopingWordInput = FALSE;
                printf("Correct word!!!\n");
                printf("SCORE: %d\n", score);   
            } else {
                printf("The word you entered is wrong! - %s\n", wordInput);
            }

        } while (isLoopingWordInput == TRUE);

    } while (isLoopingNewWord == TRUE);

    return 0;
}