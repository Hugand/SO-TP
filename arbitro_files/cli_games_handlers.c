#include "cli_games_handlers.h"

void threadSignalHandler(int s) {
    printf("Quiting thread\n");
    pthread_exit(NULL);
}

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
            strcpy(p.nome, grt->cliente->jogador.nome);
            sendResponse(p, "_game_output_", readBuffer, grt->cliente->fifo, sizeof(PEDIDO));
        }

        free(readBuffer);
    } while(*grt->isThreadRunning == TRUE && grt->cliente != NULL);
}

void *gameCommWriteThread(void *arg) {
    GAME_COMM_THRD_DATA *gwt = (GAME_COMM_THRD_DATA *) arg;
    char newLineChar = '\n';

    close(gwt->pipe[0]);
    do {
        if(strcmp(gwt->cliente->jogo.gameCommand, "") != TRUE) {
            printf("=>> %s\n", gwt->cliente->jogo.gameCommand);
            strncat(gwt->cliente->jogo.gameCommand, &newLineChar, sizeof(char));
            write(gwt->pipe[1], gwt->cliente->jogo.gameCommand, strlen(gwt->cliente->jogo.gameCommand));
            memset(gwt->cliente->jogo.gameCommand, 0, strlen(gwt->cliente->jogo.gameCommand));
        }
    } while(*gwt->isThreadRunning == TRUE && gwt->cliente != NULL);
}

void handleClientGameCommunication(Cliente *cliente, int *gameStarted) {
    char writeBuffer[20];
    // pthread_t readThread, writeThread;
    GAME_COMM_THRD_DATA gameReadThreadData, gameWriteThreadData;

    gameReadThreadData.pipe = cliente->jogo.readPipe;
    gameReadThreadData.cliente = cliente;
    gameReadThreadData.isThreadRunning = gameStarted;

    gameWriteThreadData.pipe = cliente->jogo.writePipe;
    gameWriteThreadData.cliente = cliente;
    gameWriteThreadData.isThreadRunning = gameStarted;

    pthread_create(&cliente->jogo.readThread, NULL, &gameCommReadThread, &gameReadThreadData);
    pthread_create(&cliente->jogo.writeThread, NULL, &gameCommWriteThread, &gameWriteThreadData);

    pthread_join(cliente->jogo.readThread, NULL);
    pthread_join(cliente->jogo.writeThread, NULL);

    for(int i = 0; i < 2; i++) {
        close(cliente->jogo.readPipe[i]);
        close(cliente->jogo.writePipe[i]);
    }
}

void initJogo(Cliente* cliente, int *gameStarted, Arbitro *arbitro){
    pid_t pid;      //id do nosso processo
    int exit_status, status;
    PEDIDO p;
    char finalScoreStr[10];

    if (pipe(cliente->jogo.readPipe)==-1 || pipe(cliente->jogo.writePipe)==-1) { 
        fprintf(stderr, "Pipe Failed" ); 
        return; 
    } 

    pid = fork();
    if(pid < 0) {                //Erro no fork
        fprintf(stderr, "Fork failed!");
        return;
    } else if(pid == 0){           //Child process
        close(cliente->jogo.readPipe[0]);
        close(cliente->jogo.writePipe[1]);
        dup2(cliente->jogo.readPipe[1], 1);          //Passar o (1 -> stdout) para o pipe de escrita
        dup2(cliente->jogo.writePipe[0], 0);         //Passar o (stdin -> 0) para o pipe 
        puts("AQUI");
        char *gameDir = arbitro->GAMEDIR;
        char path[100];
        sprintf(path, "%s/%s", gameDir, cliente->jogo.nome);
        // strcpy(gameName, cliente->jogo.nome);
        // strcat(path,gameName);
        puts(path);
        execlp(path, path, NULL);
        puts("ignoire");

    } else {          //Processo Pai
        printf("\nProcesso Pai!! - %d\n", pid);
        cliente->jogo.gamePID = pid;
        handleClientGameCommunication(cliente, gameStarted);
        
        waitpid(cliente->jogo.gamePID, &status, 0); 		//processo pai espera que o filho termine

        if (WIFEXITED(status) && cliente != NULL) {
            exit_status = WEXITSTATUS(status);
            cliente->jogador.pontuacao = exit_status;
            printf("Exit status of the client %s was %d\n", cliente->jogador.nome, exit_status); 
        }
    }

    return;
}
