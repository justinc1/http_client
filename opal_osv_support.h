#ifndef OPAL_OSV_SUPPORT_H
#define OPAL_OSV_SUPPORT_H

//#include "opal_config.h"

#define HTTP_VERSION_10 10
#define HTTP_VERSION_11 11

struct http_client_t {
    char *host;
    int port;
    int sockfd;
    int version; /* HTTP_VERSION_10 or HTTP_VERSION_11 */
};
typedef struct http_client_t http_client_t;

// Replacement for waitpid.
long osv_waittid(long tid, int *status, int options) __attribute__((weak));
// Replacement for fork+exec
long osv_execve(const char *path, char *const argv[], char *const envp[], long *thread_id, int notification_fd) __attribute__((weak));

int osv_get_all_app_threads(pid_t tid, pid_t** tid_arr, size_t *len) __attribute__((weak));


int opal_is_osv();
pid_t opal_getpid();
int opal_osvrest_run(char *host, int port, char **argv);


#endif
