// Host check only; not an EE build input. Build the source with -Dstrchr=w9_strchr
// so it does not collide with the host C library, compile this file separately,
// and link both.
#include <cstdio>
#include <initializer_list>
#include <cstring>
extern "C" char* w9_strchr(const char*, int);
int main() {
    alignas(16) char buf[128];
    unsigned checked = 0;
    for (unsigned off = 0; off < 17; ++off) {
        for (unsigned len = 0; len < 50; ++len) {
            for (unsigned pos = 0; pos <= len; ++pos) {
                std::memset(buf, 'x', sizeof buf);
                for (unsigned i = 0; i < len; ++i) buf[off + i] = char('b' + i % 7);
                buf[off + len] = '\0';
                buf[off + len + 1] = 'T';  // bytes past the terminator are ignored
                if (pos < len) buf[off + pos] = 'T';
                for (int c : {int('T'), 0, 0x154, int('b'), 0xe1, -31, int('Z')}) {
                    const char* got = w9_strchr(buf + off, c);
                    const char* want = std::strchr(buf + off, int(static_cast<unsigned char>(c)));
                    if (got != want) return 1;
                    ++checked;
                }
            }
        }
    }
    char high[] = {'\xe1', 'a', '\0'};
    if (w9_strchr(high, 0xe1) != high || w9_strchr(high, -31) != high) return 2;
    std::printf("strchr ok (%u cases)\n", checked);
    return 0;
}
