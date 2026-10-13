#include "ctor.cpp"

int main() {
    int buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = 0x5a5a5a00 + i;
    __t6VIPool1ZP8VIRefMap(buf + 1);
    static const int expect[6] = {0, 0, 0, -1, -1, -1};
    for (int i = 0; i < 6; ++i) if (buf[1 + i] != expect[i]) return 1 + i;
    if (buf[0] != 0x5a5a5a00 || buf[7] != 0x5a5a5a00 + 7) return 20;
    return 0;
}
