// Host contract for _$_11FireBomblet; not an EE input. Includes the source. Callees record their arguments and
// the table word at object+72 when they run. Returns non-zero when wrong.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_FireBomblet[8];
static unsigned char arena[4096] __attribute__((aligned(16)));
static std::string calls;
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static std::string tab() { unsigned w; std::memcpy(&w, arena + 72, 4); return w == lo(vt_FireBomblet) ? "own" : "other"; }
static std::string at(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void lightDeactivate__FP5Light(void* l) { calls += "deact" + at(l) + ":" + tab() + ";"; }
void light_destructor(void* l, int f) { calls += "light" + at(l) + "," + std::to_string(f) + ":" + tab() + ";"; }
void gameobject_destructor(void* s, int f) { calls += "go" + at(s) + "," + std::to_string(f) + ":" + tab() + ";"; }
void blockFree__FPv(void* b) { calls += "free" + at(b) + ":" + tab() + ";"; }
}
int main() {
    for (int flags : {0, 1, 2, 3, 4, 5, -1, -2, 0x10000, 0x10001}) {
        std::memset(arena, 0xa5, sizeof arena); calls.clear();
        destructor_FireBomblet(arena, flags);
        std::string want = "deact88:own;light88,2:own;go0,0:own;";
        if (flags & 1) want += "free0:own;";
        if (calls != want) { std::printf("fail flags=%d\n  want %s\n  got  %s\n", flags, want.c_str(), calls.c_str()); return 1; }
        unsigned w; std::memcpy(&w, arena + 72, 4);
        if (w != lo(vt_FireBomblet)) { std::puts("fail: table word"); return 2; }
    }
    return 0;
}
