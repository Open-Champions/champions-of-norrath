// Host check for _GLOBAL_$I$cutMode. Not an EE plan input.
#include "static_init_cut_mode.cpp"

int main() {
    // The original has no memory effect, so the check calls it repeatedly and confirms control returns.
    _GLOBAL_$I$cutMode();
    _GLOBAL_$I$cutMode();
    return 0;
}
