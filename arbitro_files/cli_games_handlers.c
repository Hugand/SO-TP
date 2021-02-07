#include "cli_games_handlers.h"


void *gameCommReadThread(void *arg) {
    GAME_COMM_THRD_DATA *grt = (GAME_COMM_THRD_DATA *) arg;
    char buffer;
    char *readBuffer;
    PEDIDO p;
    int n = 0;
    printf("RT JOGO CLIENTE %s pipes %d %d  %p\n", grt->cliente->fifo, grt->pipe[0], grt->pipe[1], grt->pipe);

    close(grt->pipe[1]);
    do {
        readBuffer = malloc(sizeof(char) * 3000);
        memset(readBuffer,0,strlen(readBuffer));
        
        n = 0;
        while(read(grt->pipe[0], &buffer, 1) > 0 && buffer != 36){
            readBuffer[n] = buffer;
            n++;
        }

        strcpy(p.nome, grt->cliente->jogador.nome);
        sendResponse(p, "_game_output_", readBuffer, grt->cliente->fifo, sizeof(PEDIDO));
        free(readBuffer);
    } while(1);
}

void *gameCommWriteThread(void *arg) {
    GAME_COMM_THRD_DATA *gwt = (GAME_COMM_THRD_DATA *) arg;
    char newLineChar = '\n';
    printf("WT JOGO CLIENTE %s pipes %d %d  %p\n", gwt->cliente->fifo, gwt->pipe[0], gwt->pipe[1], gwt->pipe);

    close(gwt->pipe[0]);
    do {
        if(strcmp(gwt->cliente->jogo.gameCommand, "") != TRUE) {
            strncat(gwt->cliente->jogo.gameCommand, &newLineChar, sizeof(char));
            write(gwt->pipe[1], gwt->cliente->jogo.gameCommand, strlen(gwt->cliente->jogo.gameCommand));
            memset(gwt->cliente->jogo.gameCommand, 0, strlen(gwt->cliente->jogo.gameCommand));
        }
    } while(1);
}

void handleClientGameCommunication(int readPipe[2], int writePipe[2], Cliente *cliente) {
    char writeBuffer[20];
    pthread_t readThread, writeThread;
    GAME_COMM_THRD_DATA gameReadThreadData, gameWriteThreadData;

    gameReadThreadData.pipe = readPipe;
    gameReadThreadData.cliente = cliente;

    gameWriteThreadData.pipe = writePipe;
    gameWriteThreadData.cliente = cliente;

    printf("BEFORE T JOGO CLIENTE %s pipes %d %d  %d %d  %p\n", cliente->fifo, readPipe[0], readPipe[1], writePipe[0], writePipe[1], readPipe);

    pthread_create(&readThread, NULL, &gameCommReadThread, &gameReadThreadData);
    pthread_create(&writeThread, NULL, &gameCommWriteThread, &gameWriteThreadData);

    pthread_join(readThread, NULL);
    pthread_join(writeThread, NULL);

    close(readPipe[0]);
}

void initJogo(Cliente* cliente){
    int readPipe[2], writePipe[2];         //Guarda os file descriptors
    pid_t pid;      //id do nosso processo
    pid_t childPid;

    // signal(SIGALRM, despertar);

    if (pipe(readPipe)==-1 || pipe(writePipe)==-1) { 
        fprintf(stderr, "Pipe Failed" ); 
        return; 
    } 

    pid = fork();
    if(pid < 0) {                //Erro no fork
        fprintf(stderr, "Fork failed!");
        return;
    } else if(pid == 0){           //Child process
        childPid = getpid();

        close(readPipe[0]);
        close(writePipe[1]);
        dup2(readPipe[1], 1);          //Passar o (1 -> stdout) para o pipe de escrita
        dup2(writePipe[0], 0);         //Passar o (stdin -> 0) para o pipe 

        execlp(cliente->jogo.nome, cliente->jogo.nome, NULL);

    } else {          //Processo Pai
        printf("\nProcesso Pai!!\n");
        handleClientGameCommunication(readPipe, writePipe, cliente);
        
        int status;

        waitpid(pid, &status, 0); 		//processo pai espera que o filho termine

        if ( WIFEXITED(status) ) {
            int exit_status = WEXITSTATUS(status);
            printf("Exit status of the child was %d\n", exit_status); 
        }
    }

    return;
}
