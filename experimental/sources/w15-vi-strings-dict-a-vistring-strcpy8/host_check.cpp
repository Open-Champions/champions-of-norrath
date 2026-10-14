#include "strcpy8.cpp"
#include <string.h>
int main() {
    const char* srcs[] = {"", "a", "ab", "hello", "\xe9x"};
    for (const char* s : srcs) {
        size_t n = strlen(s);
        for (unsigned cap = 0; cap < 9; ++cap) {
            unsigned char buf[16]; memset(buf, 0x5a, sizeof buf);
            char* r = strcpy8__8VIStringPcUiPCc(reinterpret_cast<char*>(buf + 2), cap, s);
            if (r != reinterpret_cast<char*>(buf + 2)) return 1;
            // expected: copy min(n, cap-1 (0 when cap<2)) chars, then one zero; nothing else touched
            size_t k = cap >= 2 ? (n < cap - 1 ? n : cap - 1) : 0;
            for (size_t i = 0; i < 16; ++i) {
                unsigned char want = 0x5a;
                if (i >= 2 && i < 2 + k) want = static_cast<unsigned char>(s[i - 2]);
                else if (i == 2 + k) want = 0;
                if (buf[i] != want) return 2;
            }
        }
    }
    return 0;
}
