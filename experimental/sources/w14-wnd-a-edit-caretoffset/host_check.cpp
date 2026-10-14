// Host check only; not a build input.
#include "caretoffset.cpp"
#include <cstdio>
#include <initializer_list>

int main() {
    bool ok = true;
    for (int v : {-5, 0, 7, 0x7fffffff}) {
        int words[128];
        for (int& w : words) w = 0x1234;
        words[101] = v;
        ok = ok && CaretOffset__9VIWndEdit(words) == v;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
