#include "skin_setting.cpp"
#include <cstdio>

int main() {
    int bad = sizeof(skinSetting) != 16;
    for (int i = 0; i < 16; ++i) bad |= skinSetting[i] != 0;
    std::printf(bad ? "FAIL\n" : "ok: 16 zero bytes\n");
    return bad;
}
