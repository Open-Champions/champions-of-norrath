// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp (statics must sit below 4 GiB).
#include "findchar.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>

int main() {
    bool ok = true;
    static unsigned char table[12 * 40];
    for (int n : {0, 1, 2, 3, 8, 17, 40}) {
        std::memset(table, 0, sizeof table);
        for (int i = 0; i < n; ++i) {
            const unsigned short code = static_cast<unsigned short>(10 + 3 * i);  // strictly ascending
            std::memcpy(table + 12 * i + 6, &code, 2);
        }
        // Host pointers are 64-bit, so use an arena address that fits the 32-bit table word through a bridge.
        struct Font { unsigned char head[36]; int count; unsigned char pad[24]; unsigned long table; } font;
        std::memset(&font, 0, sizeof font);
        font.count = n;
        font.table = reinterpret_cast<unsigned long>(table);
        // The function reads the table word as 32 bits; skip the check when the host address does not fit.
        if (reinterpret_cast<unsigned long>(table) > 0xffffffffUL) { std::puts("skip (64-bit host address)"); return 0; }
        for (int code = 0; code < 160; ++code) {
            int want = -1;
            for (int i = 0; i < n; ++i) if (10 + 3 * i == code) want = i;
            ok = ok && FindChar__7VIRFontUs(&font, static_cast<unsigned>(code) | 0x70000u) == want;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
