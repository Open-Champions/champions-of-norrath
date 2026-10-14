// Host check only; not a build input.
#include "next.cpp"
#include <cstdio>

int main() {
    Element elements[5] = {{1, 3, 9}, {2, -1, 9}, {3, 0, 9}, {4, 0x7ffffffe, 9}, {5, -2, 9}};
    Container c = {7, 8, elements};
    const int want[5] = {3, 0x7fffffff, 0, 0x7ffffffe, -2};
    bool ok = true;
    for (int i = 0; i < 5; ++i) if (Next__Ct6VIPool1ZP13VIHSpriteAnimi(&c, i) != want[i]) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
