// Host contract for msg_collision__11DiseaseBoltP10GameObject; not an EE input. 32-bit words are offsets from a host arena; the two
// table calls, eRand and gameAttackResolver are logging stubs (the resolver also records the written bytes of the item description).
// The authored function was also compared with the original in a private R5900 run (129 of 137 original words covered; the rest are nops
// and the non-first-entry branches of the description search, which a fresh description never reaches).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w14_projectile_a_host_base;
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static std::string calls;
static unsigned owner_test_result = 1;
static int amount_n = 100, amount_halfword_seen; static void* amount_address_seen; static unsigned char* amount_object_seen;
static int rolls[2], roll_i;
static void *r_attacker, *r_target; static float r_a, r_b; static int r_c, r_d, r_e, r_f; static bool r_g; static unsigned char r_desc[80];
unsigned w14_projectile_a_host_call(unsigned fn, void*, int count, unsigned long, unsigned long) {
    if (fn == 1 && count == 0) { calls += "owner160;"; return owner_test_result; }
    calls += "BADCALL;"; return 0;
}
int w14_projectile_a_host_call3(unsigned fn, void* adjusted, int a1, int a2, void* a3) {
    if (fn != 2 || a1 != 0) { calls += "BAD112;"; return 0; }
    calls += "amount;"; amount_halfword_seen = a2; amount_address_seen = a3; amount_object_seen = static_cast<unsigned char*>(adjusted);
    return amount_n;
}
extern "C" {
int eRand__10GameObject(void*) { calls += "rand;"; return rolls[roll_i++ & 1]; }
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f, void* description, bool g) {
    calls += "resolve;"; r_attacker = attacker; r_target = target; r_a = a; r_b = b; r_c = c; r_d = d; r_e = e; r_f = f; r_g = g;
    std::memcpy(r_desc, description, 80);
}
}
#include "msg_collision.cpp"

