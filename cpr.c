/*------------------------------------------------------------
Fichier: cpr.c

Noms: Sabrina Seto et Joseph Daher
Numeros d'etudiants: 300359255, 300361164
	  

Description: Ce programme contient le code pour la creation
             d'un processus enfant et y attacher un tuyau.
	     L'enfant envoyera des messages par le tuyau
	     qui seront ensuite envoyes a la sortie standard.

Explication du processus zombie
(point 5 de "A completer" dans le devoir):

	(s.v.p. completez cette partie);

-------------------------------------------------------------*/
#include <stdio.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>

/* Prototype */
void creerEnfantEtLire(int );

/*-------------------------------------------------------------
Function: main
Arguments: 
	int ac	- nombre d'arguments de la commande
	char **av - tableau de pointeurs aux arguments de commande
Description:
	Extrait le nombre de processus a creer de la ligne de
	commande. Si une erreur a lieu, le processus termine.
	Appel creerEnfantEtLire pour creer un enfant, et lire
	les donnees de l'enfant.
-------------------------------------------------------------*/

int main(int ac, char **av)
{
    int numeroProcessus; 

    if(ac == 2)
    {
       if(sscanf(av[1],"%d",&numeroProcessus) == 1)
       {
           creerEnfantEtLire(numeroProcessus);
       }
       else fprintf(stderr,"Ne peut pas traduire argument\n");
    }
    else fprintf(stderr,"Arguments pas valide\n");
    return(0);
}


/*-------------------------------------------------------------
Function: creerEnfantEtLire
Arguments: 
	int prcNum - le numero de processus
Description:
	Cree l'enfant, en y passant prcNum-1. Utilise prcNum
	comme identificateur de ce processus. Aussi, lit les
	messages du bout de lecture du tuyau et l'envoie a 
	la sortie standard (df 1). Lorsqu'aucune donnee peut
	etre lue du tuyau, termine.
-------------------------------------------------------------*/

void creerEnfantEtLire(int prcNum)
{

    /* S.V.P. completez cette fonction selon les
       instructions du devoirs. */

	int p[2], pid;
	char prcNumStr[12];

	// base case
	if (prcNum == 1) {
		printf("Processus 1 commence\n");
		fflush(stdout);
		sleep(5);
		printf("Processus 1 termine\n");
		fflush(stdout);
		sleep(10);
		exit(0);
	}

	else {
		pipe(p);
		pid = fork();
		if (pid == 0){
			close(p[0]); // ferme le bout de lecture du tuyau

			dup2(p[1], STDOUT_FILENO); // redirige stdout  à tuyau write 
			close(p[1]); // ferme write dans enfant

			//execvp("./cpr", ("cpr", (prcNum - 1) , NULL )); // execute cpr avec prcNum-1
			sprintf(prcNumStr, "%d", prcNum - 1);
			char *args[] = {"./cpr", prcNumStr, NULL};
			execvp(args[0], args); // execute cpr avec prcNum-1
			
			perror("execvp failed");
			exit(1);
		}
		else if (pid > 0){
			close(p[1]); // ferme write dans parent


			printf("Processus %d commence\n", prcNum);
			fflush(stdout);

			ssize_t data;
			char buffer[256];

			while((data=read(p[0], buffer, sizeof(buffer))) > 0) {
				write(STDOUT_FILENO, buffer, data);
			}
			close(p[0]); 
			
			printf("Processus %d termine\n", prcNum);
			fflush(stdout);

			sleep(10);
			exit(0);

		}
		else{
			perror("fork a failli");
			exit(1);
		}

	}

}
