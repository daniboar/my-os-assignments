#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


void cerinta_2(const char *path, const char *filename)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    char f[512];

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("ERROR\ninvalid directory path\n");
        return;
    }

    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            FILE *file = fopen(entry->d_name, "r");
            if(file == NULL)
            {
                printf("SUCCES\n");
                return ;

            }
            //closedir(dir);
        }
    }
    closedir(dir);
}

int main(int argc, char **argv)
{
    if(argc >= 2)
    {
        //prima cerinta cu avisarea variantei
        if(strcmp(argv[1], "variant") == 0)
        {
            printf("28180\n");
        }
        else
        {
            cerinta_2(argv[1], argv[2]);
        }
    }
    return 0;
}
