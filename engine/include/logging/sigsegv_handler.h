#pragma once

#include <cstdio>
#include <execinfo.h>
#include <cstdlib>
#include <unistd.h>

void sigsegvHandler(int sig) {
    void *array[15];
    size_t size;
    size = backtrace(array, 15);
    fprintf(stderr, "FATAL ERROR - signal %d", sig);
    std::cout << " - thread " << std::this_thread::get_id() << "\n";
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
