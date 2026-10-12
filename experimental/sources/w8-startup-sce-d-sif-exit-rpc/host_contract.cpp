#include "sif_exit_rpc.cpp"

#include <cstdio>

static int order;
static int exit_order;
static int flag_when_called;

extern "C" void sceSifExitCmd() {
    order += 1;
    exit_order = order;
    flag_when_called = sceSifInitCheck;
}

int main() {
    sceSifInitCheck = 1;
    sceSifExitRpc();
    if (exit_order != 1) return 1;
    if (flag_when_called != 1) return 2; /* the word is cleared after the call, not before */
    if (sceSifInitCheck != 0) return 3;
    sceSifInitCheck = 5;
    sceSifExitRpc();
    if (order != 2 || sceSifInitCheck != 0) return 4;
    std::puts("sif exit rpc contract passed");
    return 0;
}
