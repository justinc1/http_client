
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
#define DBG_WARMUP 1
#if DBG_WARMUP
    struct timeval tt1b;
    double dur_1b2, rate_1b2;
#endif

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
#if DBG_WARMUP
        http_loglevel = 3;
        // warm up connection
        LOGI("/* REP warm-up ***********************************/\n");
        http_get(httpc, url);
        http_read(httpc, buf, sizeof(buf));
        LOGI("/* REP warm-up done ***********************************/\n");
        gettimeofday(&tt1b, NULL);
        http_loglevel = loglevel;
#endif
    int nreq_10 = rep / 10;
    if (nreq_10 <= 0) {
        nreq_10 = nreq_10 + 100; // never/only-once switch to debug print
    }

    for (ii=0; ii<rep; ii++) {
        if ((ii % nreq_10) == 0) {
            http_loglevel = 3;
        }
        LOGI("/* REP %d/%d ***********************************/\n", ii, rep);
        http_get(httpc, url);
        http_read(httpc, buf, sizeof(buf));
        LOGI("/* REP %d/%d done ***********************************/\n", ii, rep);
        http_loglevel = loglevel;
    }
    gettimeofday(&tt2, NULL);
    http_close(&httpc);
    gettimeofday(&tt3, NULL);

    dur_03 = time_delta(tt0, tt3);
    dur_12 = time_delta(tt1, tt2);
    rate_03 = rep / dur_03;
    rate_12 = rep / dur_12;
    printf("Stat: nreq=%d, %0.0f %0.0f [req/s], time interval %0.3f %0.3f [sec]\n", rep, rate_03, rate_12, dur_03, dur_12);
#if DBG_WARMUP
    dur_1b2 = time_delta(tt1b, tt2);
    rate_1b2 = rep / dur_1b2;
    printf("Stat: nreq=%d, %0.0f %0.0f [req/s], time interval %0.3f %0.3f [sec] warm-up excluded\n", rep, rate_03, rate_1b2, dur_03, dur_1b2);
#endif
    printf("Done\n");
    return 0;
}
