// Host check only; not a build input.
#include "initialized.cpp"
#include <cstdio>

int main() {
    const bool ok = sizeof sMemCardInitialized == 4 && sMemCardInitialized == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
