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

