// Host contract for msg_save__3AntPc; not an EE input. Includes the source.
#include "msg_save.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>
static int calls; static void* seenSelf; static char* seenBuffer; static int returned;
int msg_save__8CreaturePc(void* s, char* b) { ++calls; seenSelf = s; seenBuffer = b; return returned; }
int main() {
    alignas(16) static unsigned char object[256];
    alignas(16) static char buffer[64];
    int bad = 0;
    unsigned value = 0x0badf00d; std::memcpy(object + 208, &value, 4);
    for (int n : {0, 4, 20, 36}) {
        std::memset(buffer, 0, sizeof buffer); returned = n; calls = 0;
        int r = msg_save__3AntPc(object, buffer);
        unsigned stored; std::memcpy(&stored, buffer + n, 4);
        if (r != n + 4 || calls != 1 || seenSelf != object || seenBuffer != buffer || stored != value) { std::printf("fail: n=%d\n", n); bad = 1; }
        for (int i = 0; i < 64; ++i) if ((i < n || i >= n + 4) && buffer[i] != 0) { std::printf("fail: stray write n=%d\n", n); bad = 1; break; }
    }
    return bad;
}
