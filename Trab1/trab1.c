#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<signal.h>
#define DELIMS " \t\r\n"
#define MAX 60
#define TRUE 1

struct tabela {
	pid_t pid;
	};
typedef struct tabela Tabela;

Tabela process[MAX];

int i = 0;

void lestring(char s[], int max) // função para ler os comandos e evitar o uso de fflush
{
    int j = 0;
    char letra;

    for (j = 0; j < (max - 1); j++) {
	   letra = fgetc(stdin);

   if ((letra == '\n') && (j == 0)) {
       j = j - 1;
       continue;
   }

   if (letra == '\n')
       break;
   s[j] = letra;
    }

    s[j] = 0;
}
void ctrl_z (int num)
{
	fprintf (stdout,"Nao adianta tentar suspender... minha família de processos está protegida!\n");
	signal(SIGTSTP,SIG_IGN);
}
int main (void)
{
	Tabela bash; // estrutura bash
	bash.pid = getpid (); // estrutura recebe o pid da bash
	
	while (TRUE) {
	if (getpid () == bash.pid) { // somente a fsh executa estre trecho

	printf ("fsh>");
	
	sigset_t mask;
	
	if (i != 0) // se fsh tiver filhos, adciona SIGINT à mascara e bloqueia ctrl-c 
	{
		sigemptyset(&mask);
		sigaddset(&mask, SIGINT);
		sigprocmask(SIG_BLOCK, &mask, NULL);
		signal(SIGTSTP,ctrl_z);
	}
	else // caso nao tenha filhos, remove SIGINT da mascara de bloqueados e libera ctrl-c
	{
		if (sigismember(&mask,SIGINT))
			sigdelset(&mask,SIGINT);
		signal(SIGTSTP,SIG_IGN); // apenas ignora ctrl-z
	}
		
	char command [MAX];
	
	lestring (command,60); // leu os comandos
	
	char* parte = (char*) malloc (1+strlen(command));
	
	parte = strtok(command," "); 
	
		while (parte != NULL) {
			
			if (!strcmp(parte,"cd"))
			{
				parte = strtok (NULL," ");
				int status = chdir (parte);
				if (status == -1)
					perror("Falha ao executar cd\n");
			}
			else if (!strcmp(parte,"pwd"))
			{
			  	char cwd[MAX];
   					if (getcwd(cwd,MAX) != NULL)
       						fprintf(stdout, " %s\n", cwd);
					else
						perror ("Falha no pwd\n");
			}
			else if (!strcmp(parte,"exit"))
			{
			short int aux;
			int status;
			
			while (waitpid((-1), &status, WNOHANG) > 0) {
				}
				
				for (aux = 0;aux < i;aux = aux + 1)
				{
					kill(process[aux].pid,9);
				}
				return 0;
			}
			else if (!strcmp(parte,"waita"))
			{
				
				int status;
				while (waitpid((-1), &status, WNOHANG) > 0) {
				
				printf ("Morto processo\n");
				
				}
			}
			else 
			{
			
			pid_t child;
			
				if ((child = fork()) == -1)
				{
					perror ("Falha em fork\n");
				}
				else if (child != 0) // bash
				{
					process[i].pid = child;
					i = i + 1;
					signal(SIGTSTP,ctrl_z);
				}
				else if (child == 0) // P0
				{
					signal(SIGTSTP,ctrl_z);// processo P0 ignora o sinal SIGTSTP e printa mensagem
					
					if (setsid () == -1)
					{
						perror ("Falha em background\n");
					}
					else if ((child = fork()) == -1)
					{
						perror ("Falha em fork\n");
					}
					else if (child == 0) // P1
					{
					
					signal(SIGTSTP,SIG_IGN);// processo P1 ignora o sinal SIGTSTP
					
						if (setsid () == -1)
						{
							perror ("Falha em background\n");
						}

						execvp(parte,&parte);
						perror ("falha no exec\n");
						return 1;
					}
					else if (child != 0) // P0 
					{
						process[i].pid = child;
						i = i + 1;
						
						if (parte != NULL)
						{
						
						parte = strtok (NULL," ");
						
								while (parte != NULL )
								{
									if (strcmp(parte,"@") == 0)
									{
										parte = strtok (NULL," ");
										
										if ((child = fork()) == -1)
										{
											perror ("Falha em fork\n");
										}
										else if (child == 0) // Pn
										{
										
										signal(SIGTSTP,SIG_IGN);// processo Pn ignora o sinal SIGTSTP
										
											if (setsid () == -1)
											{
												perror ("Falha em background\n");
											}
												execvp(parte,&parte);
												perror ("falha no exec\n");
												return 1;
										}
										else if (child != 0) //p0
										{
											process[i].pid = child;
											i = i + 1;
										}
									}
								parte = strtok (NULL," ");						
								}
						}
					}
				}
			}
			parte = strtok (NULL," ");
			
			if (bash.pid == getpid() ) // caso seja a bash, sai do loop e depois sai para receber comandos
			{
				break;
			}
		}
	}
		if ( (bash.pid != getpid() )) // caso seja o processo p0, dá um wait em todos os filhos e depois morre
		{
		int aux1;
			for (aux1 = 0; aux1 < i; aux1 = aux1 + 1)
			{
				waitpid (process[aux1].pid);
			}
		return 0;
		}
	}
}
