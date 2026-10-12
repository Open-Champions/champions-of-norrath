#include <cstdio>
#include "static_init.cpp"
extern "C" void _$_14AnimationState(void*, int) {}
static int nonzero_other_bytes() {
    int n = 0;
    for (int i = 0; i < 3; ++i)
        for (int b = 0; b < 40; ++b)
            if (b != 5 && option_menu_animation_states[i][b]) ++n;
    return n;
}
int main() {
    if (sizeof option_menu_animation_states != 120) return 1;
    option_menu_static_work(1, 0x1234);       // wrong priority: nothing
    option_menu_static_work(2, 0xffff);       // neither 0 nor 1: nothing
    for (int i = 0; i < 3; ++i) if (option_menu_animation_states[i][5]) return 2;
    _GLOBAL_$I$optionMenu();
    for (int i = 0; i < 3; ++i) if (option_menu_animation_states[i][5] != 1) return 3;
    if (nonzero_other_bytes()) return 4;
    std::puts("_GLOBAL_$I$optionMenu check passed");
    return 0;
}
