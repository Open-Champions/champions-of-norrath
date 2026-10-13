#include "clear.cpp"
#include <string.h>

int main() {
    alignas(4) unsigned char s[128];
    memset(s, 0xaa, sizeof s);
    if (Clear__12VINameSprite(s) != 0) return 1;
    if (s[68] != 0) return 2;
    if (*(float*)(s + 48) != 22.0f) return 3;
    if (*(int*)(s + 52) != 6) return 4;
    if (*(int*)(s + 64) != -1) return 5;
    for (int i = 0; i < 4; ++i) if (*(float*)(s + 100 + 4 * i) != 1.0f) return 6 + i;
    for (int i = 0; i < 128; ++i) {
        bool written = i == 68 || (i >= 48 && i < 56) || (i >= 64 && i < 68) || (i >= 100 && i < 116);
        if (!written && s[i] != 0xaa) return 20;
    }
    return 0;
}
