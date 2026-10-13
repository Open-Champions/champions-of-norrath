// Host check only; not a build input. Fills an object with a pattern, runs the routine and compares every byte.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>

unsigned char _vt$8VISprite[8] __asm__("_vt$8VISprite");
static unsigned char object[0x100];

int main() {
    unsigned char expect[sizeof object];
    std::memset(object, 0x5a, sizeof object);
    std::memset(expect, 0x5a, sizeof expect);
    std::memset(expect, 0, 8);
    std::memset(expect + 8, 0, 40);
    std::memset(expect + 52, 0, 4);
    unsigned f500 = 0x43fa0000u, m1 = 0xffffffffu;
    unsigned vt = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$8VISprite));
    std::memcpy(expect + 48, &f500, 4);
    std::memcpy(expect + 56, &m1, 4);
    std::memcpy(expect + 60, &vt, 4);
    __8VISprite(object);
    bool ok = std::memcmp(object, expect, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
