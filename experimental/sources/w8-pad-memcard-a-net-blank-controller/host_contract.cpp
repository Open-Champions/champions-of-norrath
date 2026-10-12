#include "net_blank_controller.cpp"

#include <cstdio>
#include <initializer_list>

extern "C" PadBlankRecord padBlank[4] = {};

int main() {
    padBlank[2] = {5, 7, 0};
    netBlankController__Fii(2, 3);
    if (padBlank[2].count != 5 || padBlank[2].buttons != -1 || padBlank[2].flag != 1) return 1;
    netBlankController__Fii(2, 9);
    if (padBlank[2].count != 9) return 2;
    padBlank[2].count = -4;
    netBlankController__Fii(2, -6);
    if (padBlank[2].count != -4) return 3;
    netBlankController__Fii(2, -2);
    if (padBlank[2].count != -2) return 4;
    for (int i : {0, 1, 3})
        if (padBlank[i].count || padBlank[i].buttons || padBlank[i].flag) return 5;
    std::puts("net blank controller passed");
    return 0;
}
