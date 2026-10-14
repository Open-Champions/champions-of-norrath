// Host contract for msg_collision__16PoisonProjectileP10GameObject; not an EE input. 32-bit words are offsets from a host arena; the
// run-time type reads go through a stub that returns a type word per object (named by the function word in its table) and logs the
// read order; the type comparison is a stub too. The authored function was also compared with the original in a private R5900 run
// (80 scenarios; 60 of 70 original words covered, the rest being the null-object __throw_bad_typeid paths, nops and nothing else).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w14_projectile_a_host_base;
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
int w14_projectile_a_host_call3(unsigned, void*, int, int, void*) { return 0; }
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static std::string calls;
static unsigned type_of_fn[4];   // function words 0x100 + i return type_word[i]
static unsigned type_word[4];
static int eq_result_when_same = 1;
unsigned w14_projectile_a_host_call(unsigned fn, void*, int count, unsigned long, unsigned long) {
    if (count != 0 || fn < 0x100 || fn > 0x103) { calls += "BADCALL;"; return 0; }
    calls += "type" + std::to_string(fn - 0x100) + ";";
    return type_word[fn - 0x100];
}
extern "C" {
int __eq__C9type_infoRC9type_info(const void* left, const void* right) {
    const unsigned a = static_cast<unsigned>(static_cast<const unsigned char*>(left) - arena), b = static_cast<unsigned>(static_cast<const unsigned char*>(right) - arena);
    calls += "eq(" + std::to_string(a) + "," + std::to_string(b) + ");";
    return a == b ? eq_result_when_same : 0;
}
void __throw_bad_typeid() { calls += "THROW;"; throw 1; }
}
#include "msg_collision.cpp"

// objects: 0 = projectile o, 1 = touched object t, 2 = owner w, 3 = the word at o+76 (an object whose +12 word is w)
static unsigned char* object(int i) { return arena + 0x100 + 0x100 * i; }
static unsigned off(unsigned char* p) { return static_cast<unsigned>(p - arena); }
static int run(bool has_q, unsigned type_o, unsigned type_t, unsigned type_w, int same_result, const char* want_calls, bool want_flag) {
    using namespace w14pa;
    std::memset(arena, 0, sizeof arena);
    for (int i = 0; i < 3; ++i) {
        word(object(i), 72) = off(arena + 0x800 + 0x40 * i);
        word(arena + 0x800 + 0x40 * i, 4) = 0x100 + i;
    }
    word(object(0), 76) = has_q ? off(object(3)) : 0;
    word(object(3), 12) = off(object(2));
    word(object(0), 152) = 0x77;
    type_word[0] = type_o; type_word[1] = type_t; type_word[2] = type_w;
    // type words are offsets into the arena so that the comparison stub can print them
    for (int i = 0; i < 3; ++i) type_word[i] = 0x40 + type_word[i];
    eq_result_when_same = same_result;
    calls.clear();
    msg_collision__16PoisonProjectileP10GameObject(object(0), object(1));
    if (calls != want_calls) { std::printf("calls %s\nwant  %s\n", calls.c_str(), want_calls); return 1; }
    const unsigned flag = word(object(0), 152);
    if (flag != (want_flag ? 1u : 0x77u)) return 2;
    return 0;
}
int main() {
    w14_projectile_a_host_base = arena;
    int r;
    // no owner link: the projectile's type is read first, then the touched object's
    if ((r = run(false, 5, 5, 6, 1, "type0;type1;eq(69,69);", false))) return r;
    if ((r = run(false, 5, 6, 6, 1, "type0;type1;eq(69,70);", true))) return 10 + r;
    // only a comparison result of exactly 1 counts as equal
    if ((r = run(false, 5, 5, 6, 2, "type0;type1;eq(69,69);", true))) return 20 + r;
    // owner link: the touched object's type first, then the owner's; equal types return without writing
    if ((r = run(true, 5, 7, 7, 1, "type1;type2;eq(71,71);", false))) return 30 + r;
    // owner link, types differ: continue with the projectile and touched object
    if ((r = run(true, 5, 6, 7, 1, "type1;type2;eq(70,71);type0;type1;eq(69,70);", true))) return 40 + r;
    if ((r = run(true, 6, 6, 7, 1, "type1;type2;eq(70,71);type0;type1;eq(70,70);", false))) return 50 + r;
    // a null touched object or owner reaches __throw_bad_typeid when its type is read
    {
        using namespace w14pa;
        std::memset(arena, 0, sizeof arena);
        for (int i = 0; i < 2; ++i) { word(object(i), 72) = off(arena + 0x800 + 0x40 * i); word(arena + 0x800 + 0x40 * i, 4) = 0x100 + i; }
        calls.clear();
        bool threw = false;
        try { msg_collision__16PoisonProjectileP10GameObject(object(0), nullptr); } catch (int) { threw = true; }
        if (!threw || calls != "type0;THROW;") return 60;
        word(object(0), 76) = off(object(3));  // the owner object (word at +12 of that one) is null
        calls.clear(); threw = false;
        try { msg_collision__16PoisonProjectileP10GameObject(object(0), object(1)); } catch (int) { threw = true; }
        if (!threw || calls != "type1;THROW;") return 61;
    }
    return 0;
}
