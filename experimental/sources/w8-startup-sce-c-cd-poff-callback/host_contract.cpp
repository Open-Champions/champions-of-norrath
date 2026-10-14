#include <cstdio>
#include "poff_callback.cpp"
extern "C" int _icmd_bind = -1;
extern "C" unsigned norrath_host_gp = 0x00abcd00u;
static int poff_calls, dintr_result = 1, eintr_calls, order_bad;
extern "C" void PowerOffCB() { ++poff_calls; if (eintr_calls) order_bad = 1; }
extern "C" int DIntr() { if (poff_calls == 0 && _icmd_bind < 0) order_bad = 1; return dintr_result; }
extern "C" int EIntr() { ++eintr_calls; return 0; }
int main() {
    if (sceCdPOffCallback(0x111, 0x222) != 0u || poff_calls != 1 || eintr_calls != 1) return 1;
    if (sceCdPoffCbfunc != 0x111 || sceCdPoffCbdata != 0x222 || sceCdCbPoff_Gp != 0x00abcd00u) return 2;
    _icmd_bind = 0; norrath_host_gp = 0x55;
    if (sceCdPOffCallback(0x333, 0x444) != 0x111u || poff_calls != 1) return 3;
    if (sceCdPoffCbfunc != 0x333 || sceCdPoffCbdata != 0x444 || sceCdCbPoff_Gp != 0x55u) return 4;
    dintr_result = 0;
    _icmd_bind = 5;
    if (sceCdPOffCallback(0, 0) != 0x333u) return 8;
    if (eintr_calls != 2) return 5;   // second call also had dintr_result 1; third must not add
    if (sceCdPoffCbfunc != 0 || sceCdPoffCbdata != 0) return 6;
    if (order_bad) return 7;
    std::puts("sceCdPOffCallback contract passed");
    return 0;
}
