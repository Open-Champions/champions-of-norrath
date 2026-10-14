// Host check only; not a build input.
#include "init.cpp"
#include <cstdio>

int main() {
    int word = 7;
    const bool ok = Init__20VIWndGenericRenderer(&word) == 0 && Init__20VIWndGenericRenderer(nullptr) == 0 && word == 7;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
