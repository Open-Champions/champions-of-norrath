// Host check only; not an EE build input. Build the source with -Dstrcat=w9_strcat
// so it does not collide with the host C library, compile this file separately,
// and link both.
#include <cstdio>
#include <cstring>
extern "C" char* w9_strcat(char*, const char*);
int main() {
    alignas(16) char dst[160];
    alignas(16) char src[96];
    unsigned checked = 0;
    for (unsigned d_off = 0; d_off < 17; ++d_off) {
        for (unsigned d_len = 0; d_len < 40; d_len += 3) {
            for (unsigned s_len = 0; s_len < 30; s_len += 2) {
                std::memset(dst, 'z', sizeof dst);
                std::memset(src, 'q', sizeof src);
                for (unsigned i = 0; i < d_len; ++i) dst[d_off + i] = char('a' + i % 20);
                dst[d_off + d_len] = '\0';
                for (unsigned i = 0; i < s_len; ++i) src[i] = char('A' + i % 20);
                src[s_len] = '\0';
                char expect[160];
                std::memcpy(expect, dst, sizeof dst);
                std::memcpy(expect + d_off + d_len, src, s_len + 1);
                char* ret = w9_strcat(dst + d_off, src);
                if (ret != dst + d_off) return 1;
                if (std::memcmp(dst, expect, sizeof dst) != 0) return 2;
                ++checked;
            }
        }
    }
    std::printf("strcat ok (%u cases)\n", checked);
    return 0;
}
