
#include <stdio.h>
#include <stdlib.h>

#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <opal_osv_support.h>
#include <opal_osv_support.c>


int main(int argc, char *argv[])
{
    const char* host="127.0.0.1";
    int ii, rep = 1;
    int port = 8000;
    char def_url[] = "/index.html";
    char *url = def_url;
    int loglevel = 3; // 0-no log, 1-error, 2-info, 3-debug

    printf("Usage: %s [rep=%d] [host=%s] [port=%d] [url=%s] [loglevel=%d]\n", argv[0], rep, host, port, url, loglevel);
    if(argc >= 2)
        rep = atoi(argv[1]);
    if(argc >= 3)
        host = argv[2];
    if(argc >= 4)
        port = atoi(argv[3]);
    if(argc >= 5)
        url = (argv[4]);
    if(argc >= 6)
        loglevel = atoi(argv[5]);
    printf("Param: host=%s port=%d url=%s loglevel=%d\n", host, port, url, loglevel);
    http_loglevel = loglevel;
   
    char buf[4096];
    http_client_t httpc = http_connect(host, port, HTTP_VERSION_11);
    for (ii=0; ii<rep; ii++) {
        LOGI("/* REP %d/%d ***********************************/\n", ii, rep);
        http_get(httpc, url);
        http_read(httpc, buf, sizeof(buf));
        LOGI("/* REP %d/%d done ***********************************/\n", ii, rep);
    }
    http_close(&httpc);
    
    printf("Done\n");
    return 0;
}
