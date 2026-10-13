// Host contract for _$_8Scorpion; not an EE input. Each callee logs its object offset and flag, and
// whether the table word at +72 already held the 8Scorpion table pointer; blockFree is called only
// when bit 0 of the flags is set.
#include <cstring>
#include <initializer_list>
#include <string>
static std::string calls; static unsigned char* o;
extern "C" unsigned char norrath_ctors_lava_wraith_view_44b630[8] = {};
static bool vt_ok() { unsigned w; std::memcpy(&w, o + 72, 4); return w == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_44b630)); }
extern "C" void creature_destructor(void* s, int f) __asm__("_$_8Creature");
extern "C" void creature_destructor(void* s, int f) { calls += "creature_destructor" + std::to_string(static_cast<unsigned char*>(s) - o) + "/" + std::to_string(f) + (vt_ok() ? "v;" : "x;"); }
extern "C" void blockFree__FPv(void* p) { calls += "free" + std::to_string(p == o) + ";"; }
#include "dtor.cpp"
int main() {
    alignas(16) static unsigned char obj[1024];
    o = obj; int bad = 0;
    for (int flags : {0, 1, 2, 3, 0x100, 0x101, -1, -2}) {
        std::memset(obj, 0x31, sizeof obj); calls.clear();
        destructor_8scorpion(obj, flags);
        std::string want = "creature_destructor0/0v;";
        if (flags & 1) want += "free1;";
        if (calls != want) bad = 1;
    }
    return bad;
}
