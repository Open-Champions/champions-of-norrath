// Host contract for msg_collision__12ShockOfFrostP10GameObject; not an EE input. 32-bit words are offsets from a host arena; the two
// table calls, eRand and gameAttackResolver are logging stubs (the resolver also records the written bytes of the item description).
// The authored function was also compared with the original in a private R5900 run (138 of 147 original words covered; the rest are nops
// and the non-first-entry branches of the description search, which a fresh description never reaches).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
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
// objects: 0 bolt o, 1 touched t, 2 link Q (its +12 word is object 3), 3 the owner object, 4 amount object X
static void setup(bool linked, unsigned flags, unsigned long long t160) {
    using namespace w14pa;
    std::memset(arena, 0xee, sizeof arena);
    for (int i = 0; i < 5; ++i) std::memset(object(i), 0, 0x100);
    word(object(0), 76) = linked ? off(object(2)) : 0;
    word(object(2), 12) = off(object(3));
    word(object(3), 72) = off(arena + 0xa00); *reinterpret_cast<short*>(arena + 0xa00 + 160) = 0; word(arena + 0xa00, 164) = 1;
    word(object(0), 200) = off(object(4));
    word(object(4), 96) = off(arena + 0xb00); *reinterpret_cast<short*>(arena + 0xb00 + 112) = 24; word(arena + 0xb00, 116) = 2;
    half(object(0), 148) = -1234;
    word(object(0), 204) = 0x1111; word(object(0), 196) = 0x2222;
    word(object(1), 12) = flags;
    dword(object(1), 160) = t160;
    calls.clear(); roll_i = 0; owner_test_result = 1; amount_n = 100;
    rolls[0] = 0x8000; rolls[1] = 0x4000;
}
static int check_hit(float want_f_roll, int want_b) {
    using namespace w14pa;
    if (calls.find("BAD") != std::string::npos) return 1;
    if (r_a != 0.0f || r_b != 0.0f || r_c != 100000 || r_d != 0 || r_e != 0 || r_f != 0 || r_g) return 2;
    if (r_target != nullptr || r_attacker != object(1)) return 3;
    if (amount_halfword_seen != -1234 || amount_address_seen != object(0) + 76 || amount_object_seen != object(4) + 24) return 4;
    for (int i = 16; i < 32; ++i) if (r_desc[i] != 0) return 5;
    if (r_desc[32] != 0xff || r_desc[33] != 0xff) return 6;
    const short* e0 = reinterpret_cast<const short*>(r_desc + 36);
    if (e0[0] != 22 || e0[1] != 100 || e0[2] != want_b || e0[3] != 0) { std::printf("e0 %d %d %d %d want %d\n", e0[0], e0[1], e0[2], e0[3], want_b); return 7; }
    for (int e = 1; e < 4; ++e) if (*reinterpret_cast<const short*>(r_desc + 36 + 8 * e) != -1) return 8;
    (void)want_f_roll;
    if (word(object(0), 204) != off(object(1)) || word(object(0), 196) != 1) return 9;
    return 0;
}
int main() {
    using namespace w14pa;
    w14_projectile_a_host_base = arena;
    int r;
    // the touched object lacks flag 0x20: word +196 becomes 1 at once, no owner test; n = 100, f = 100 * 0.2f * 0.5 = 9.999999 (the single 0.2 is just below 0.2) -> truncated 9 -> second halfword 109
    setup(true, 0x00, 0); msg_collision__12ShockOfFrostP10GameObject(object(0), object(1));
    if (calls != "amount;rand;resolve;") { std::puts(calls.c_str()); return 1; }
    if ((r = check_hit(0.0f, 100 + static_cast<int>(100.0f * bits(0x3e4ccccc) * (static_cast<float>(0x8000) * bits(0x37800000u)))))) return 10 + r;
    // flag 0x20 and a link: the owner entry-160 test runs; a zero result returns with +204 already cleared
    setup(true, 0x20, 0); owner_test_result = 0; msg_collision__12ShockOfFrostP10GameObject(object(0), object(1));
    if (calls != "owner160;" || word(object(0), 204) != 0 || word(object(0), 196) != 0x2222) return 30;
    // flag 0x20, link, passing owner test, 64-bit value without bit 0x200: the attack runs
    setup(true, 0x20, 0x1ffull); msg_collision__12ShockOfFrostP10GameObject(object(0), object(1));
    if (calls != "owner160;amount;rand;resolve;") { std::puts(calls.c_str()); return 40; }
    if ((r = check_hit(0.0f, 109))) return 50 + r;
    // flag 0x20 without a link skips the owner test
    setup(false, 0x20, 0); msg_collision__12ShockOfFrostP10GameObject(object(0), object(1));
    if (calls != "amount;rand;resolve;") return 70;
    // flag 0x20 with bit 0x200 of the 64-bit value returns after +204 was cleared and before the amount call
    setup(false, 0x20, 0x200ull); msg_collision__12ShockOfFrostP10GameObject(object(0), object(1));
    if (!calls.empty() || word(object(0), 204) != 0 || word(object(0), 196) != 0x2222) return 71;
    // flag 0x40 (and 0x20 clear): a negative roll returns; zero or positive continues and the second roll feeds the amount
    setup(false, 0x40, 0x200ull); rolls[0] = -1; msg_collision__12ShockOfFrostP10GameObject(object(0), object(1));
    if (calls != "rand;" || word(object(0), 204) != 0 || word(object(0), 196) != 1) return 72;
    setup(false, 0x40, 0x200ull); rolls[0] = 0; rolls[1] = 0x8000; msg_collision__12ShockOfFrostP10GameObject(object(0), object(1));
    if (calls != "rand;amount;rand;resolve;") { std::puts(calls.c_str()); return 73; }
    if ((r = check_hit(0.0f, 109))) return 80 + r;
    // a larger amount shifts the second halfword by the truncated spread: n = 1000 -> 1000 + int(1000 * 0.2 * 0.5)
    setup(false, 0x00, 0); amount_n = 1000; msg_collision__12ShockOfFrostP10GameObject(object(0), object(1));
    {
        const short* e0 = reinterpret_cast<const short*>(r_desc + 36);
        if (e0[1] != 1000 || e0[2] != 1000 + static_cast<int>(1000.0f * bits(0x3e4ccccc) * (static_cast<float>(0x8000) * bits(0x37800000u)))) return 90;
    }
    return 0;
}
