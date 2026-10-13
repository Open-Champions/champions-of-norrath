// Host check only; not a build input. The routine must leave the object bytes unchanged.
#include "onkeyup.cpp"
#include <cstdio>
#include <cstring>

int main() {
    unsigned char object[64], copy[64];
    std::memset(object, 0x5a, sizeof object);
    std::memcpy(copy, object, sizeof object);
    OnKeyUp__9VIWndDnasi(object, 13);
    const bool ok = std::memcmp(object, copy, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
