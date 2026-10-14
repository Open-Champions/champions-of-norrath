// Host check, not a plan input.
#include <cstdio>
#include "wait_get_address.cpp"
static int which, a_ret = 11, n_ret = 22; static void* seen[3]; static int ints[2];
extern "C" int WaitGetAddressAutoUpIf(void* o, void* b, int* s, int t, int f) { which = 1; seen[0] = o; seen[1] = b; seen[2] = s; ints[0] = t; ints[1] = f; return a_ret; }
extern "C" int WaitGetAddressNoAutoUpIf(void* o, void* b, int* s, int t, int f) { which = 2; seen[0] = o; seen[1] = b; seen[2] = s; ints[0] = t; ints[1] = f; return n_ret; }
int main() {
    char o, b; int s;
    if (sceLibnetWaitGetAddress(&o, &b, &s, 5, 6, 2) != 11 || which != 1 || seen[0] != &o || seen[1] != &b || seen[2] != &s || ints[0] != 5 || ints[1] != 6) return 1;
    if (sceLibnetWaitGetAddress(&o, &b, &s, 7, 8, 0) != 22 || which != 2 || ints[0] != 7 || ints[1] != 8) return 2;
    if (sceLibnetWaitGetAddress(&o, &b, &s, 7, 8, 0xfffffffdu) != 22) return 3;
    if (sceLibnetWaitGetAddress(&o, &b, &s, 7, 8, 3) != 11) return 4;
    std::puts("wait get address check passed");
    return 0;
}
