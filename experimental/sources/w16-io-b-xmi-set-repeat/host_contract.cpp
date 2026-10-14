#include "set_repeat.cpp"

#include <cstddef>
#include <cstdio>

int main() {
    if (offsetof(VIXmi, repeat) != 52) return 1;
    VIXmi x;
    for (unsigned i = 0; i < sizeof x.before; ++i) x.before[i] = 0x5a;
    x.repeat = 7;
    SetRepeat__4VIXmi(&x, -3);
    if (x.repeat != -3) return 2;
    for (unsigned i = 0; i < sizeof x.before; ++i)
        if (x.before[i] != 0x5a) return 3;
    SetRepeat__4VIXmi(&x, 0);
    if (x.repeat != 0) return 4;
    std::puts("xmi set-repeat passed");
    return 0;
}
