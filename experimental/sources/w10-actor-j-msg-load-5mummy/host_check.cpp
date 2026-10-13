// Host contract for msg_load__5MummyPc; not an EE input. initType is a stub that logs its
// argument and records the +208 word it saw (stores must come first).
#include <cstdio>
#include <initializer_list>
#include <cstring>
static int returned, seenFlag = -1, initCalls; static void* seenSelf; static unsigned seen208; static short stateAtCall;
extern "C" int msg_load__8CreaturePc(void* s, char*) { seenSelf = s; return returned; }
extern "C" void initType__5Mummyb(void* s, bool f) { ++initCalls; seenFlag = f; std::memcpy(&seen208, static_cast<unsigned char*>(s) + 208, 4); }
#include "msg_load.cpp"
int main() {
    alignas(16) static unsigned char o[1024];
    alignas(16) static char buffer[96];
    int bad = 0;
    for (short state : {0, 10, 9, 11}) {
        for (int n : {0, 8, 24}) {
            std::memset(o, 0, sizeof o); std::memset(buffer, 0, sizeof buffer);
            const unsigned w[3] = {0xa0a0a0a0u, 0xb1b1b1b1u, 0xc2c2c2c2u};
            std::memcpy(buffer + n, w, 12);
            std::memcpy(o + 572, &state, 2);
            returned = n; initCalls = 0; seenFlag = -1; seen208 = 0;
            int r = msg_load__5MummyPc(o, buffer);
            unsigned g960, g964, g208; std::memcpy(&g960, o + 960, 4); std::memcpy(&g964, o + 964, 4); std::memcpy(&g208, o + 208, 4);
            const unsigned want964 = state == 10 ? 0u : w[1];
            if (r != n + 12 || g960 != w[0] || g964 != want964 || g208 != w[2] || initCalls != 1 || seenFlag != 1 || seen208 != w[2] || seenSelf != o) {
                std::printf("fail state=%d n=%d\n", state, n); bad = 1;
            }
        }
    }
    return bad;
}
