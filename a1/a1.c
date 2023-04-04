#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


void list_it(const char *path, const char *name, const char *size)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("ERROR\ninvalid directory path");
        return;
    }
    printf("SUCCESS\n");
    if(name != NULL && size == NULL)
    {
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                if(strncmp(entry->d_name, name, strlen(name)) == 0)
                {
                    snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                    if(lstat(fullPath, &statbuf) == 0)
                    {
                        printf("%s\n", fullPath);
                    }
                }
            }
        }
    }
    else if(name == NULL && size != NULL)
    {
        long size_el = atol(size);
        //printf("%ld", size_el);
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                if(lstat(fullPath, &statbuf) == 0)
                {
                    if(S_ISREG(statbuf.st_mode) && statbuf.st_size < size_el)
                    {
                        printf("%s\n", fullPath);
                    }
                }
            }
        }
    }
    else
    {
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                if(lstat(fullPath, &statbuf) == 0)
                {
                    printf("%s\n", fullPath);
                }
            }
        }
    }
    closedir(dir);
}

void list_rec(const char *path, const char *name, const char *size)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("ERROR\ninvalid directory path");
        return;
    }

    if(name != NULL && size == 0)
    {
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                if(strstr(entry->d_name,name ) == entry->d_name)
                {
                    snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                    if(lstat(fullPath, &statbuf) == 0)
                        printf("%s\n", fullPath);
                    else
                        list_rec(fullPath, name,0);
                }
            }
        }
    }
    else if(name == NULL && size != NULL)
    {
        long size_el = atol(size);
        //printf("%ld", size_el);
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                if(lstat(fullPath, &statbuf) == 0)
                {
                    if(S_ISREG(statbuf.st_mode) && statbuf.st_size < size_el)
                        printf("%s\n", fullPath);
                    if(S_ISDIR(statbuf.st_mode))
                        list_rec(fullPath, 0, size);
                }
            }
        }
    }
    else
    {
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                if(lstat(fullPath, &statbuf) == 0)
                {
                    printf("%s\n", fullPath);
                    if(S_ISDIR(statbuf.st_mode))
                    {
                        list_rec(fullPath, 0, 0);
                    }
                }
            }
        }
    }
    closedir(dir);
}

int main(int argc, char **argv)
{
    if(argc >= 2)
    {
        if(strcmp(argv[1], "variant") == 0) //prima cerinta cu afisarea variantei
        {
            printf("28180\n");
        }
        if(strcmp(argv[1], "list") == 0) // daca vrem sa listam din director
        {
            if(strstr(argv[2], "path=") - argv[2] == 0 ) //daca nu e recursiv si nu avem filtering_option
                list_it(argv[2]+5, 0, 0);
            else if(strstr(argv[2], "name_starts_with=") != NULL) // daca nu e recursiv si avem "name starts with.."
            {
                if(strstr(argv[3], "path=") - argv[3] == 0 )
                {
                    list_it(argv[3]+5, argv[2]+17, 0);
                }
            }
            else if(strstr(argv[2], "size_smaller=") != NULL) // daca nu e recursiv si avem "size smaller"
            {
                if(strstr(argv[3], "path=") - argv[3] == 0 )
                {
                    list_it(argv[3]+5, 0, argv[2]+13);
                }
            }
            if(strcmp(argv[2], "recursive") == 0) // daca vrem listare recursiva doar si recursive este al 3 lea argument
            {
                if(strstr(argv[3], "path=") - argv[3] == 0 )
                {
                    printf("SUCCESS\n");
                    list_rec(argv[3]+5, 0, 0);
                }
                else if(strstr(argv[3], "name_starts_with=") != NULL) // daca e recursiv si avem "name starts with.."
                {
                    if(strstr(argv[4], "path=") - argv[4] == 0 )
                    {
                        printf("SUCCESS\n");
                        list_rec(argv[4]+5, argv[3]+17, 0);
                    }
                }
                else if(strstr(argv[3], "size_smaller=") != NULL) // daca e recursiv si avem "size smaller"
                {
                    if(strstr(argv[4], "path=") - argv[4] == 0 )
                    {
                        printf("SUCCESS\n");
                        list_rec(argv[4]+5, 0, argv[3]+13);
                    }
                }

            }
            else if(strstr(argv[2], "name_starts_with=") != NULL)  // daca avem "/list name_start_with recursive..."
            {
                if(strcmp(argv[3], "recursive") == 0)
                {
                    if(strstr(argv[4], "path=") - argv[4] == 0)
                    {
                        printf("SUCCESS\n");
                        list_rec(argv[4]+5, argv[2]+17, 0);

                    }
                }
            }
            else if(strstr(argv[2], "size_smaller=") != NULL)  // daca avem "/list size_smaller recursive..."
            {
                if(strcmp(argv[3], "recursive") == 0)
                {
                    if(strstr(argv[4], "path=") - argv[4] == 0)
                    {
                        printf("SUCCESS\n");
                        list_rec(argv[4]+5, 0, argv[2]+13);
                    }
                }
            }

        }
    }
    return 0;
}

