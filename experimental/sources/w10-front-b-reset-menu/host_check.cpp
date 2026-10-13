#include "reset_menu.cpp"
#include <cstdio>
#include <cstring>

int main() {
    unsigned char a[116], b[116];
    for (int i = 0; i < 116; ++i) a[i] = b[i] = static_cast<unsigned char>(i * 13 + 5);
    resetMenu__FP12screenStruct(a);
    const int offs[7] = {20, 24, 28, 36, 40, 60, 64};
    for (int o : offs) std::memset(b + o, 0, 4);
    int bad = std::memcmp(a, b, 116) != 0;
    std::printf("%s\n", bad ? "FAIL" : "ok: exactly the seven listed words are cleared");
    return bad;
}
