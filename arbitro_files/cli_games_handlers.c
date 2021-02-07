#include "cli_games_handlers.h"


void *gameCommReadThread(void *arg) {
    GAME_COMM_THRD_DATA *grt = (GAME_COMM_THRD_DATA *) arg;
    char buffer;
    char *readBuffer;
    PEDIDO p;
    int n = 0;
    printf("RT JOGO CLIENTE %s pipes %d %d  %d\n", grt->cliente->fifo, grt->pipe[0], grt->pipe[1], *grt->isThreadRunning);

    close(grt->pipe[1]);
    do {
        readBuffer = malloc(sizeof(char) * 3000);
        memset(readBuffer,0,strlen(readBuffer));
        
        n = 0;
        while(read(grt->pipe[0], &buffer, 1) > 0 && buffer != '$' && n < 3000){
            readBuffer[n] = buffer;
            n++;
        }
        
        if(strcmp(readBuffer, "") != TRUE) {
            printf("========\n");
            strcpy(p.nome, grt->cliente->jogador.nome);
            sendResponse(p, "_game_output_", readBuffer, grt->cliente->fifo, sizeof(PEDIDO));
        }

        free(readBuffer);
    } while(*grt->isThreadRunning == TRUE);

    printf("QUITING READ THREAD %s\n", grt->cliente->jogador.nome);

    close(grt->pipe[0]);
}

void *gameCommWriteThread(void *arg) {
    GAME_COMM_THRD_DATA *gwt = (GAME_COMM_THRD_DATA *) arg;
    char newLineChar = '\n';

    printf("WRITING THREAD\n");

    close(gwt->pipe[0]);
    do {
        if(strcmp(gwt->cliente->jogo.gameCommand, "") != TRUE) {
            printf("=>> %s\n", gwt->cliente->jogo.gameCommand);
            strncat(gwt->cliente->jogo.gameCommand, &newLineChar, sizeof(char));
            write(gwt->pipe[1], gwt->cliente->jogo.gameCommand, strlen(gwt->cliente->jogo.gameCommand));
            memset(gwt->cliente->jogo.gameCommand, 0, strlen(gwt->cliente->jogo.gameCommand));
        }
    } while(*gwt->isThreadRunning == TRUE);
    close(gwt->pipe[1]);
}

void handleClientGameCommunication(int readPipe[2], int writePipe[2], Cliente *cliente, int *gameStarted) {
    char writeBuffer[20];
    pthread_t readThread, writeThread;
    GAME_COMM_THRD_DATA gameReadThreadData, gameWriteThreadData;

    gameReadThreadData.pipe = readPipe;
    gameReadThreadData.cliente = cliente;
    gameReadThreadData.isThreadRunning = gameStarted;

    gameWriteThreadData.pipe = writePipe;
    gameWriteThreadData.cliente = cliente;
    gameWriteThreadData.isThreadRunning = gameStarted;

    pthread_create(&readThread, NULL, &gameCommReadThread, &gameReadThreadData);
    pthread_create(&writeThread, NULL, &gameCommWriteThread, &gameWriteThreadData);

    pthread_join(readThread, NULL);
    pthread_join(writeThread, NULL);
}

void initJogo(Cliente* cliente, int *gameStarted){
    int readPipe[2], writePipe[2];         //Guarda os file descriptors
    pid_t pid;      //id do nosso processo
    int exit_status, status;
    PEDIDO p;
    char finalScoreStr[10];

    if (pipe(readPipe)==-1 || pipe(writePipe)==-1) { 
        fprintf(stderr, "Pipe Failed" ); 
        return; 
    } 

    pid = fork();
    if(pid < 0) {                //Erro no fork
        fprintf(stderr, "Fork failed!");
        return;
    } else if(pid == 0){           //Child process
        close(readPipe[0]);
        close(writePipe[1]);
        dup2(readPipe[1], 1);          //Passar o (1 -> stdout) para o pipe de escrita
        dup2(writePipe[0], 0);         //Passar o (stdin -> 0) para o pipe 

        execlp(cliente->jogo.nome, cliente->jogo.nome, NULL);
    } else {          //Processo Pai
        printf("\nProcesso Pai!! - %d\n", pid);
        cliente->jogo.gamePID = pid;
        handleClientGameCommunication(readPipe, writePipe, cliente, gameStarted);
        
        waitpid(pid, &status, 0); 		//processo pai espera que o filho termine

        if (WIFEXITED(status)) {
            exit_status = WEXITSTATUS(status);
            cliente->jogador.pontuacao = exit_status;
            strcpy(p.nome, cliente->jogador.nome);
            sprintf(finalScoreStr, "%d", exit_status);
            printf("Exit status of the client %s was %d\n", cliente->jogador.nome, exit_status); 
            sendResponse(p, "_final_score_", finalScoreStr, cliente->fifo, sizeof(PEDIDO));
        }
    }


    return;
}
