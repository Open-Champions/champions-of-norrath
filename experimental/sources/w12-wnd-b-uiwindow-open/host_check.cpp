// Host check only; not a build input. +40 changes only when the flag is true, +44 always changes,
// and no other byte moves.
#include "open.cpp"
#include <cstdio>
#include <cstring>

int main() {
    static unsigned char object[96];
    bool ok = true;
    for (int flag = 0; flag < 2; ++flag) {
        std::memset(object, 0x11, sizeof object);
        Open__10VIUIWindowb(object, flag != 0);
        unsigned a, b;
        std::memcpy(&a, object + 40, 4);
        std::memcpy(&b, object + 44, 4);
        if (a != (flag ? 0x3f800000u : 0x11111111u) || b != 0x3f800000u) ok = false;
        for (unsigned i = 0; i < sizeof object; ++i)
            if ((i < 40 || i >= 48) && object[i] != 0x11) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
