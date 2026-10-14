// Host contract for msg_save__10CloudGiantPc; not an EE input. Includes the source.
#include "msg_save.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static int calls, returned;
int msg_save__8CreaturePc(void*, char*) { ++calls; return returned; }
int main() {
    alignas(16) static unsigned char object[256];
    alignas(16) static char buffer[128];
    int bad = 0;
    for (int n : {0, 8, 32, 64}) {
        std::memset(object, 0x55, sizeof object); std::memset(buffer, 0x77, sizeof buffer);
        int word = 0x12345678 + n; std::memcpy(object + 208, &word, 4);
        returned = n; calls = 0;
        int r = msg_save__10CloudGiantPc(object, buffer);
        int stored; std::memcpy(&stored, buffer + n, 4);
        bool rest = true;
        for (int i = 0; i < 128; ++i) if ((i < n || i >= n + 4) && buffer[i] != 0x77) rest = false;
        if (r != n + 4 || calls != 1 || stored != word || !rest) { std::printf("fail: n=%d\n", n); bad = 1; }
    }
    return bad;
}
