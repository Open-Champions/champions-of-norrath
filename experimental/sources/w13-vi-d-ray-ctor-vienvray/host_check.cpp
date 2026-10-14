// Host check only; not a build input. Fills an object with a pattern, runs the routine and compares every byte
// against an expectation built from the observed stores.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>

unsigned char _vt$8VIEnvRay[8] __asm__("_vt$8VIEnvRay");
static unsigned char object[0x100];

int main() {
    unsigned char expect[sizeof object];
    std::memset(object, 0x5a, sizeof object);
    std::memset(expect, 0x5a, sizeof expect);
    std::memset(expect, 0, 36);
    unsigned w15 = 15, w1 = 1, w0 = 0;
    unsigned vt = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$8VIEnvRay));
    std::memcpy(expect + 44, &w15, 4);
    std::memcpy(expect + 48, &w0, 4);
    std::memcpy(expect + 52, &w1, 4);
    std::memcpy(expect + 60, &vt, 4);
    __8VIEnvRay(object);
    bool ok = std::memcmp(object, expect, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
