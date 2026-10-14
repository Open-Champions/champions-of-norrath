#include "pad_blank.cpp"

#include <cstdio>

int main() {
    if (sizeof padBlank != 48 || sizeof(PadBlankRecord) != 12) return 1;
    for (int i = 0; i < 4; ++i)
        if (padBlank[i].count || padBlank[i].buttons || padBlank[i].flag) return 2;
    std::puts("pad blank passed");
    return 0;
}
