#include "play_bgm.cpp"
#include <string.h>

static const void* got_dst; static const char* got_src; static int calls;
extern "C" char* strcpy(char* dst, const char* src) { got_dst = dst; got_src = src; ++calls; char* d = dst; while ((*d++ = *src++)) {} return dst; }

int main() {
    static unsigned char dev[8192];
    memset(dev, 0x55, sizeof dev);
    if (PlayBgm__13VISoundDeviceiPCc(dev, 2, "abc") != 0 || calls != 1) return 1;
    if (got_dst != dev + 512 + 3436) return 2;
    if (memcmp(dev + 3436 + 512, "abc", 4) != 0) return 3;
    if (dev[3436 + 512 + 4] != 0x55 || dev[3436 + 511] != 0x55) return 4;
    PlayBgm__13VISoundDeviceiPCc(dev, 0, "x");
    if (got_dst != dev + 3436 || dev[3436] != 'x') return 5;
    return 0;
}
