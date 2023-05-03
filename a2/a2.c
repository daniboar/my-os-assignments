#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"

int main()
{
    init();
    pid_t P2, P3, P4, P5, P6, P7, P8, P9;
    info(BEGIN, 1, 0);

    //ramura cu p2,p5,p6,p9
    P2 = fork(); //proces p2
    if(P2 == 0)
    {
        info(BEGIN, 2, 0);
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
