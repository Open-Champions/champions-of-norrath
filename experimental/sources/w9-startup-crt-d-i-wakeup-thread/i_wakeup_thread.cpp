// Observed iWakeupThread at 0x002d5f90, 148 bytes. It issues the system call
// numbered -47 (the original loads the number into v1 and executes syscall; the
// result register is the current thread id) and compares that id with its
// argument.
//   - Different: it returns _iWakeupThread (0x002d5020) of the argument.
//   - Equal, but the id is 256 or more, or the word topId (0x00444958) is zero:
//     return -1 and queue nothing.
//   - Equal otherwise: the word topSema (0x008de0c0) is read; the cursor word at
//     topArg+4 (topArg is 0x008de0c8) is masked with 511 to an index; the cursor
//     is then stored back as index + 1 (not the unmasked cursor + 1); the byte
//     pair {0, id} is stored at topArg + 8 + 2*index; iSignalSema is called with
//     the semaphore; the id is returned.
// The stored cursor therefore wraps to 1, never 0, after index 511.
// The system call is isolated in one macro so a host check can replace it.
// Unknown: what system call -47 does beyond returning the id of the thread it
// is made from, and why the interrupt-side form uses a negative number.
#ifndef W9_CURRENT_THREAD_ID
#define W9_CURRENT_THREAD_ID(thread) w9_syscall_minus_47(thread)
#endif
extern "C" {
extern int topId;
extern int topSema;
extern unsigned char topArg[] __attribute__((aligned(8)));
int _iWakeupThread(int thread);
int iSignalSema(int semaphore);

#ifndef W9_HOST_CHECK
static inline int w9_syscall_minus_47(int argument) {
    register int number asm("$3") = -47;
    register int first asm("$4") = argument;
    register int result asm("$2");
    asm volatile("syscall" : "=r"(result) : "r"(number), "r"(first) : "memory");
    return result;
}
#endif

int iWakeupThread(int thread) {
    const int current = W9_CURRENT_THREAD_ID(thread);
    if (current != thread) return _iWakeupThread(thread);
    if (static_cast<unsigned>(current) >= 256u || topId == 0) return -1;
    const int semaphore = topSema;
    int* const cursor = reinterpret_cast<int*>(topArg + 4);
    const int index = *cursor & 0x1ff;
    *cursor = index + 1;
    unsigned char* const entry = topArg + index * 2;
    entry[8] = 0;
    entry[9] = static_cast<unsigned char>(current);
    iSignalSema(semaphore);
    return current;
}
}
