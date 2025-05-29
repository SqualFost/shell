#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_TOKENS 20

typedef struct
{
    const char *nom;
    void (*fonction)(char **args);
} Commandes;

void pwd(char **args);
void ls(char **args);
void exit_cmd(char **args);
void cd(char **args);

Commandes commandes[] = {
    {"pwd", pwd},
    {"ls", ls},
    {"cd", cd},
    {"exit", exit_cmd},
    {NULL, NULL} // Fin de la liste des commandes
};

int main(void)
{
    char input[100];
    char cwd[1024];
    char *array[MAX_TOKENS]; // Tableau de pointeurs sur char
    char *ptrInput;
    DIR *mydir;
    struct dirent *myfile;

    while (1)
    {
        getcwd(cwd, sizeof(cwd)); // Obtenir le répertoire courant
        printf("%s > ", cwd);
        fgets(input, sizeof(input), stdin); // Lire l'input utilisateur

        input[strcspn(input, "\n")] = 0;

        if (array == NULL)
        {
            perror("Pas d'allocation mémoire");
            exit(EXIT_FAILURE);
        }

        int i = 0;
        ptrInput = strtok(input, " ");
        while (ptrInput != NULL && i < MAX_TOKENS)
        {
            printf("%s\n", ptrInput);
            array[i++] = ptrInput;
            ptrInput = strtok(NULL, " ");
        }

        for (int i = 0; commandes[i].nom != NULL; i++)
        {
            if (strcmp(array[0], commandes[i].nom) == 0)
            {
                commandes[i].fonction(array);
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}

void pwd(char **args)
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("votre chemin est : %s\n", cwd);
}

void ls(char **args)
{
    DIR *mydir;
    struct dirent *myfile;

    mydir = opendir(".");
    if (mydir == NULL)
    {
        perror("Erreur lors de l'ouverture du répertoire");
        return;
    }

    while ((myfile = readdir(mydir)) != NULL)
    {
        printf("%s\n", myfile->d_name);
    }
    closedir(mydir);
}

void cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "cd: argument requis\n");
        return;
    }

    if (chdir(args[1]) != 0)
    {
        fprintf(stderr, "cd : Aucun fichier ou dossier de ce type\n");
    }
}

void exit_cmd(char **args)
{
    exit(EXIT_SUCCESS);
}