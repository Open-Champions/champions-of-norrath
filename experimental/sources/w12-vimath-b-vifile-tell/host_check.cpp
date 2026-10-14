// Host check only; not a build input. Build with the host C++ compiler. Checks that the 64-bit word at offset 16 is
// returned whole (including the upper half and the sign) and that the object is untouched.
#include "tell.cpp"
#include <cstdio>
#include <cstring>

int main() {
    bool ok = true;
    const long long values[] = {0, 1, -1, 0x123456789abcdefLL, -0x123456789abcdefLL, 0x80000000LL, 0x7fffffffffffffffLL};
    for (long long v : values) {
        alignas(8) unsigned char object[64];
        std::memset(object, 0x5a, sizeof object);
        std::memcpy(object + 16, &v, 8);
        unsigned char before[64];
        std::memcpy(before, object, sizeof object);
        ok = ok && Tell__6VIFile(object) == v && std::memcmp(before, object, sizeof object) == 0;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
