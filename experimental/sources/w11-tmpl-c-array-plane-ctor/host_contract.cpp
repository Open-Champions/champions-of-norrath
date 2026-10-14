#include "ctor.cpp"

int main() {
    int buf[4];
    for (int i = 0; i < 4; ++i) buf[i] = 0x5a5a5a00 + i;
    __t7VIArray1Z7VIPlane(buf + 1);
    static const int expect[2] = {0, 0};
    for (int i = 0; i < 2; ++i) if (buf[1 + i] != expect[i]) return 1 + i;
    if (buf[0] != 0x5a5a5a00 || buf[3] != 0x5a5a5a00 + 3) return 20;
    return 0;
}
