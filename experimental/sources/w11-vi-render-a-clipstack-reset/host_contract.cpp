#include "reset.cpp"

int main() {
    int words[10];
    for (int i = 0; i < 10; ++i) words[i] = 0x11110000 + i;
    int result = Reset__11VIClipStack(words);
    if (result != 0) return 1;
    for (int i = 0; i < 10; ++i) {
        int expect = (i == 0 || i == 3 || i == 6) ? 0 : 0x11110000 + i;
        if (words[i] != expect) return 2 + i;
    }
    return 0;
}
