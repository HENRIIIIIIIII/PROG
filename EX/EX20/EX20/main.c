//-----------------------------------------------------------------------------------//
// Nom du projet 		: Exercice 20
// Nom du fichier 		: EX20.c
// Date de cration 		: 04.04.2024
// Date de modification : 04.04.2024
//
// Auteur 				: Henri Mott
//
// Description          : Cet exercice a pour but de vous familiariser avec la gestion des fichiers textes.
//
// Remarques			: voir donnee de l'preuve            
//----------------------------------------------------------------------------------//

//-- definission pour utiliser la lib math sous VS --// 
#define _USE_MATH_DEFINES

//--- librairie standart ---//
#include <stdint.h>  //lib pour les int8_t
#include <stdio.h>                  // entre - sortie
#include <stdlib.h>					// fonctions systme 
#include <math.h>

//--- librairie perso ---//

FILE *fp1;	// Pointeur that is going to write
int main(void)
{
	// Ouverture du ficier, ou creation
	fp1 = fopen("Ex20.txt", "w");

	// Writing values
	fprintf(fp1, "Exercice 20 Henri Mott\n");
	fprintf(fp1, "1235\n");
	fprintf(fp1, "521.13\n");
	fprintf(fp1,
		"          ___\n"
		"        /`.-'\\\n"
		"   _.-'`     ||  .---.\n"
		" .'        _ ||_/     \\\n"
		"/   _     ( o|| o )     |\n"
		"|  (_)     \\__/        |\n"
		"\\             |       /\n"
		" `.___.'     /__.-._.'\n"
		"   |  |     |\n"
		"   |  |     |\n"
		"   |  |     |\n"
		"  /__|    /__|\n"
		" Jack Sparrow\n"
	);
	
	fprintf(fp1,
		"                               |    |    |                             \n"
		"                              )_)  )_)  )_)                            \n"
		"                             )___))___))___)\\                         \n"
		"                            )____)____)_____)\\\\                       \n"
		"                          _____|____|____|____\\\\\\__                  \n"
		"                 ---------\\                   /---------             \n"
		"                   ^^^^^ ^^^^^^^^^^^^^^^^^^^^^                       \n"
		"                     ^^^^      ^^^^     ^^^    ^^                    \n"
		"                          ^^^^      ^^^                              \n"
		"                    ~~~     ~~~~  ~~~~  ~~~   ~~                     \n"
		"                         ~~~   ~~~~   ~~   ~~                        \n"
	);


	int _fcloseall(void);
}
