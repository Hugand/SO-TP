#include <stdio.h>
#include <stdlib.h>
#include "../general.h"

int charToInt(char c){
	int num = 0;

	//Substract '0' from entered char to get
	//corresponding digit
	num = c - '0';
	return num;
}

void main(){
    char *maxplayerBuff;
    printf("ARBITRO\n");
    printf("GAMEDIR => %s\n", getenv("GAMEDIR"));
    printf("MAXPLAYER => %s\n", getenv("MAXPLAYER"));

    Arbitro arbitro;
    if((arbitro.GAMEDIR = getenv("GAMEDIR")) == NULL) {
        printf("GAMEDIR NULLL\n");
    }else {
        printf("%s\n", arbitro.GAMEDIR);
    }

    if((maxplayerBuff = getenv("MAXPLAYER")) == NULL) {
        printf("MAXPLAYERS NULLL\n");
    } else {
        arbitro.MAXPLAYERS = atoi(maxplayerBuff);
        printf("%d\n", arbitro.MAXPLAYERS);
    }

}