#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int pontuacao=0;

void trataSinal(int sinal){
	exit(pontuacao);
}


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
    printf("                      Bem vindo ao Math Game!!                           \n");
    printf("             -----------------------------------------------             \n");
    printf("  Neste jogo serao apresentados dois numeros que seram gerados           \n");
    printf("  aleatoriamente entre 0 e 100.                                          \n");
    printf("  O teu objetivo sera advinhares qual e a soma desses numeros,           \n");
    printf("  Sempre que acertares, ganhas 10 pontos e seram gerados outros numeros. \n");
    printf("                                                                         \n");
    printf("  Boa sorte jogador, e que a forca esteja contigo!                       \n");
    printf("#########################################################################\n");
}

void MathGame(){
    int number1, number2, numPlayer;
    int isLooping = 1;
    int soma=0;
    initRandom();
    setbuf(stdout, NULL);

    showWelcomeMessage();

    printf("<%d> \n", getpid());
    signal(SIGUSR1, trataSinal);	//Quando se receber um sinal
					//SIGUSR1 ele executa o trataSinal
    // Gerar um numero
    number1 = sortNumber(0, 100);
    number2 = sortNumber(0, 100);


    // Hipoteses
    printf("\n\nJa foram sorteados aleatoreamente 2 numeros entre 0 e 100!\nTente advinhar a soma deles!");
    while(isLooping == 1){
        soma = number1 + number2;
        printf("\nOperacao -> %d + %d = ...\nDigite a Soma - %d:\n$ ", number1,number2, soma);
        scanf("%d", &numPlayer);
        // fflush(stdin);

        if(numPlayer > -1){
            if(numPlayer == soma){
                printf("\nPARABENS acertou!\t +10 Pontos");
                pontuacao += 10;
                number1 = sortNumber(0, 100);
                number2 = sortNumber(0, 100);
                printf("\n\nForam sorteados outros numeros.\t Boa sorte!");
            }else{
                printf("\nIncorreto!!");
            }
        }else if(numPlayer == -1){
            isLooping = 0;
        }else{
            printf("\nDigite um numero entre 0 e 100!");
            //break;
        }
    }

    printf("\n\nPontuacao: %d\n\n", pontuacao);
}


int main(int argc, char** argv) {
    MathGame();
    return (EXIT_SUCCESS);
}
