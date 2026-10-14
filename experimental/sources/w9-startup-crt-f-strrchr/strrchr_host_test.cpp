// Host check only; not an EE build input. Build the source with -Dstrrchr=w9_strrchr
// so it does not collide with the host C library, compile this file separately,
// and link both.
#include <cstdio>
#include <initializer_list>
#include <cstring>
extern "C" char* w9_strrchr(const char*, int);
int main() {
    char buf[64];
    unsigned checked = 0;
    for (unsigned len = 0; len < 30; ++len) {
        for (unsigned mask = 0; mask < 64; ++mask) {
            std::memset(buf, 'x', sizeof buf);
            for (unsigned i = 0; i < len; ++i) buf[i] = (mask >> (i % 6)) & 1 ? '/' : char('a' + i % 5);
            buf[len] = '\0';
            for (int c : {int('/'), 0, int('a'), int('e'), int('q'), 0x12f, -31}) {
                const char* got = w9_strrchr(buf, c);
                const char* want = std::strrchr(buf, int(static_cast<signed char>(c)));
                if (got != want) return 1;
                ++checked;
            }
        }
    }
    char high[] = {'\xe1', 'a', '\xe1', '\0'};
    if (w9_strrchr(high, -31) != high + 2) return 2;
    if (w9_strrchr(high, 0xe1) != high + 2) return 3;
    std::printf("strrchr ok (%u cases)\n", checked);
    return 0;
}
