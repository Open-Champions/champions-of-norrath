// Host contract for msg_save__5MummyPc; not an EE input.
#include <cstdio>
#include <initializer_list>
#include <cstring>
static int calls, returned; static void* seenSelf; static char* seenBuffer;
extern "C" int msg_save__8CreaturePc(void* s, char* b) { ++calls; seenSelf = s; seenBuffer = b; return returned; }
#include "msg_save.cpp"
int main() {
    alignas(16) static unsigned char o[1024];
    alignas(16) static char buffer[96];
    int bad = 0;
    const unsigned a = 0x11111111, b = 0x22222222, c = 0x33333333;
    std::memcpy(o + 960, &a, 4); std::memcpy(o + 964, &b, 4); std::memcpy(o + 208, &c, 4);
    for (int n : {0, 4, 20, 40}) {
        std::memset(buffer, 0, sizeof buffer); returned = n; calls = 0;
        int r = msg_save__5MummyPc(o, buffer);
        unsigned w[3]; std::memcpy(w, buffer + n, 12);
        if (r != n + 12 || calls != 1 || seenSelf != o || seenBuffer != buffer || w[0] != a || w[1] != b || w[2] != c) { std::printf("fail n=%d\n", n); bad = 1; }
        for (int i = 0; i < 96; ++i) if ((i < n || i >= n + 12) && buffer[i]) bad = 1;
    }
    return bad;
}
