// Host contract for msg_load__10CloudGiantPc; not an EE input. Includes the source.
#include "msg_load.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static int calls, returned;
int msg_load__8CreaturePc(void*, char*) { ++calls; return returned; }
int main() {
    alignas(16) static unsigned char object[1100];
    alignas(16) static char buffer[128];
    int bad = 0;
    for (int n : {0, 8, 32}) {
        for (short state : {10, 9, 11, -10, 0}) {
            std::memset(object, 0x55, sizeof object);
            std::memcpy(object + 572, &state, 2);
            int v = 0x1badcafe + n; std::memcpy(buffer + n, &v, 4); returned = n; calls = 0;
            int r = msg_load__10CloudGiantPc(object, buffer);
            int stored, w984; std::memcpy(&stored, object + 208, 4); std::memcpy(&w984, object + 984, 4);
            if (r != n + 4 || calls != 1 || stored != v || w984 != (state == 10 ? 0 : 0x55555555)) { std::printf("fail: n=%d state=%d\n", n, state); bad = 1; }
        }
    }
    return bad;
}
