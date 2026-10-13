#include "ctor.cpp"

int main() {
    int buf[5];
    for (int i = 0; i < 5; ++i) buf[i] = 0x5a5a5a00 + i;
    __t6VIList1ZQ27VIScene12VISceneOccup(buf + 1);
    static const int expect[3] = {0, 0, 0};
    for (int i = 0; i < 3; ++i) if (buf[1 + i] != expect[i]) return 1 + i;
    if (buf[0] != 0x5a5a5a00 || buf[4] != 0x5a5a5a00 + 4) return 20;
    return 0;
}
