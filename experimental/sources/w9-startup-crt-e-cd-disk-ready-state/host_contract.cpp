#include <cstdio>
#include "disk_ready_state.cpp"
int main() {
    static_assert(sizeof _sceCd_rcmdrdata == 0x100, "symbol size");
    static_assert(sizeof cddr == 0x40, "span to _dr_mode");
    for (unsigned char b : _sceCd_rcmdrdata) if (b != 0) return 1;
    if (_dr_mode != 0) return 2;
    if (reinterpret_cast<const void*>(&_dr_mode) == static_cast<const void*>(cddr)) return 3;
    std::puts("disk ready state contract passed");
    return 0;
}
