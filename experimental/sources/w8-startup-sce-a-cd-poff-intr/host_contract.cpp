#define NORRATH_HOST_CONTRACT 1
#include "cd_poff_intr.cpp"

#include <cstdio>

int Init_seq = 0;
void (*sceCdPoffCbfunc)(void*) = nullptr;
unsigned sceCdCbPoff_Gp = 0x1234;
void* sceCdPoffCbdata = nullptr;

static unsigned host_gp = 0x9999;
static unsigned gp_in_callback;
static int calls;
static void* arg_seen;
extern "C" unsigned norrath_host_swap_gp(unsigned value) {
    const unsigned previous = host_gp;
    host_gp = value;
    return previous;
}
static void callback(void* argument) {
    ++calls;
    arg_seen = argument;
    gp_in_callback = host_gp;
}

int main() {
    int token;
    sceCdPoffCbdata = &token;
    if (calls != 0) return 1;
    _sceCd_Poff_Intr(nullptr, nullptr);                 // no callback registered
    if (calls != 0 || host_gp != 0x9999) return 2;
    sceCdPoffCbfunc = callback;
    Init_seq = 1;
    _sceCd_Poff_Intr(nullptr, nullptr);                 // initialization in progress
    if (calls != 0) return 3;
    Init_seq = 0;
    _sceCd_Poff_Intr(nullptr, nullptr);
    if (calls != 1 || arg_seen != &token) return 4;
    if (gp_in_callback != 0x1234 || host_gp != 0x9999) return 5;
    std::puts("cd poff intr contract passed");
    return 0;
}
