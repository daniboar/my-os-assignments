#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


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

void parse(const char* path)
{
    char MAGIC; // magic = 8
    short HEADER_SIZE;
    int VERSION; // version = [36,128]
    char NO_OF_SECTION; // [4,19]

    char SECT_NAME[13];
    short SECT_TYPE; // 26 sau 98
    int SECT_OFFSET;
    int SECT_SIZE;

    int fd = open(path, O_RDONLY);
    if(fd == -1)
    {
        perror("Could not open inpun file");
        return;
    }

    read(fd, &MAGIC, sizeof(MAGIC)); //citim magic si verificam daca a fost citit bine
    if(MAGIC != '8')
    {
        printf("ERROR\nwrong magic\n");
        close(fd);
        return;
    }

    read(fd, &HEADER_SIZE, sizeof(HEADER_SIZE)); //citim header size

    read(fd, &VERSION, sizeof(VERSION)); //citim version si verificam daca a fost citit bine
    if(VERSION < 36 || VERSION > 128)
    {
        printf("ERROR\nwrong version\n");
        close(fd);
        return;
    }

    read(fd, &NO_OF_SECTION, sizeof(NO_OF_SECTION));
    if(NO_OF_SECTION < 4 || NO_OF_SECTION > 19)
    {
        printf("ERROR\nwrong sect_nr");
        close(fd);
        return;
    }

    char name[19][13]; //declaram name,offset, type si size pentru a retine fiecare valoare din cele no_of_section
    short type[19];
    int offset[19];
    int size[19];

    for(int i = 0; i < NO_OF_SECTION; i++)
    {
        read(fd, SECT_NAME, 12); // citim numele si il adaugam in matricea de caractere
        SECT_NAME[12] = '\0';
        strcpy(name[i], SECT_NAME);

        read(fd, &SECT_TYPE, sizeof(SECT_TYPE)); // citim tipul si verificam daca a fost citit bine
        if(SECT_TYPE != 26 && SECT_TYPE != 98)
        {
            printf("ERROR\nwrong sect_types\n");
            close(fd);
            return;
        }
        type[i] = SECT_TYPE;

        read(fd, &SECT_OFFSET, sizeof(SECT_OFFSET)); // citesc offset
        offset[i] = SECT_OFFSET;
        if(offset[i]) {}


        read(fd, &SECT_SIZE, sizeof(SECT_SIZE)); // citesc size
        size[i] = SECT_SIZE;

    }

    printf("SUCCESS\n");
    printf("version=%d\n", VERSION);
    printf("nr_sections=%d\n", NO_OF_SECTION);
    for(int i = 0; i < NO_OF_SECTION; i++)
    {
        printf("section%d: %s %hd %d\n", i+1, name[i], type[i], size[i]);
    }

    close(fd);
}

void findall(const char *path)
{
    // DIR *dir = NULL;
    // //struct dirent *entry = NULL;
    // //char fullPath[512];
    // //struct stat statbuf;

    // dir = opendir(path);
    // if(dir == NULL)
    // {
    //     printf("ERROR\ninvalid directory path");
    //     return;
    // }

    // closedir(dir);
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
                    DIR *dir = NULL;
                    dir = opendir(argv[3]+5);
                    if(dir == NULL)
                    {
                        printf("ERROR\ninvalid directory path\n");
                        closedir(dir);
                    }
                    else
                    {
                        printf("SUCCESS\n");
                        list_rec(argv[3]+5, 0, 0);
                        closedir(dir);
                    }

                }
                else if(strstr(argv[3], "name_starts_with=") != NULL) // daca e recursiv si avem "name starts with.."
                {
                    if(strstr(argv[4], "path=") - argv[4] == 0 )
                    {
                        DIR *dir = NULL;
                        dir = opendir(argv[4]+5);
                        if(dir == NULL)
                        {
                            printf("ERROR\ninvalid directory path\n");
                            closedir(dir);
                        }
                        else
                        {
                            printf("SUCCESS\n");
                            list_rec(argv[4]+5, argv[3]+17, 0);
                            closedir(dir);
                        }
                    }
                }
                else if(strstr(argv[3], "size_smaller=") != NULL) // daca e recursiv si avem "size smaller"
                {
                    if(strstr(argv[4], "path=") - argv[4] == 0 )
                    {
                        DIR *dir = NULL;
                        dir = opendir(argv[4]+5);
                        if(dir == NULL)
                        {
                            printf("ERROR\ninvalid directory path\n");
                            closedir(dir);
                        }
                        else
                        {
                            printf("SUCCESS\n");
                            list_rec(argv[4]+5, 0, argv[3]+13);
                            closedir(dir);
                        }
                    }
                }

            }
            else if(strstr(argv[2], "name_starts_with=") != NULL)  // daca avem "/list name_start_with recursive..."
            {
                if(strcmp(argv[3], "recursive") == 0)
                {
                    if(strstr(argv[4], "path=") - argv[4] == 0)
                    {
                        DIR *dir = NULL;
                        dir = opendir(argv[4]+5);
                        if(dir == NULL)
                        {
                            printf("ERROR\ninvalid directory path\n");
                            closedir(dir);
                        }
                        else
                        {
                            printf("SUCCESS\n");
                            list_rec(argv[4]+5, argv[2]+17, 0);
                            closedir(dir);
                        }

                    }
                }
            }
            else if(strstr(argv[2], "size_smaller=") != NULL)  // daca avem "/list size_smaller recursive..."
            {
                if(strcmp(argv[3], "recursive") == 0)
                {
                    if(strstr(argv[4], "path=") - argv[4] == 0)
                    {
                        DIR *dir = NULL;
                        dir = opendir(argv[4]+5);
                        if(dir == NULL)
                        {
                            printf("ERROR\ninvalid directory path\n");
                            closedir(dir);
                        }
                        else
                        {
                            printf("SUCCESS\n");
                            list_rec(argv[4]+5, 0, argv[2]+13);
                            closedir(dir);
                        }
                    }
                }
            }

        }

        if(strcmp(argv[1], "parse") == 0)  // daca vrem sa facem parse
        {
            if(strstr(argv[2], "path=") - argv[2] == 0)
                parse(argv[2]+5);
        }

        if(strcmp(argv[1], "findall") == 0)  // daca vrem sa facem findall
        {
            if(strstr(argv[2], "path=") - argv[2] == 0)
                findall(argv[2]+5);
        }
    }
    return 0;
}

