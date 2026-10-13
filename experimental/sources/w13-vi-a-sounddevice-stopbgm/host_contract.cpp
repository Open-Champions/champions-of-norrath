#include "stop_bgm.cpp"
#include <string.h>

static int calls;
extern "C" int strcmp(const char* a, const char* b) { ++calls; while (*a && *a == *b) { ++a; ++b; } return (unsigned char)*a - (unsigned char)*b; }

int main() {
    static unsigned char d[5000];
    memset(d, 0, sizeof d);
    *(int*)(d + 4460) = 7;
    strcpy((char*)d + 3948, "forest");
    strcpy((char*)d + 3436 + 256, "forest");
    StopBgm__13VISoundDevicei(d, 1);
    if (*(int*)(d + 4460) != -1 || d[3436 + 256] != 0 || calls != 1) return 1;
    if (d[3436 + 256 + 1] != 'o') return 2;
    *(int*)(d + 4460) = 7;
    strcpy((char*)d + 3436, "cave");
    StopBgm__13VISoundDevicei(d, 0);
    if (*(int*)(d + 4460) != 7 || d[3436] != 0 || calls != 2) return 3;
    StopBgm__13VISoundDevicei(d, 0);          // empty name: no compare
    if (calls != 2 || *(int*)(d + 4460) != 7) return 4;
    return 0;
}