static unsigned char* object(int i) { return arena + 0x100 + 0x100 * i; }
static unsigned off(unsigned char* p) { return static_cast<unsigned>(p - arena); }
// objects: 0 bolt o, 1 touched t, 2 link W (owner of the bolt's link; its +12 word is object 3), 3 the owner object, 4 amount object X
static void setup(bool linked, unsigned flags, unsigned long long t160, unsigned done) {
    using namespace w14pa;
    std::memset(arena, 0xee, sizeof arena);
    for (int i = 0; i < 5; ++i) std::memset(object(i), 0, 0x100);
    word(object(0), 84) = linked ? off(object(2)) : 0;
    word(object(2), 12) = off(object(3));
    word(object(3), 72) = off(arena + 0xa00); *reinterpret_cast<short*>(arena + 0xa00 + 160) = 0; word(arena + 0xa00, 164) = 1;
    word(object(0), 92) = off(object(4));
    word(object(4), 96) = off(arena + 0xb00); *reinterpret_cast<short*>(arena + 0xb00 + 112) = 24; word(arena + 0xb00, 116) = 2;
    half(object(0), 96) = -1234;
    word(object(0), 120) = done;
    word(object(1), 12) = flags;
    dword(object(1), 160) = t160;
    calls.clear(); roll_i = 0; owner_test_result = 1;
    rolls[0] = 0x8000; rolls[1] = 0x4000;
}
static int expect_return_early(const char* want, unsigned want_done) {
    using namespace w14pa;
    if (calls != want) { std::printf("calls %s\nwant  %s\n", calls.c_str(), want); return 1; }
    if (word(object(0), 120) != want_done) return 2;
    return 0;
}
static int check_hit(float want_amount_roll_scale, unsigned want_done) {
    using namespace w14pa;
    // the amount n = 100; first roll 0x8000 (0.5) in the spread, so d = 100 + 100 * 0.5 * 0.2
    if (calls.find("BAD") != std::string::npos) return 1;
    if (r_a != 100.0f) return 2;
    const float spread = 100.0f * (static_cast<float>(0x8000) * bits(0x37800000u));
    if (r_b != 100.0f + spread * bits(0x3e4ccccc) || r_b != want_amount_roll_scale) return 3;
    if (r_c != 100000 || r_d != 0 || r_e != 0 || r_f != 0 || r_g) return 4;
    if (amount_halfword_seen != -1234 || amount_address_seen != object(0) + 76 || amount_object_seen != object(4) + 24) return 5;
    // item description: eight zero halfwords at 16..31, bytes 32 and 33 = 255, entry 0 = {38, n, n, 0}, entries 1..3 start with -1
    for (int i = 16; i < 32; ++i) if (r_desc[i] != 0) return 6;
    if (r_desc[32] != 0xff || r_desc[33] != 0xff) return 7;
    const short* e0 = reinterpret_cast<const short*>(r_desc + 36);
    if (e0[0] != 38 || e0[1] != 100 || e0[2] != 100 || e0[3] != 0) return 8;
    for (int e = 1; e < 4; ++e) if (*reinterpret_cast<const short*>(r_desc + 36 + 8 * e) != -1) return 9;
    if (word(object(0), 120) != want_done) return 10;
    return 0;
}
int main() {
    using namespace w14pa;
    w14_projectile_a_host_base = arena;
    int r;
    // already done: nothing happens
    setup(true, 0x20, 0, 1); msg_collision__11DiseaseBoltP10GameObject(object(0), object(1));
    if ((r = expect_return_early("", 1))) return r;
    // linked owner fails the entry-160 test
    setup(true, 0x20, 0, 0); owner_test_result = 0; msg_collision__11DiseaseBoltP10GameObject(object(0), object(1));
    if ((r = expect_return_early("owner160;", 0))) return 10 + r;
    // not linked and the touched object lacks flag 0x20: the done word is set at once; neither later test applies; the attack runs
    setup(false, 0x00, 0, 0); msg_collision__11DiseaseBoltP10GameObject(object(0), object(1));
    if (calls != "amount;rand;resolve;") { std::puts(calls.c_str()); return 20; }
    if (r_target != nullptr || r_attacker != object(1)) return 21;
    if ((r = check_hit(110.0f, 1))) return 30 + r;
    // linked, flag 0x20 set and the 64-bit value without bit 0x200: the owner object is the resolver's second argument
    setup(true, 0x20, 0x1ffull, 0); msg_collision__11DiseaseBoltP10GameObject(object(0), object(1));
    if (calls != "owner160;amount;rand;resolve;") { std::puts(calls.c_str()); return 50; }
    if (r_target != object(3) || r_attacker != object(1)) return 51;
    if ((r = check_hit(110.0f, 1))) return 60 + r;
    // flag 0x20 with bit 0x200 of the 64-bit value set returns before the amount call and leaves the done word clear
    setup(false, 0x20, 0x200ull, 0); msg_collision__11DiseaseBoltP10GameObject(object(0), object(1));
    if ((r = expect_return_early("", 0))) return 80 + r;
    // flag 0x40: a negative roll returns, a non-negative one continues (the bit 0x200 value is not consulted)
    setup(false, 0x40, 0x200ull, 0); rolls[0] = -1; msg_collision__11DiseaseBoltP10GameObject(object(0), object(1));
    if ((r = expect_return_early("rand;", 1))) return 90 + r;
    setup(false, 0x40, 0x200ull, 0); rolls[0] = 5; rolls[1] = 0x8000; msg_collision__11DiseaseBoltP10GameObject(object(0), object(1));
    if (calls != "rand;amount;rand;resolve;") { std::puts(calls.c_str()); return 100; }
    // a roll of exactly zero is not negative and continues
    setup(false, 0x40, 0x200ull, 0); rolls[0] = 0; rolls[1] = 0x8000; msg_collision__11DiseaseBoltP10GameObject(object(0), object(1));
    if (calls != "rand;amount;rand;resolve;") { std::puts(calls.c_str()); return 102; }
    // the second roll (0x8000) feeds the spread, as in the first hit
    if (r_b != 100.0f + (100.0f * (static_cast<float>(0x8000) * bits(0x37800000u))) * bits(0x3e4ccccc)) return 101;
    return 0;
}
