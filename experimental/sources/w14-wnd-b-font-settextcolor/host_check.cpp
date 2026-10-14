// Host check only; not a build input.
#include "settextcolor.cpp"
#include <cstdio>
#include <cstring>

int main() {
    unsigned char dest[32], src[32], copy[32];
    for (int i = 0; i < 32; ++i) { dest[i] = 0xa0 + i; src[i] = i + 1; }
    std::memcpy(copy, dest, sizeof dest);
    SetTextColor__6VIFontRC10VIColor32F(reinterpret_cast<VIColor16*>(dest + 1), reinterpret_cast<const VIColor16*>(src + 3));
    bool ok = std::memcmp(dest + 1, src + 3, 16) == 0 && dest[0] == copy[0] && std::memcmp(dest + 17, copy + 17, 15) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
