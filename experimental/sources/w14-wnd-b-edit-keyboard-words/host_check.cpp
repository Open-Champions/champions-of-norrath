// Host check only; not a build input. Compares the authored words with the initial bytes observed in the image.
#include "keyboard_words.cpp"
#include <cstdio>
#include <cstring>

int main() {
    const unsigned char want_translucency[4] = {0x00, 0x00, 0x80, 0x3f};
    const unsigned char want_compact[4] = {0x01, 0x00, 0x00, 0x00};
    bool ok = std::memcmp(&sTranslucency, want_translucency, 4) == 0 && std::memcmp(&sCompactKeyboard, want_compact, 4) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
