#include "request_functions.h"

/*
    Send a RESPONSE to a client with a given code and desc
*/
void sendResponse(PEDIDO p, char *code, char* desc, char *fifo, int n) {
    int fdr;
    RESPONSE resp;
    
    if(n == sizeof(PEDIDO)) {
        sprintf(fifo, FIFO_CLI, p.nome);
        fdr = open(fifo, O_WRONLY);
        strcpy(resp.nome, p.nome);
        strcpy(resp.code, code);
        strcpy(resp.desc, desc);
        n = write(fdr, &resp, sizeof(RESPONSE));
        close(fdr);
    }
}