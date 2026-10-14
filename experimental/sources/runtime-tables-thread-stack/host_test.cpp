// Host comparison only. Not an EE build input.
#include "thread_stack.cpp"

#include <cstdio>

int main() {
    if (sizeof runtime_tables_init_thread_stack != 1024u) return 1;
    // Starts cleared, as the nobits image starts cleared.
    for (int i = 0; i < 1024; ++i) {
        if (runtime_tables_init_thread_stack[i] != 0) return 2;
    }
    // Writable: the thread initializer hands this address to the kernel.
    runtime_tables_init_thread_stack[0] = 1;
    runtime_tables_init_thread_stack[1023] = 2;
    if (runtime_tables_init_thread_stack[0] != 1) return 3;
    if (runtime_tables_init_thread_stack[1023] != 2) return 4;
    return 0;
}
