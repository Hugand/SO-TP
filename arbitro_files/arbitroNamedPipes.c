#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/wait.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>


pid_t childPid;

void despertar(int sinal){
	kill(childPid, SIGUSR1);
}

// Driver code
int main(void)
{
	int fd1; 
  
    // FIFO file path 
    char * myfifo = "tubo"; 
  
    // Creating the named file(FIFO) 
    // mkfifo(<pathname>,<permission>) 
    mkfifo(myfifo, 0600); 
  
    char str1[80], str2[80]; 
    while (1) 
    { 
        // First open in read only and read 
        fd1 = open(myfifo,O_RDONLY); 
        read(fd1, str1, 80); 

    
		switch (str1){
		    case "players":
				puts("Comando Players");
		        break;
		    case "games":
				puts("Comando Games");
		        break;
		    case "k":
				puts("Comando K");
		        break;
		    case "exit":
				puts("Comando Exit");
		        break;
		    default:
		}
  
        // Print the read string and close 
        //printf("User1: %s\n", str1); 
        close(fd1); 
  
        // Now open in write mode and write 
        // string taken from user. 
        fd1 = open(myfifo,O_WRONLY); 
        fgets(str2, 80, stdin); 
        write(fd1, str2, strlen(str2)+1); 
        close(fd1); 
    } 

    signal(SIGALRM, despertar);
    pid_t pid = fork();
	
	
	//Child Process
	childPid = pid;
    if ( pid == 0 )
    {
       /* The pathname of the file passed to execl()
          is not defined   */
	childPid = getpid();
        execl("g_2.o", "g_2.o", NULL);
    }
    alarm(100);
    int status;

    waitpid(pid, &status, 0); 		//processo pai espera que o filho termine

    if ( WIFEXITED(status) )
    {
        int exit_status = WEXITSTATUS(status);
        printf("Exit status of the child was %d\n",
                                     exit_status); 
    }
    return 0; 
}

