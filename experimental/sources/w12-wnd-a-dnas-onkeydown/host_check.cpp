// Host check only; not a build input. Keys 13 and 134 call SetFocus(this, 0, true); every other key
// does nothing.
#include "onkeydown.cpp"
#include <cstdio>
#include <string>

static std::string trace;
extern "C" void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force) {
    trace += std::string(self ? "s" : "n") + (window ? "w" : "n") + (force ? "1" : "0") + ";";
}
int main() {
    int object;
    bool ok = true;
    for (int key = -300; key < 600; ++key) {
        trace.clear();
        OnKeyDown__9VIWndDnasi(&object, key);
        ok = ok && trace == ((key == 13 || key == 134) ? "sn1;" : "");
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
