#include "ctor.cpp"

int main() {
    int buf[7];
    for (int i = 0; i < 7; ++i) buf[i] = 0x5a5a5a00 + i;
    __t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(buf + 1);
    static const int expect[5] = {0, 0, 0, 0, -1};
    for (int i = 0; i < 5; ++i) if (buf[1 + i] != expect[i]) return 1 + i;
    if (buf[0] != 0x5a5a5a00 || buf[6] != 0x5a5a5a00 + 6) return 20;
    return 0;
}
