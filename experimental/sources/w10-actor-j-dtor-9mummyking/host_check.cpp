// Host contract for _$_9MummyKing; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_MummyKing[8];
static std::string calls;
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }
static unsigned char* objectBase;
extern "C" {
void listDestroy__FP8ListHead(void* head) { calls += "L" + std::to_string(static_cast<unsigned char*>(head) - objectBase) + ";"; }
}
void creature_destructor(void* object, int flags) {
    unsigned w; std::memcpy(&w, static_cast<unsigned char*>(object) + 72, 4);
    calls += std::string("base") + std::to_string(flags) + (w == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_MummyKing)) ? "T" : "F") + ";";
}
void blockFree__FPv(void* object) { calls += "free" + std::to_string(static_cast<unsigned char*>(object) - objectBase) + ";"; }
int main() {
    alignas(16) unsigned char object[1280];
    objectBase = object;
    const std::string lists = "L1148;L1144;L1140;L1136;L1124;L1120;L1116;L1112;L1108;base0T;";
    for (int flags : {0, 2, 1, 5}) {
        std::memset(object, 0x3c, sizeof object); calls.clear();
        destructor_MummyKing(object, flags);
        expect(calls == lists + ((flags & 1) ? "free0;" : ""), "list destruction order, base call, free only for odd flags");
    }
    return failures ? 1 : 0;
}
