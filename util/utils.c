//
// Created by root on 3/3/19.
//


#include <stdint.h>
#include "utils.h"

void exit_with_error(const char *msg) {
    perror(msg);
    exit(0);
}


int safe_sock_read(int __fd, void *__buf, size_t __nbytes) {
    uint8_t sz;
    if (read(__fd, (void *) &sz, 1) > 0) {
        __nbytes = sz < __nbytes ? sz : __nbytes;
        if (read(__fd, __buf, __nbytes) > 0) {
            LOG(("DEBUG: Message received: %s\n", (char *) __buf));
            return 1;
        }
    }
    close(__fd);
    return 0;
}

void safe_sock_write(int __fd, void *__buf, size_t __nbytes) {
    size_t tmp = strnlen((char *) __buf, NET_BUF_SZ);
    __nbytes = tmp < __nbytes ? tmp : __nbytes;
    uint8_t sz = (uint8_t) __nbytes;
    if (write(__fd, (void *) &sz, 1) > 0)
        if (write(__fd, __buf, __nbytes) > 0) {
            LOG(("DEBUG: Message sent: %s\n", (char *) __buf));
            return;
        }
    close(__fd);
    exit_with_error("ERROR writing to socket");
}
