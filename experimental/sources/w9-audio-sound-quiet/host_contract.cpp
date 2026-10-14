#include "sound_quiet.cpp"
#include <cstdio>
static int calls[8][4], count;
extern "C" int sceSdRemote(int a, int b, int c, int d) {
    if (count < 8) { calls[count][0] = a; calls[count][1] = b; calls[count][2] = c; calls[count][3] = d; }
    ++count;
    return 0;
}
int main() {
    soundQuiet__Fv();
    if (count != 4) return 1;
    const int expect[4] = {0x980, 0xa80, 0x981, 0xa81};
    for (int i = 0; i < 4; ++i)
        if (calls[i][0] != 1 || calls[i][1] != 0x8010 || calls[i][2] != expect[i] || calls[i][3] != 0) return 2 + i;
    std::puts("sound_quiet passed");
    return 0;
}
