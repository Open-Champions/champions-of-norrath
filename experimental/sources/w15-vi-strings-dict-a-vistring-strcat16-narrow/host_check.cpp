#include "strcat16.cpp"
int main() {
    const char s1[] = {'a', static_cast<char>(0xe9), static_cast<char>(0x80), 'b', 0};
    const unsigned short want1[] = {0x61, 0xffe9, 0xff80, 0x62};
    const unsigned short pre[][4] = {{0}, {0x41, 0}, {0x41, 0x42, 0x43, 0}};
    const unsigned plen[] = {0, 1, 3};
    for (int p = 0; p < 3; ++p)
        for (unsigned limit = 0; limit < 20; ++limit) {
            unsigned short buf[12]; for (auto& x : buf) x = 0x5a5a;
            for (int i = 0; i < 4; ++i) { buf[1 + i] = pre[p][i]; if (!pre[p][i]) break; }
            if (strcat16__8VIStringPUsUiPCc(buf + 1, limit, s1) != buf + 1) return 1;
            unsigned k = 0;
            while (k < 4 && 2 * (plen[p] + k) + 2 < limit) ++k;
            for (unsigned i = 0; i < 12; ++i) {
                unsigned short want = 0x5a5a;
                if (i >= 1 && i < 1 + plen[p]) want = pre[p][i - 1];
                else if (i >= 1 + plen[p] && i < 1 + plen[p] + k) want = want1[i - 1 - plen[p]];
                else if (i == 1 + plen[p] + k) want = 0;
                if (buf[i] != want) return 2;
            }
        }
    return 0;
}
