// Host check only; not an EE build input. Build the source with -Dstrstr=w8_strstr so it
// does not collide with the host C library, compile this file separately, and link both.
#include <cstdio>
#include <cstring>
extern "C" char* w8_strstr(const char*, const char*);
int main() {
    const char* texts[] = {"", "a", "ab", "orccave1", "xorccave1", "orccave", "aaab", "abababc", "levels/orccave1/x"};
    const char* needles[] = {"", "a", "b", "ab", "orccave1", "aab", "abc", "bab", "cave1", "orccave12", "zz", "1"};
    unsigned checked = 0;
    for (const char* t : texts) {
        for (const char* n : needles) {
            if (w8_strstr(t, n) != std::strstr(t, n)) return 1;
            ++checked;
        }
    }
    // Exhaustive over a tiny alphabet.
    char t[8], n[5];
    for (unsigned tc = 0; tc < 3 * 3 * 3 * 3 * 3 * 3; ++tc) {
        unsigned v = tc;
        for (int i = 0; i < 6; ++i) { t[i] = char('a' + v % 3); v /= 3; }
        t[6] = 0;
        for (unsigned nl = 0; nl < 3 * 3 * 3 * 3; ++nl) {
            unsigned w = nl;
            for (int i = 0; i < 4; ++i) { n[i] = char('a' + w % 3); w /= 3; }
            for (int cut = 0; cut <= 4; ++cut) {
                n[cut] = 0;
                if (w8_strstr(t, n) != std::strstr(t, n)) return 2;
                n[cut] = 'a';
                ++checked;
            }
            n[4] = 0;
        }
    }
    std::printf("strstr ok (%u cases)\n", checked);
    return 0;
}
