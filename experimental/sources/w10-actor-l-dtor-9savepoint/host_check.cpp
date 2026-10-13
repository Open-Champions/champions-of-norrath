// Host contract for _$_9Savepoint; not an EE input. Each callee logs its object offset and flag, and
// whether the table word at +72 already held the 9Savepoint table pointer; blockFree is called only
// when bit 0 of the flags is set.
#include <cstring>
#include <initializer_list>
#include <string>
static std::string calls; static unsigned char* o;
extern "C" unsigned char norrath_ctors_lava_wraith_view_449550[8] = {};
static bool vt_ok() { unsigned w; std::memcpy(&w, o + 72, 4); return w == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_449550)); }
extern "C" void light_destructor(void* s, int f) __asm__("_$_5Light");
extern "C" void light_destructor(void* s, int f) { calls += "light_destructor" + std::to_string(static_cast<unsigned char*>(s) - o) + "/" + std::to_string(f) + (vt_ok() ? "v;" : "x;"); }
extern "C" void animation_state_destructor(void* s, int f) __asm__("_$_14AnimationState");
extern "C" void animation_state_destructor(void* s, int f) { calls += "animation_state_destructor" + std::to_string(static_cast<unsigned char*>(s) - o) + "/" + std::to_string(f) + (vt_ok() ? "v;" : "x;"); }
extern "C" void game_object_destructor(void* s, int f) __asm__("_$_10GameObject");
extern "C" void game_object_destructor(void* s, int f) { calls += "game_object_destructor" + std::to_string(static_cast<unsigned char*>(s) - o) + "/" + std::to_string(f) + (vt_ok() ? "v;" : "x;"); }
extern "C" void blockFree__FPv(void* p) { calls += "free" + std::to_string(p == o) + ";"; }
#include "dtor.cpp"
int main() {
    alignas(16) static unsigned char obj[1024];
    o = obj; int bad = 0;
    for (int flags : {0, 1, 2, 3, 0x100, 0x101, -1, -2}) {
        std::memset(obj, 0x31, sizeof obj); calls.clear();
        destructor_9savepoint(obj, flags);
        std::string want = "light_destructor136/2v;animation_state_destructor76/2v;game_object_destructor0/0v;";
        if (flags & 1) want += "free1;";
        if (calls != want) bad = 1;
    }
    return bad;
}
