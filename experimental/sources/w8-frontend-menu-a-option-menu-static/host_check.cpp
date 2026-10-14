#include <cstdio>
#include "static_init.cpp"
static int calls, order[8], flags[8];
extern "C" void _$_14AnimationState(void* p, int f) {
    order[calls] = static_cast<int>((static_cast<unsigned char*>(p) - &option_menu_animation_states[0][0]) / 40);
    flags[calls++] = f;
}
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
    option_menu_static_work(0, 0x1234);
    for (int i = 0; i < 3; ++i) if (option_menu_animation_states[i][5]) return 2;
    if (calls) return 3;
    _GLOBAL_$I$optionMenu();
    for (int i = 0; i < 3; ++i) if (option_menu_animation_states[i][5] != 1) return 4;
    if (nonzero_other_bytes() || calls) return 5;
    _GLOBAL_$D$optionMenu();
    if (calls != 3 || order[0] != 2 || order[1] != 1 || order[2] != 0) return 6;
    if (flags[0] != 2 || flags[1] != 2 || flags[2] != 2) return 7;
    for (int i = 0; i < 3; ++i) if (option_menu_animation_states[i][5] != 1) return 8;  // dtor does not clear
    std::puts("_GLOBAL_$I$optionMenu and _GLOBAL_$D$optionMenu checks passed");
    return 0;
}
