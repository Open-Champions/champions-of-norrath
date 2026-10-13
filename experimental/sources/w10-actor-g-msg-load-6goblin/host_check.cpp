// Host contract for msg_load__6GoblinPc; not an EE input.
#include <cstdio>
#include <cstring>
#include <string>
static std::string order; static unsigned seen208, seen972, seen988, seen992; static void* initSelf; static bool initFlag;
static unsigned char* theObject;
extern "C" int msg_load__8CreaturePc(void*, char*) { order += "base;"; return 8; }
extern "C" void initType__6Goblinb(void* s, bool f) {
    order += "init;"; initSelf = s; initFlag = f;
    std::memcpy(&seen208, theObject + 208, 4); std::memcpy(&seen972, theObject + 972, 4);
    std::memcpy(&seen988, theObject + 988, 4); std::memcpy(&seen992, theObject + 992, 4);
}
#include "msg_load.cpp"
int main() {
    alignas(16) unsigned char object[1100]; alignas(16) char buffer[64]; int bad = 0; theObject = object;
    unsigned in[4] = {0xa1a1a1a1u, 0xb2b2b2b2u, 0xc3c3c3c3u, 0xd4d4d4d4u};
    for (short state : {10, 9, 0, -10}) {
        std::memset(object, 0, sizeof object); std::memset(buffer, 0, sizeof buffer); order.clear();
        std::memcpy(buffer + 8, in, 16); std::memcpy(object + 572, &state, 2);
        unsigned v996 = 0x99999999u; std::memcpy(object + 996, &v996, 4);
        int n = msg_load__6GoblinPc(object, buffer);
        unsigned g972, g988, g992, g208, g996;
        std::memcpy(&g972, object + 972, 4); std::memcpy(&g988, object + 988, 4); std::memcpy(&g992, object + 992, 4);
        std::memcpy(&g208, object + 208, 4); std::memcpy(&g996, object + 996, 4);
        if (n != 24 || order != "base;init;" || initSelf != object || !initFlag) { std::puts("fail: sequence/return"); bad = 1; }
        if (g972 != in[0] || g988 != in[1] || g992 != in[2] || g208 != in[3]) { std::puts("fail: stored words"); bad = 1; }
        if (seen972 != in[0] || seen988 != in[1] || seen992 != in[2] || seen208 != in[3]) { std::puts("fail: words stored before initType"); bad = 1; }
        if ((state == 10) != (g996 == 0)) { std::puts("fail: +996 cleared only for state 10"); bad = 1; }
    }
    if (!bad) std::puts("goblin msg_load ok");
    return bad;
}
