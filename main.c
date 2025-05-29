#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

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

struct stat info;

int main(void)
{
    char input[100];
    char cwd[1024];
    char *array[MAX_TOKENS];
    char *ptrInput;

    while (1)
    {
        getcwd(cwd, sizeof(cwd)); // Obtenir le répertoire courant
        printf("%s > ", cwd);
        fgets(input, sizeof(input), stdin); // Lire l'input utilisateur

        input[strcspn(input, "\n")] = 0;

        int i = 0;
        ptrInput = strtok(input, " ");
        while (ptrInput != NULL && i < MAX_TOKENS)
        {
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
    printf("votre chemin est : %s\n", cwd); // Affiche le répertoire courant
}

void ls(char **args)
{
    DIR *mydir;
    struct dirent *myfile;

    mydir = opendir(".");
    if (mydir == NULL) // Si null, afficher une erreur
    {
        perror("Erreur lors de l'ouverture du répertoire");
        return;
    }

    while ((myfile = readdir(mydir)) != NULL)
    {

        if (strcmp(args[1], "-a") == 0) // Si l'argument est "-a", afficher tous les fichiers (. et .. inclus)
        {
            printf("%s\n", myfile->d_name);
        }
        else if (strcmp(args[1], "-l") == 0) // Si l'argument est "-l", afficher les détails des fichiers sauf les "." et ".."
        {
            if (stat(myfile->d_name, &info) == 0 && strcmp(myfile->d_name, ".") != 0 && strcmp(myfile->d_name, "..") != 0) // Obtenir les informations du fichier
            {
                struct passwd *pw = getpwuid(info.st_uid);
                struct group *gr = getgrgid(info.st_gid);
                printf("%ld %s %s %-8ld %s\n", (long)info.st_nlink, pw->pw_name, gr->gr_name, info.st_size, myfile->d_name); // Afficher les infos du fichier
            }
        }
        else if (strcmp(args[1], "-la") == 0) // Si l'argument est "-la", afficher les détails des fichiers
        {
            if (stat(myfile->d_name, &info) == 0) // Obtenir les informations du fichier
            {
                struct passwd *pw = getpwuid(info.st_uid);
                struct group *gr = getgrgid(info.st_gid);
                printf("%ld %s %s %-8ld %s\n", (long)info.st_nlink, pw->pw_name, gr->gr_name, info.st_size, myfile->d_name); // Afficher les infos du fichier
            }
        }
        else // si seulement ls (ou autres arguments)
        {
            if (strcmp(myfile->d_name, ".") != 0 && strcmp(myfile->d_name, "..") != 0)
            {
                printf("%s\n", myfile->d_name);
            }
        }
    }
    closedir(mydir);
}

void cd(char **args)
{
    if (args[1] == NULL) // Si aucun argument, afficher un message d'erreur
    {
        fprintf(stderr, "cd: argument requis\n");
        return;
    }

    if (chdir(args[1]) != 0) // Changer de répertoire
    {
        fprintf(stderr, "cd : Aucun fichier ou dossier de ce type\n"); // Afficher une erreur si le répertoire n'existe pas
    }
}

void exit_cmd(char **args)
{
    exit(EXIT_SUCCESS);
}