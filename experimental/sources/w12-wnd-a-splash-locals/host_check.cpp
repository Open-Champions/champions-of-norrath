// Host check only; not a build input. Checks the initial value recorded from the image and that the
// object is writable with the documented width.
#include "last_pad_state.cpp"
#include <cstdio>
int main() {
    bool ok = norrath_splash_last_pad_state == 1 && sizeof norrath_splash_last_pad_state == 4;
    norrath_splash_last_pad_state = 0;
    ok = ok && norrath_splash_last_pad_state == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
