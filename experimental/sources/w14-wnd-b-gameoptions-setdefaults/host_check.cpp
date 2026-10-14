// Host check only; not a build input.
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <vector>
#include "setdefaults.cpp"

// provide the memset the unit imports (declaration matches libc)
int main() {
    std::vector<unsigned char> o(1024, 0xcc);
    SetDefaults__13VIGameOptions(o.data());
    std::vector<unsigned char> e(1024, 0);
    const float one = 1.0f;
    std::memcpy(&e[8], &one, 4); std::memcpy(&e[12], &one, 4);
    e[5] = 1; e[151] = 1;
    const unsigned m = 0x01234567u; std::memcpy(&e[1020], &m, 4);
    const bool ok = o == e;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
