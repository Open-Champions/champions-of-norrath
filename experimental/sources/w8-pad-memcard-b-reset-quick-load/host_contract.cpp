#include "reset_quick_load.cpp"

#include <cstdio>

unsigned char quickLoadStamp[64];

int main() {
    for (int index = 0; index < 64; ++index) quickLoadStamp[index] = static_cast<unsigned char>(index + 1);
    resetQuickLoad__Fv();
    for (int index = 0; index < 64; ++index)
        if (quickLoadStamp[index] != 0) return 1;
    std::puts("reset quick load passed");
    return 0;
}
