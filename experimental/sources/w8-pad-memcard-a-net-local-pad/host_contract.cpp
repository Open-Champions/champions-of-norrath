#include "net_local_pad.cpp"

#include <cstdio>

int main() {
    if (sizeof netLocalPad != 352) return 1;
    for (unsigned i = 0; i < sizeof netLocalPad; ++i) if (netLocalPad[i] != 0) return 2;
    std::puts("net local pad passed");
    return 0;
}
