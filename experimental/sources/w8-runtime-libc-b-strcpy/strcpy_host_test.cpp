// Host check only; not an EE build input. Build the source with -Dstrcpy=w8_strcpy so it
// does not collide with the host C library, compile this file separately, and link both.
#include <cstdio>
#include <cstring>
extern "C" char* w8_strcpy(char*, const char*);
int main() {
    alignas(16) char source[96];
    alignas(16) char target[128];
    unsigned checked = 0;
    for (unsigned s_off = 0; s_off < 17; s_off += 1) {
        for (unsigned d_off = 0; d_off < 17; d_off += 4) {
            for (unsigned length = 0; length < 40; ++length) {
                std::memset(source, 'q', sizeof source);
                for (unsigned i = 0; i < length; ++i) source[s_off + i] = char(1 + (i * 7) % 250);
                source[s_off + length] = '\0';
                source[s_off + length + 1] = 'z';
                std::memset(target, 0x5a, sizeof target);
                char* result = w8_strcpy(target + d_off, source + s_off);
                if (result != target + d_off) return 1;
                if (std::memcmp(target + d_off, source + s_off, length + 1) != 0) return 2;
                if (target[d_off + length + 1] != 0x5a) return 3;  // nothing past the NUL
                if (d_off && target[d_off - 1] != 0x5a) return 4;
                ++checked;
            }
        }
    }
    std::printf("strcpy ok (%u cases)\n", checked);
    return 0;
}
