// Host check only; not a build input.
#include "setcaretoffset.cpp"
#include <cstdio>
#include <initializer_list>

static unsigned short text[8] = {1, 2, 3, 4, 5, 0, 0, 0};
static int len_calls;
extern "C" int strlen16__8VIStringPCUs(const unsigned short* p) { ++len_calls; int n = 0; while (p[n]) ++n; return n; }

int main() {
    bool ok = true;
    for (int has_text = 0; has_text < 2; ++has_text) {
        for (int offset : {-5, -1, 0, 1, 4, 5, 6, 100}) {
            for (int old : {0, 3, 5}) {
                int words[128];
                for (int& w : words) w = 0x1234;
                words[106] = has_text ? static_cast<int>(reinterpret_cast<unsigned long>(text)) : 0;
                words[101] = old; words[102] = 99;
                len_calls = 0;
                const int len = has_text ? 5 : 0;
                int want = offset > len ? len : offset;
                if (want < 0) want = 0;
                const int ret = SetCaretOffset__9VIWndEditi(words, offset);
                ok = ok && ret == (want != old) && words[101] == want && words[102] == -1 && len_calls == has_text;
                for (int i = 0; i < 128; ++i) ok = ok && (i == 101 || i == 102 || i == 106 || words[i] == 0x1234);
            }
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
