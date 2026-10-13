// Host contract for _$_10StaticFire; not an EE input. Includes the source. Callees record their arguments
// and the table word at object+72 when they run.
#define W10_ACTOR_N_HOST 1
unsigned char* w10_actor_n_host_base;
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_10StaticFire[8];
static unsigned char arena[1300] __attribute__((aligned(16)));
static std::string calls;
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static std::string tab() { unsigned w; std::memcpy(&w, arena + 72, 4); return w == lo(vt_10StaticFire) ? "own" : "other"; }
extern "C" {
void memFree__FPv(void* b) { calls += "memfree" + std::to_string(static_cast<unsigned char*>(b) - arena) + ":" + tab() + ";"; }
void light_destructor(void* l, int f) __asm__("_$_5Light");
void light_destructor(void* l, int f) { calls += "light" + std::to_string(static_cast<unsigned char*>(l) - arena) + "," + std::to_string(f) + ":" + tab() + ";"; }
void gameobject_destructor(void* s, int f) __asm__("_$_10GameObject");
void gameobject_destructor(void* s, int f) { calls += std::string("gameobject") + (s == arena ? "obj" : "bad") + std::to_string(f) + ":" + tab() + ";"; }
void blockFree__FPv(void* b) { calls += std::string("free") + (b == arena ? "obj" : "bad") + ":" + tab() + ";"; }
}
int main() {
    w10_actor_n_host_base = arena;
    for (int flag111 : {0, 1, 0x80, 0xff}) {
    for (int flags : {0, 1, 2, 3, 4, 5, -1, -2, 0x10000, 0x10001}) {
        std::memset(arena, 0xa5, sizeof arena); calls.clear();
        arena[111] = static_cast<unsigned char>(flag111);
        { unsigned w = 600; std::memcpy(arena + 1188, &w, 4); }
        destructor_10StaticFire(arena, flags);
        std::string want = std::string(flag111 ? "memfree600:own;" : "") + "light128,2:own;gameobjectobj0:own;";
        if (flags & 1) want += "freeobj:own;";
        if (calls != want) { std::printf("fail flags=%d\n  want %s\n  got  %s\n", flags, want.c_str(), calls.c_str()); return 1; }
        unsigned w; std::memcpy(&w, arena + 72, 4);
        if (w != lo(vt_10StaticFire)) { std::puts("fail: table word"); return 2; }
    }
    }
    return 0;
}
