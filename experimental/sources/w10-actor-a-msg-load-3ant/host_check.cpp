// Host contract for msg_load__3AntPc; not an EE input. Includes the source.
#include "msg_load.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>
static int calls; static int returned;
int msg_load__8CreaturePc(void*, char*) { ++calls; return returned; }
int main() {
    alignas(16) static unsigned char object[256];
    alignas(16) static char buffer[64];
    int bad = 0;
    struct Case { int v; int byte; } cases[] = {{1, 2}, {2, 4}, {0, 0}, {3, 0}, {-1, 0}};
    for (int n : {0, 8, 32}) {
        for (const Case& c : cases) {
            std::memset(object, 0x55, sizeof object);
            std::memcpy(buffer + n, &c.v, 4); returned = n; calls = 0;
            int r = msg_load__3AntPc(object, buffer);
            int stored; std::memcpy(&stored, object + 208, 4);
            if (r != n + 4 || calls != 1 || stored != c.v || object[156] != c.byte) { std::printf("fail: n=%d v=%d\n", n, c.v); bad = 1; }
        }
    }
    return bad;
}
