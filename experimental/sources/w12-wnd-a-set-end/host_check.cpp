// Host check only; not a build input. Paired-run compared with the original in a private interpreter.
#include "end.cpp"
#include <cstdio>
int main() {
    unsigned char set[32] = {1, 2, 3};
    const bool ok = End__Ct5VISet1ZP5VIWnd(set) == 0 && End__Ct5VISet1ZP5VIWnd(nullptr) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
