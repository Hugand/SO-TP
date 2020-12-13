#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  

int main() 
{ 
    int fd; 
  
   	//FIFO File path
    char * myfifo = "tubo"; 
  
    //Criar o FIFO
    mkfifo(myfifo, 0600);
 	printf("\nFIFO Criado!!\n");
  
    char arr1[80], arr2[80]; 
    while (1) 
    { 
        //Abrir FIFO apenas para escrita
        fd = open(myfifo, O_WRONLY); 
  
        // Escrever o comando
		printf("\nEscreva um comando: ");
        fgets(arr2, 80, stdin); 
  
        // Colocar o noss comando no FIFO e fechá-lo
        write(fd, arr2, strlen(arr2)+1); 
        close(fd); 
  
        // Abrir o FIFO com ReadOnly
        fd = open(myfifo, O_RDONLY); 
  
        // Ler do FIFO
        read(fd, arr1, sizeof(arr1)); 
  
        // Mostra mensagem e fechar FIFO
        printf("User2: %s\n", arr1); 
        close(fd); 
    } 
    return 0; 
} 
