#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

sem_t s3;
int ok3 = 0, ok5 = 0;

void *thread_function2(void *p)
{
    int threadNr = (*(int*)p);
    info(BEGIN, 2, threadNr);
    info(END, 2, threadNr);
    return NULL;
}

void *thread_function3(void *p)
{
    int threadNr = (*(int*)p);
    if(threadNr == 1 || threadNr == 2 || threadNr == 4)
    {
        info(BEGIN, 3, threadNr);
        info(END, 3, threadNr);
    }
    if(threadNr == 3)
    {
        sem_wait(&s3);
        info(BEGIN, 3, threadNr);
        ok3 = 1;
        while(ok5 != 1)  //astept sa inceapa threadul 5 pentru a putea inchide threadul 3
        {
            sem_post(&s3);
            sem_wait(&s3);
        }
        info(END, 3, threadNr);
        sem_post(&s3);
    }
    if(threadNr == 5)
    {
        while(!ok3)  //astept sa inceapa threadul 3
        {
            sem_post(&s3);
            sem_wait(&s3);
        }
        sem_wait(&s3);
        info(BEGIN, 3, threadNr);
        ok5 = 1;
        info(END, 3, threadNr);
        sem_post(&s3);
    }
    return NULL;
}

void *thread_function7(void *p)
{
    int threadNr = (*(int*)p);
    info(BEGIN, 7, threadNr);
    info(END, 7, threadNr);
    return NULL;
}

int main()
{
    init();
    pid_t P2, P3, P4, P5, P6, P7, P8, P9;
    pthread_t tids3[5];
    int params3[5];
    pthread_t tids7[46];
    int params7[46];
    pthread_t tids2[6];
    int params2[6];
    info(BEGIN, 1, 0);

    //ramura cu p2,p5,p6,p9
    P2 = fork(); //proces p2
    if(P2 == 0)
    {
        info(BEGIN, 2, 0);
        for(int i = 0; i < 6; i++)
        {
            params2[i] = i + 1;
            pthread_create(&tids2[i], NULL, thread_function2, &params2[i]);
        }
        for(int i = 0; i < 6 ; i++)
        {
            pthread_join(tids2[i], NULL);
        }

        P5 = fork(); //process p5, copilul lui P2
        if(P5 == 0)
        {
            info(BEGIN, 5, 0);
            info(END, 5, 0);
        }
        else
        {
            P6 = fork();  //proces p6, copilul lui p2, frate cu p5
            if(P6 == 0)
            {
                info(BEGIN, 6, 0);
                info(END, 6, 0);
            }
            else
            {
                P9 = fork(); //proces P9, copilul lui p2, frate cu p5, p6
                if(P9 == 0)
                {
                    info(BEGIN, 9, 0);
                    info(END, 9, 0);
                }
                else
                {
                    waitpid(P5, NULL, 0);  //astept terminarea proceselor
                    waitpid(P6, NULL, 0);
                    waitpid(P9, NULL, 0);
                    info(END, 2, 0);
                }
            }
        }

    }
    else
    {
        //ramura P3, P4
        P3 = fork();
        if(P3 == 0)
        {
            info(BEGIN, 3, 0);
            sem_init(&s3, 0, 1); //initializez semaforul
            for(int i = 0; i < 5; i++)
            {
                params3[i] = i + 1;
                pthread_create(&tids3[i], NULL, thread_function3, &params3[i]);
            }
            for(int i = 0; i < 5 ; i++)
            {
                pthread_join(tids3[i], NULL);
            }
            sem_destroy(&s3);
            P4 = fork(); // proces P4, copilul lui p3
            if(P4 == 0)
            {
                info(BEGIN, 4, 0);
                info(END, 4, 0);
            }
            else
            {
                wait(NULL);
                info(END, 3, 0);
            }
        }
        else
        {
            P7 = fork(); //proces P7
            if(P7 == 0)
            {
                info(BEGIN, 7, 0);
                for(int i = 0; i < 46; i++)
                {
                    params7[i] = i + 1;
                    pthread_create(&tids7[i], NULL, thread_function7, &params7[i]);
                }
                for(int i = 0; i < 46 ; i++)
                {
                    pthread_join(tids7[i], NULL);
                }
                info(END, 7, 0);
            }
            else
            {
                P8 = fork(); //proces p8
                if(P8 == 0)
                {
                    info(BEGIN, 8, 0);
                    info(END, 8, 0);
                }
                else
                {
                    waitpid(P2, NULL, 0); //astept terminarea copiilor lui P1
                    waitpid(P3, NULL, 0);
                    waitpid(P7, NULL, 0);
                    waitpid(P8, NULL, 0);
                    info(END, 1, 0);
                }
            }
        }
    }

    return 0;
}
