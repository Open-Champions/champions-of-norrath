// Host check only; not a build input. Checks the initial value recorded from the image and that the
// object is writable with the documented width.
#include "dot_state.cpp"
#include <cstdio>
int main() {
    bool ok = norrath_patcher_dot == 0 && norrath_patcher_dot_delay == 15 && sizeof norrath_patcher_dot == 1 &&
              sizeof norrath_patcher_dot_delay == 1;
    norrath_patcher_dot = 2;
    norrath_patcher_dot_delay = 15;
    ok = ok && norrath_patcher_dot == 2 && norrath_patcher_dot_delay == 15;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
