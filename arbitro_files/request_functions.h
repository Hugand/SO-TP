#include <stdio.h>
#include "../utils.h"
#include "../general.h"
#include <string.h> 
#include <unistd.h>
#include <fcntl.h>


#ifndef REQUEST_FUNCTIONS_H
#define REQUEST_FUNCTIONS_H

/*
    Send a RESPONSE to a client with a given code and desc
*/
void sendResponse(PEDIDO p, char *code, char* desc, char *fifo, int n);

#endif