// Host check only; not a build input.
#include "clear.cpp"
#include <cstdio>
#include <cstring>
static int arrayCalls, arrayFail, deletes;
static unsigned long long arraysSeen[4];
static unsigned deleted;
extern "C" int Clear__t7VIArray1Zi(unsigned char* a) { if (arrayCalls < 4) arraysSeen[arrayCalls] = reinterpret_cast<unsigned long long>(a); ++arrayCalls; return arrayCalls == arrayFail ? -1 : 0; }
extern "C" void __builtin_vec_delete(unsigned b) { ++deletes; deleted = b; }
int main() {
    int w[32];
    for (int i = 0; i < 32; ++i) w[i] = 0x11111111 * (i % 7 + 1);
    int before[32]; std::memcpy(before, w, sizeof w);
    unsigned char* self = reinterpret_cast<unsigned char*>(w);
    bool ok = true;
    arrayFail = 1; arrayCalls = 0;
    ok = ok && Clear__10VIZoneRoom(self) == -1 && arrayCalls == 1 && deletes == 0 && std::memcmp(w, before, sizeof w) == 0;
    arrayFail = 2; arrayCalls = 0;
    ok = ok && Clear__10VIZoneRoom(self) == -1 && arrayCalls == 2 && deletes == 0 && std::memcmp(w, before, sizeof w) == 0;
    arrayFail = 0; arrayCalls = 0; w[29] = 0x1234;
    ok = ok && Clear__10VIZoneRoom(self) == 0 && arrayCalls == 2 && deletes == 1 && deleted == 0x1234;
    ok = ok && arraysSeen[0] == reinterpret_cast<unsigned long long>(self + 60) && arraysSeen[1] == reinterpret_cast<unsigned long long>(self + 68);
    const int zero[] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 19, 20, 21, 22, 23, 24, 28, 29};
    const int neg[] = {7, 11, 13, 14, 25, 26, 27};
    bool touched[32] = {};
    for (int i : zero) { ok = ok && w[i] == 0; touched[i] = true; }
    for (int i : neg) { ok = ok && w[i] == -1; touched[i] = true; }
    for (int i = 0; i < 32; ++i) if (!touched[i]) ok = ok && w[i] == before[i];
    deletes = 0; w[29] = 0; arrayCalls = 0;
    ok = ok && Clear__10VIZoneRoom(self) == 0 && deletes == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
