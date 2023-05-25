#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define resp_pipe "RESP_PIPE_28180"
#define req_pipe "REQ_PIPE_28180"

int main(void){
    char msg[7] = "HELLO$\0";

    if(mkfifo(resp_pipe, 0600) != 0){
        printf("ERROR\ncannot create the response pipe\n");
        return 1;
    }

    int req = open(req_pipe, O_RDONLY);
    if(req == -1){
        printf("ERROR\ncannot open the request pipe\n");
        return 1;
    }

    int resp = open(resp_pipe, O_WRONLY);
    if(resp == -1){
        printf("ERROR\ncannot open the request pipe\n");
        return 1;
    }

    write(resp, &msg, sizeof(msg));
    printf("SUCCESS\n");

    char msg1[6] = "ECHO$\0";
    char msg2[9] = "VARIANT$\0";
    unsigned int n = 28180;     

    write(resp, &msg1, sizeof(msg1));
    write(resp, &n, sizeof(unsigned int));
    write(resp, &msg2, sizeof(msg2)); 

    close(req);
    close(resp);

    return 0;
}