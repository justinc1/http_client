
#include <stdio.h>
#include <stdlib.h>

#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <opal_osv_support.h>
#include <opal_osv_support.c>


int main(int argc, char *argv[])
{
    char* host="127.0.0.1";
    int ii, rep = 1;
    int port = 8000;
    printf("Usage: %s [rep=%d] [host=%s] [port=%d]\n", argv[0], rep, host, port);
    if(argc >= 2)
        rep = atoi(argv[1]);
    if(argc >= 3)
        host = argv[2];
    if(argc >= 4)
        port = atoi(argv[3]);
    printf("Param: host=%s port=%d\n", host, port);
   
    char *argv3[] = {"/zpool.so", "list", NULL};
    char **argv2 = argv3;
    if(argc >= 5) {
        argv2 = (char**) malloc(argc + 1);
        char **prm;
        int pos = 0;
        for (pos = 0, prm = &argv[4]; prm != NULL && *prm != NULL; prm++, pos++) {
            argv2[pos] = *prm;
        }
        argv2[pos] = NULL;        
    }
    for (ii=0; ii<rep; ii++) {
        printf("/* REP %d/%d ***********************************/\n", ii, rep);
        opal_osvrest_run(host, port, argv2); // char **argv
        printf("/* REP %d/%d done ***********************************/\n", ii, rep);
        usleep(20*1000);
    }
    
    printf("Done\n");
    return 0;
}
