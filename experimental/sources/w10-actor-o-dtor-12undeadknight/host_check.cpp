// Host contract for _$_12UndeadKnight; not an EE input. Includes the source. Callees record their arguments
// and the table word at object+72 when they run.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_UndeadKnight[8];
static unsigned char arena[4096] __attribute__((aligned(16)));
static std::string calls;
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static std::string tab() { unsigned w; std::memcpy(&w, arena + 72, 4); return w == lo(vt_UndeadKnight) ? "own" : "other"; }
extern "C" {
void listDestroy__FP8ListHead(void* h) { calls += "list" + std::to_string(static_cast<unsigned char*>(h) - arena) + ":" + tab() + ";"; }
void light_destructor(void* l, int f) __asm__("_$_5Light");
void light_destructor(void* l, int f) { calls += "light" + std::to_string(static_cast<unsigned char*>(l) - arena) + "," + std::to_string(f) + ":" + tab() + ";"; }
void creature_destructor(void* s, int f) { calls += std::string("creature") + (s == arena ? "obj" : "bad") + std::to_string(f) + ":" + tab() + ";"; }
void blockFree__FPv(void* b) { calls += std::string("free") + (b == arena ? "obj" : "bad") + ":" + tab() + ";"; }
}
int main() {
    for (int flags : {0, 1, 2, 3, 4, 5, -1, -2, 0x10000, 0x10001}) {
        std::memset(arena, 0xa5, sizeof arena); calls.clear();
        destructor_UndeadKnight(arena, flags);
        std::string want = "list964:own;" "list960:own;" "creatureobj0:own;";
        if (flags & 1) want += "freeobj:own;";
        if (calls != want) { std::printf("fail flags=%d\n  want %s\n  got  %s\n", flags, want.c_str(), calls.c_str()); return 1; }
        unsigned w; std::memcpy(&w, arena + 72, 4);
        if (w != lo(vt_UndeadKnight)) { std::puts("fail: table word"); return 2; }
    }
    return 0;
}
