// Host check only; not a build input. Compares the authored tables with the initial bytes observed in the image
// (all zero, two 32-bit words each).
#include "text_tables.cpp"
#include <cstdio>

int main() {
    bool ok = true;
    const unsigned* tables[5] = {sSoundOutputText, sCompactKeyboardText, sOnOffText, sCameraTypeText, sCameraAxisText};
    for (const unsigned* t : tables) ok = ok && t[0] == 0 && t[1] == 0;
    ok = ok && sizeof sSoundOutputText == 8 && sizeof sCameraAxisText == 8;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
