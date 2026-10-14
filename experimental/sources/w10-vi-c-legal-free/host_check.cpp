// Host check only; not a build input. Compared with the original in a private interpreter run.
#include "free.cpp"
#include <cstdio>
int main() {
    unsigned char storage[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    Free__10VIWndLegal(storage);
    const bool ok = storage[0] == 1 && storage[7] == 8;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
