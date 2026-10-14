// Host check only; not an EE build input. Build the source with -Dstrncpy=w8_strncpy so it
// does not collide with the host C library, compile this file separately, and link both.
#include <cstdio>
#include <cstring>
extern "C" char* w8_strncpy(char*, const char*, unsigned);
int main() {
    alignas(16) char source[96];
    alignas(16) char target[160], expect[160];
    unsigned checked = 0;
    for (unsigned s_off = 0; s_off < 9; ++s_off) {
        for (unsigned d_off = 0; d_off < 17; d_off += 8) {
            for (unsigned length = 0; length < 36; length += 1) {
                for (unsigned count = 0; count < 45; ++count) {
                    std::memset(source, 'q', sizeof source);
                    for (unsigned i = 0; i < length; ++i) source[s_off + i] = char(1 + (i * 11) % 250);
                    source[s_off + length] = '\0';
                    std::memset(target, 0x5a, sizeof target);
                    std::memset(expect, 0x5a, sizeof expect);
                    std::strncpy(expect + d_off, source + s_off, count);
                    char* result = w8_strncpy(target + d_off, source + s_off, count);
                    if (result != target + d_off) return 1;
                    if (std::memcmp(target, expect, sizeof target) != 0) return 2;
                    ++checked;
                }
            }
        }
    }
    std::printf("strncpy ok (%u cases)\n", checked);
    return 0;
}
