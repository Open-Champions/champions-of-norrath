// Host check only; not a build input. The routine ignores both arguments: no memory it can be
// handed changes, for a range of key values.
#include "onkeyup.cpp"
#include <cstdio>
#include <cstring>

int main() {
    unsigned char object[64], copy[64];
    std::memset(object, 0xa5, sizeof object);
    std::memcpy(copy, object, sizeof object);
    for (int key = -5; key < 300; ++key) OnKeyUp__11VIWndSplashi(object, key);
    const bool ok = std::memcmp(object, copy, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
