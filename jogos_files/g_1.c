#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initRandom(){
    srand(time(NULL));          //previne a repeticao de numeros entre runs
}

int sortNumber(int menor, int maior){
    int number;
    number = rand()%(maior+(menor+1));          //Gerarmos numeros entre 0 e 100
    return number;
}

void showWelcomeMessage() {
    printf("#########################################################################\n");
    printf("                      Bem vindo ao Random Numbers!!                      \n");
    printf("             -----------------------------------------------             \n");
    printf("  Neste jogo sera apresentado um numero que sera gerado                  \n");
    printf("  aleatoriamente entre 0 e 20.                                           \n");
    printf("  O teu objetivo sera advinhares qual e esse numero,                     \n");
    printf("  Sempre que acertares, ganhas 10 pontos e sera gerado outro numero.     \n");
    printf("                                                                         \n");
    printf("  Quando decidires terminar o jogo, escreve '-1'.                      \n");
    printf("                                                                         \n");
    printf("  Boa sorte jogador, e que a forca esteja contigo!                       \n");
    printf("#########################################################################\n");
}

void RandomNumber(){
    int number, numPlayer;
    int pontuacao=0;
    int isLooping = 1;
    initRandom();

    showWelcomeMessage();

    // Gerar um numero
    number = sortNumber(0, 5);

    // Hipoteses
    printf("\n\nJa foi sorteado aleatoreamente um numero entre 0 e 100!\nTente advinhar-lo.");
    while(isLooping == 1){
        printf("\nDigite um numero: ");
        scanf("%d", &numPlayer);
        fflush(stdin);

        if(numPlayer > -1 && numPlayer < 101){
            if(numPlayer == number){
                printf("\nPARABENS acertou o numero!\t +10 Pontos");
                pontuacao += 10; 
                number = sortNumber(0, 5);
                printf("\n\nFoi sorteado outro numero.\t Boa sorte!");
            }else{
                printf("\nIncorreto!!");
            }
        }else if(numPlayer == -1){
            isLooping = 0;
        }else{
            printf("\nDigite um numero entre 0 e 100!");
            break;
        }
    }

    printf("\n\nPontuacao: %d\n\n", pontuacao);
}


int main(int argc, char** argv) {
    RandomNumber();
    return (EXIT_SUCCESS);
}
