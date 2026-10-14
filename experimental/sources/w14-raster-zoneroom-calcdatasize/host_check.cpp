// Host check only; not a build input.
#include "calc.cpp"
#include <cstdio>
int main() {
    unsigned char room[128] = {};
    int* w = reinterpret_cast<int*>(room);
    w[20] = 5; w[22] = 7; w[23] = 99; w[24] = 99; w[28] = 99;
    bool ok = CalcDataSize__10VIZoneRoom(room) == 0 && w[23] == 0 && w[24] == 160 && w[28] == 160 + 84 && w[20] == 5 && w[22] == 7 && w[21] == 0;
    w[20] = 0; w[22] = 0;
    ok = ok && CalcDataSize__10VIZoneRoom(room) == 0 && w[24] == 0 && w[28] == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
