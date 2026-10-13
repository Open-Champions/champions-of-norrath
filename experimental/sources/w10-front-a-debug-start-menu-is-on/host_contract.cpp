#include "debug_start_menu_is_on.cpp"

#include <cstdio>

signed char startMenuState;

int main() {
    int values[] = {0, 1, 2, 127, -1, -128};
    for (int v : values) {
        startMenuState = static_cast<signed char>(v);
        if (debugStartMenuIsOn__Fv() != v) return 1;
    }
    std::puts("debug start menu is on passed");
    return 0;
}
