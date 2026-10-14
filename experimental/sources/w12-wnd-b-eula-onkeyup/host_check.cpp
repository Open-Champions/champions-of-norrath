// Host check only; not a build input. Exactly the word at +9756 is cleared, whatever the key.
#include "onkeyup.cpp"
#include <cstdio>
#include <cstring>

int main() {
    static unsigned char object[10000];
    bool ok = true;
    for (int key = -1; key < 3; ++key) {
        std::memset(object, 0xa5, sizeof object);
        OnKeyUp__9VIWndEulai(object, key);
        for (unsigned i = 0; i < sizeof object; ++i) {
            const bool inside = i >= 9756 && i < 9760;
            if (object[i] != (inside ? 0 : 0xa5)) ok = false;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
