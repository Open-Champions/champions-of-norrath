// Host contract for the jump-gate position and hidden storage; not an EE input.
#include "jump_gate_pos_hidden.cpp"
#include <cstdio>
int main() {
    static_assert(sizeof jumpGatePos == 16 && sizeof jumpHidden == 1, "layout");
    int bad = 0;
    if (reinterpret_cast<__UINTPTR_TYPE__>(jumpGatePos) % 16 != 0) { std::puts("fail: alignment"); return 1; }
    for (float f : jumpGatePos) if (f != 0.0f) bad = 1;
    if (jumpHidden != 0 || bad) { std::puts("fail: initial values"); return 1; }
    std::puts("jump gate pos/hidden ok");
    return 0;
}
