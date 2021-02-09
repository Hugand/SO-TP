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
    printf("                      Bem vindo ao Random Numbers!!                      \n");
    printf("             -----------------------------------------------             \n");
    printf("  Neste jogo sera apresentado um numero que sera gerado                  \n");
    printf("  aleatoriamente entre 0 e 20.                                           \n");
    printf("  O teu objetivo sera advinhares qual e esse numero,                     \n");
    printf("  Sempre que acertares, ganhas 10 pontos e sera gerado outro numero.     \n");
    printf("                                                                         \n");
    printf("  Quando decidires terminar o jogo, escreve '-1'.                        \n");
    printf("                                                                         \n");
    printf("  Boa sorte jogador, e que a forca esteja contigo!                       \n");
    printf("#########################################################################\n");
}

void RandomNumber(){
    int number, numPlayer;
    char buffer[50];
    int isLooping = 1;
    initRandom();
    setbuf(stdout, NULL);

    showWelcomeMessage();

    printf("<%d> \n", getpid());
    signal(SIGUSR1, trataSinal);	//Quando se receber um sinal
					//SIGUSR1 ele executa o trataSinal
    // Gerar um numero
    number = sortNumber(1, 20);

    // Hipoteses
    printf("\n\nJa foi sorteado aleatoreamente um numero entre 0 e 100!\nTente advinhar-lo.");
    while(isLooping == 1){
        printf("\nDigite um numero de 0 a 20 - %d:\n$ ", number);
        // scanf("%d\n", &numPlayer);
        fgets(buffer, 50, stdin);
        numPlayer = atoi(buffer);

        if(numPlayer > 0 && numPlayer < 21){
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
            printf("\nDigite um numero entre 1 e 20!");
        }
    }

    printf("\n\nPontuacao: %d\n\n", pontuacao);
}


int main(int argc, char** argv) {
    RandomNumber();
    return (EXIT_SUCCESS);
}
