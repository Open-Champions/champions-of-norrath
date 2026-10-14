#include "web_ctor.cpp"

#include <cstdio>

int main() {
    int words[20];
    for (int i = 0; i < 20; ++i) words[i] = 0x5a5a0000 + i;
    __3Webii(words, 35, 180);
    for (int i = 0; i < 20; ++i) {
        int expected = 0x5a5a0000 + i;
        if (i == 12) expected = 35;
        if (i == 13) expected = 180;
        if (i == 2 || i == 7 || i == 11) expected = 0;
        if (words[i] != expected) return 1 + i;
    }
    __3Webii(words, -1, 0x7fffffff);
    if (words[12] != -1 || words[13] != 0x7fffffff) return 30;
    std::puts("web constructor contract passed");
    return 0;
}
