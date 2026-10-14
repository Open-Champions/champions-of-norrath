#include "sif_send_cmd.cpp"

#include <cstdio>

static unsigned got_a, got_zero;
static void *got_b, *got_d, *got_e;
static int got_c, got_f, calls;

extern "C" int _sceSifSendCmd(unsigned a, unsigned zero, void* b, int c, void* d, void* e, int f) {
    got_a = a; got_zero = zero; got_b = b; got_c = c; got_d = d; got_e = e; got_f = f;
    calls += 1;
    return 4242;
}

int main() {
    char b, d, e;
    got_zero = 99;
    if (sceSifSendCmd(0x80000001u, &b, 16, &d, &e, 32) != 4242 || calls != 1) return 1;
    if (got_a != 0x80000001u || got_zero != 0 || got_b != &b || got_c != 16 || got_d != &d || got_e != &e || got_f != 32)
        return 2;
    if (sceSifSendCmd(5, nullptr, -1, nullptr, nullptr, 0) != 4242 || got_a != 5 || got_c != -1 || got_f != 0) return 3;
    std::puts("sif send cmd contract passed");
    return 0;
}
