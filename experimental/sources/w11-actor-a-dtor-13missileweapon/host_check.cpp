// Host contract for _$_13MissileWeapon; not an EE input. Includes the source. Callees record their arguments and
// which table the word at object+72 names when they run. Returns non-zero when wrong.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_MissileWeapon[8];
unsigned char vt_Projectile[8];
static unsigned char arena[4096] __attribute__((aligned(16)));
static std::string calls;
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static std::string tab() {
    unsigned w; std::memcpy(&w, arena + 72, 4);
    return w == lo(vt_MissileWeapon) ? "mw" : w == lo(vt_Projectile) ? "proj" : "other";
}
static std::string at(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void P_DeleteTrail__Fi(int t) { calls += "trail" + std::to_string(t) + ":" + tab() + ";"; }
void light_destructor(void* l, int f) { calls += "light" + at(l) + "," + std::to_string(f) + ":" + tab() + ";"; }
void listDestroy__FP8ListHead(void* h) { calls += "list" + at(h) + ":" + tab() + ";"; }
void gameobject_destructor(void* s, int f) { calls += "go" + at(s) + "," + std::to_string(f) + ":" + tab() + ";"; }
void blockFree__FPv(void* b) { calls += "free" + at(b) + ":" + tab() + ";"; }
}
int main() {
    for (int trail : {0, 7})
        for (int flags : {0, 1, 2, 3, -1, -2, 0x10000, 0x10001}) {
            std::memset(arena, 0xa5, sizeof arena); calls.clear();
            std::memcpy(arena + 364, &trail, 4);
            destructor_MissileWeapon(arena, flags);
            std::string want;
            if (trail) want += "trail7:mw;";
            want += "light368,2:mw;list292:mw;list188:mw;list164:mw;list140:mw;list116:mw;list92:mw;list76:proj;go0,0:proj;";
            if (flags & 1) want += "free0:proj;";
            if (calls != want) { std::printf("fail flags=%d trail=%d\n  want %s\n  got  %s\n", flags, trail, want.c_str(), calls.c_str()); return 1; }
        }
    return 0;
}
