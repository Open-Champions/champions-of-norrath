// Host contract for the Projectile constructor; not an EE input. The GameObject constructor
// writes a marker pattern into the words the Projectile constructor then edits; the link call
// is a stub that logs. The contract fails on any wrong value, call or argument.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char vt_Projectile[8];
static std::string calls; static unsigned char* the_object;
void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls += "go(" + std::string(self == the_object ? "self" : "other") + "," + std::to_string(p.x) + ":" + std::to_string(p.y) + ":" + std::to_string(p.z) + "," +
             std::to_string(reinterpret_cast<unsigned long>(map)) + "," + std::to_string(reinterpret_cast<unsigned long>(names)) + ");";
    unsigned char* o = static_cast<unsigned char*>(self);
    std::memset(o, 0x21, 128);
    const unsigned zero12 = 0x100; std::memcpy(o + 12, &zero12, 4);
}
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* item) {
    calls += "link(" + std::to_string(static_cast<unsigned char*>(head) - the_object) + "," + (other == item ? "same" : "diff") + "," + (item == reinterpret_cast<void*>(0x77) ? "owner" : "?") + ");";
}
int main() {
    alignas(16) static unsigned char o[256];
    int bad = 0;
    the_object = o;
    for (void* owner : {static_cast<void*>(nullptr), reinterpret_cast<void*>(0x77)}) {
        std::memset(o, 0, sizeof o); calls.clear();
        const Point3 p = {1.0f, -2.0f, 3.5f};
        construct(o, p, owner, reinterpret_cast<void*>(0x55));
        float f28; unsigned w12, w72, w76; std::memcpy(&f28, o + 28, 4); std::memcpy(&w12, o + 12, 4);
        std::memcpy(&w72, o + 72, 4); std::memcpy(&w76, o + 76, 4);
        std::string want = "go(self,1.000000:-2.000000:3.500000,85,0);";
        if (owner) want += "link(76,same,owner);";
        if (calls != want || f28 != 12.0f || w12 != (0x100u | 0x4081u) || w76 != 0 || o[80] != 0 ||
            w72 != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Projectile))) { std::puts(calls.c_str()); bad = 1; }
        if (o[81] != 0x21 && o[81] != 0) bad = 1;
        if (o[0] != 0x21 || o[127] != 0x21) bad = 1;
    }
    return bad;
}
