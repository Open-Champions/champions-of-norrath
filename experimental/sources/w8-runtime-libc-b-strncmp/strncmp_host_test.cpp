// Host check only; not an EE build input. Build the source with -Dstrncmp=w8_strncmp so it
// does not collide with the host C library, compile this file separately, and link both.
#include <cstdio>
#include <cstring>
extern "C" int w8_strncmp(const char*, const char*, unsigned);
static int reference(const unsigned char* a, const unsigned char* b, unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        if (a[i] != b[i]) return int(a[i]) - int(b[i]);
        if (!a[i]) return 0;
    }
    return 0;
}
int main() {
    alignas(16) unsigned char left[80], right[80];
    unsigned checked = 0;
    for (unsigned l_off = 0; l_off < 9; ++l_off) {
        for (unsigned r_off = 0; r_off < 9; r_off += 4) {
            for (unsigned length = 0; length < 36; length += 3) {
                for (unsigned diff_at = 0; diff_at <= length + 2; diff_at += 1) {
                    for (unsigned count = 0; count < 40; count += 1) {
                        std::memset(left, 'a', sizeof left);
                        std::memset(right, 'a', sizeof right);
                        left[l_off + length] = 0;
                        right[r_off + length] = 0;
                        if (diff_at < length) right[r_off + diff_at] = (diff_at & 1) ? 0xf0 : 'b';
                        const int want = reference(left + l_off, right + r_off, count);
                        if (w8_strncmp((char*)left + l_off, (char*)right + r_off, count) != want) return 1;
                        ++checked;
                    }
                }
            }
        }
    }
    if (w8_strncmp("abc", "abd", 0) != 0) return 2;
    if (w8_strncmp("--net", "--netx", 5) != 0) return 3;
    if (w8_strncmp("\xff", "\x7f", 1) != 0x80) return 4;
    std::printf("strncmp ok (%u cases)\n", checked);
    return 0;
}
