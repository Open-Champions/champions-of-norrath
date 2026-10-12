#include "net_blank_controller_buttons.cpp"

#include <cstdio>
#include <initializer_list>

extern "C" PadBlankRecord padBlank[4] = {};

int main() {
    padBlank[1] = {5, 7, 1};
    netBlankControllerButtons__Fiii(1, 3, 0x120);
    if (padBlank[1].count != 5 || padBlank[1].buttons != 0x120 || padBlank[1].flag != 0) return 1;
    netBlankControllerButtons__Fiii(1, 11, -1);
    if (padBlank[1].count != 11 || padBlank[1].buttons != -1) return 2;
    padBlank[1].count = -4;
    netBlankControllerButtons__Fiii(1, -6, 2);
    if (padBlank[1].count != -4) return 3;
    for (int i : {0, 2, 3})
        if (padBlank[i].count || padBlank[i].buttons || padBlank[i].flag) return 4;
    std::puts("net blank controller buttons passed");
    return 0;
}
