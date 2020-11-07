#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
void initRandom(){
    srand(time(NULL));          //previne a repeticao de numeros entre runs
}

int sortNumber(){
    int number, maior=100, menor=0;
    number = rand()%(maior+(menor+1));          //Gerarmos numeros entre 0 e 100
    return number;
}


void RandomNumber(){
    int number, numPlayer;
    int pontuacao=0;
    initRandom();
    
    printf("Bem vindo ao Jogo: Numeros Aleatorios!\n");
    printf("Sumario do jogo:\n \tVamos ter a nossa maquina a gerar um numero entre 0 e 100\n\t"
            "e o teu objetivo e acertares nesse numero.\n\t"
            "Quanto mais cedo acertares mais pontuacao teras!!!"
            "");
/*
/*
    
    /*Gerar um numero*/
/*
    number = sortNumber();
    
    /*Hipoteses*/
/*
    printf("\n\nJa foi sorteado aleatoreamente um numero entre 0 e 100!\nTente advinhar-lo.");
    while(1){
        printf("\nDigite um numero: ");
        scanf("%d", &numPlayer);

        if(numPlayer > -1 && numPlayer < 101){
            if(numPlayer == number){
                printf("\nPARABENS acertou o numero!\t +10 Pontos");
                pontuacao += 10; 
                number = sortNumber();
                printf("\n\nFoi sorteado outro numero.\t Boa sorte!");
            }else{
                printf("\nIncorreto!!");
            }
        }else{
            printf("\nDigite um numero entre 0 e 100!");
            break;
        }
    }
    
    printf("\n\nPontuacao: %d\n\n", pontuacao);
}
*/
