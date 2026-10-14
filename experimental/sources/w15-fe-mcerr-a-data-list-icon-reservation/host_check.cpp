// Host check only; not a build input.
#include "list_icon.cpp"
#include <cstdio>

int main() {
    bool ok = sizeof sListIcon == 118232 && reinterpret_cast<unsigned long>(sListIcon) % 16 == 0;
    for (unsigned char v : sListIcon) ok = ok && v == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
