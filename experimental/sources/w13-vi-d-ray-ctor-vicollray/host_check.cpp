// Host check only; not a build input. Fills an object with a pattern, runs the routine and compares every byte.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>

static unsigned char object[0x100];

int main() {
    unsigned char expect[sizeof object];
    std::memset(object, 0x5a, sizeof object);
    std::memset(expect, 0x5a, sizeof expect);
    std::memset(expect, 0, 48);
    std::memset(expect + 56, 0, 12);
    std::memset(expect + 76, 0, 12);
    __9VICollRay(object);
    bool ok = std::memcmp(object, expect, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
