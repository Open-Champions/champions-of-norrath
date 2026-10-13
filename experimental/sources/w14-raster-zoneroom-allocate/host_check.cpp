// Host check only; not a build input.
#include "allocate.cpp"
#include <cstdio>
static unsigned want, calls, result;
extern "C" unsigned __builtin_vec_new(unsigned size) { want = size; ++calls; return result; }
int main() {
    unsigned room[32] = {};
    room[28] = 0;  room[29] = 0x77;
    bool ok = Allocate__10VIZoneRoom(reinterpret_cast<unsigned char*>(room)) == 0 && calls == 0 && room[29] == 0x77;  // zero size: no allocation
    room[28] = 244; result = 0x4000;
    ok = ok && Allocate__10VIZoneRoom(reinterpret_cast<unsigned char*>(room)) == 0 && calls == 1 && want == 244 && room[29] == 0x4000;
    result = 0;
    ok = ok && Allocate__10VIZoneRoom(reinterpret_cast<unsigned char*>(room)) == -1 && calls == 2 && room[29] == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
