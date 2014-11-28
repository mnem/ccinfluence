#pragma once

#ifdef XASSERT
#error You forgot to remove this file. Tsk.
#endif

#include <cstdlib>
#include <cstdio>

#define XASSERT(test, message)                                                  \
    do {                                                                        \
        if (!(test)) {                                                          \
            setvbuf(stdout, NULL, _IONBF, 0);                                   \
            printf("Assert failed: %s:%d - %s\n", __FILE__, __LINE__, message); \
            exit(EXIT_FAILURE);                                                 \
        }                                                                       \
    } while (0)
