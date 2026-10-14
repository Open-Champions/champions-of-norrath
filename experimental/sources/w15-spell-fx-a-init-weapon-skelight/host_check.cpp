// Host contract for initWeapon__8Skelight; not an EE input. 32-bit words are offsets from a host base and the table call is
// routed through a logging function. The authored function was also compared with the original in a private R5900 run (150
// scenarios over the selector values 0 to 4 and -1, several base values, rolls and difficulty values; every original word
// covered except the 21 words of the slot-scan fallthrough, which cannot run because all four slots are cleared first).
#define W12_SPELL_C_HOST 1
#include "init_weapon.cpp"
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
unsigned skills[16];
float worldDifficulty;
static std::string calls;
static int base_value, roll_value;
unsigned w12_spell_c_host_call(unsigned fn, void* object, int count, unsigned long a1, unsigned long a2) {
    calls += "call" + std::to_string(fn) + "," + std::to_string(count) + "," + std::to_string(a1) + "," + std::to_string(a2) + ";";
    (void)object;
    return static_cast<unsigned>(base_value);
}
extern "C" int eRand__10GameObject(void*) { calls += "rand;"; return roll_value; }
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static short geth(unsigned off) { short v; std::memcpy(&v, arena + off, 2); return v; }
enum { O = 0x100, SK = 0x1000, T = 0x1400 };
static void reset(int mode, float difficulty) {
    std::memset(arena, 0, sizeof arena);
    calls.clear();
    put32(O + 208, static_cast<unsigned>(mode));
    arena[O + 156] = 0xaa;
    for (int i = 0; i < 16; ++i) skills[i] = SK + 0x40 * i;
    for (int i = 0; i < 16; ++i) put32(SK + 0x40 * i + 96, T);
    const short adjust = 0;
    std::memcpy(arena + T + 96, &adjust, 2);
    put32(T + 96 + 4, 7);
    worldDifficulty = difficulty;
}
static int expected(int base, int roll) {
    const float k = __builtin_bit_cast(float, 0x3e4cccccu);
    const float scale = __builtin_bit_cast(float, 0x37800000u);
    return base + static_cast<int>((static_cast<float>(base) * k) * (static_cast<float>(roll) * scale));
}
int main() {
    w12_spell_c_host_base = arena;
    const unsigned D = O + 1032 + 36;  // first slot
    // case 1: kind 21, flag 2. base 1000, difficulty 3.5 (truncates to 3), roll 0 leaves the amount at the base.
    reset(1, 3.5f); base_value = 1000; roll_value = 0;
    initWeapon__8Skelight(arena + O);
    if (calls != "call7,2,0,3;rand;" || arena[O + 156] != 2) return 1;
    if (geth(D) != 21 || geth(D + 2) != 1000 || geth(D + 4) != 1000 || geth(D + 6) != 0) return 2;
    if (geth(D + 8) != -1 || geth(D + 16) != -1 || geth(D + 24) != -1) return 3;
    // case 2: kind 22, flag 3. amount = base + (int)((float)base * 0.2f * ((float)roll * 2^-16)) with the pool's 0.2 (bits 0x3e4ccccc).
    reset(2, 0.0f); base_value = 1000; roll_value = 65536;
    initWeapon__8Skelight(arena + O);
    if (arena[O + 156] != 3 || geth(D) != 22 || geth(D + 2) != 1000 || geth(D + 4) != expected(1000, 65536) || expected(1000, 65536) < 1199 || expected(1000, 65536) > 1200) return 4;
    // case 3: kind 24, flag 1.
    reset(3, 0.0f); base_value = 50; roll_value = 32768;
    initWeapon__8Skelight(arena + O);
    if (arena[O + 156] != 1 || geth(D) != 24 || geth(D + 2) != 50 || geth(D + 4) != expected(50, 32768)) return 5;
    // any other selector leaves the slots cleared and the flag byte at zero
    reset(4, 0.0f); base_value = 50; roll_value = 0;
    initWeapon__8Skelight(arena + O);
    if (!calls.empty() || arena[O + 156] != 0 || geth(D) != -1 || geth(D + 24) != -1) return 6;
    reset(-1, 0.0f);
    initWeapon__8Skelight(arena + O);
    return calls.empty() && arena[O + 156] == 0 ? 0 : 7;
}
