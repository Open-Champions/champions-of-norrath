// Host check only; not a build input.
#include "onkeyup.cpp"
#include <cstdio>
#include <initializer_list>

int sCompactKeyboard;
int main() {
    bool ok = true;
    for (int compact = 0; compact < 3; ++compact) {
        for (int key : {0, 5, 135, 136, 139, 140, 141, 142, 200}) {
            for (int held : {0, key, key + 1}) {
                int words[128];
                for (int& w : words) w = 77;
                words[96] = held;
                sCompactKeyboard = compact;
                OnKeyUp__9VIWndEditi(words, key);
                int want96 = held, want12 = 77;
                if (key == held) want96 = 0;
                if (compact != 1) {
                    if (key < 136) want96 = 0;
                    else if (key >= 140) { want96 = 0; if (key == 142) want12 = 0; }
                }
                ok = ok && words[96] == want96 && words[12] == want12;
                for (int i = 0; i < 128; ++i) ok = ok && (i == 96 || i == 12 || words[i] == 77);
            }
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
