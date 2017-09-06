
#include <stdio.h>
#include <stdlib.h>

#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <opal_osv_support.h>
#include <opal_osv_support.c>

double time_delta(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) + double(end.tv_usec - start.tv_usec)/1e6;
}

int main(int argc, char *argv[])
{
    const char* host="127.0.0.1";
    int ii, rep = 1;
    int port = 8000;
    char def_url[] = "/index.html";
    char *url = def_url;
    int loglevel = 3; // 0-no log, 1-error, 2-info, 3-debug
    struct timeval tt0, tt1, tt2, tt3;
    double dur_03, rate_03; // include connect, close
    double dur_12, rate_12; // exclude connect, close

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
    gettimeofday(&tt0, NULL);
    http_client_t httpc = http_connect(host, port, HTTP_VERSION_11);
    gettimeofday(&tt1, NULL);
    for (ii=0; ii<rep; ii++) {
        LOGI("/* REP %d/%d ***********************************/\n", ii, rep);
        http_get(httpc, url);
        http_read(httpc, buf, sizeof(buf));
        LOGI("/* REP %d/%d done ***********************************/\n", ii, rep);
    }
    gettimeofday(&tt2, NULL);
    http_close(&httpc);
    gettimeofday(&tt3, NULL);

    dur_03 = time_delta(tt0, tt3);
    dur_12 = time_delta(tt1, tt2);
    rate_03 = rep / dur_03;
    rate_12 = rep / dur_12;
    
    printf("Stat: nreq=%d, %0.0f %0.0f [req/s], time interval %0.3f %0.3f [sec]\n", rep, rate_03, rate_12, dur_03, dur_12);
    printf("Done\n");
    return 0;
}
