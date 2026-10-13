// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "set_weapons.cpp"
#include <cstdio>
#include <cstring>
static int rand_calls = 0;
static void* rand_object = nullptr;
static unsigned rand_value = 0;
extern "C" unsigned eRand__10GameObject(void* object) { ++rand_calls; rand_object = object; return rand_value; }
int main() {
    int bad = 0;
    const unsigned rolls[] = {0u, 1u, 2u, 3u, 4u, 0xfffffffeu, 0xffffffffu, 0x80000001u, 70000u};
    for (unsigned variant = 0; variant < 9; ++variant) {
        for (unsigned roll : rolls) {
            unsigned char object[1000];
            std::memset(object, 0x5a, sizeof object);
            rtti_c::write_word(rtti_c::address(object) + 972u, variant == 8 ? 0xffffffffu : variant);
            rtti_c::write_word(rtti_c::address(object) + 988u, 0x11223344u);
            rand_calls = 0; rand_value = roll; rand_object = nullptr;
            setWeapons__6Goblin(object);
            const unsigned got = rtti_c::read_word(rtti_c::address(object) + 988u);
            unsigned want = 0x11223344u;
            int want_calls = 0;
            if (variant <= 3) { want = roll % 3u + 5u; want_calls = 1; }
            else if (variant == 4) want = 3u;
            else if (variant == 5) want = 2u;
            if (got != want || rand_calls != want_calls || (want_calls && rand_object != object)) {
                std::printf("variant %u roll %u: got %u want %u\n", variant, roll, got, want); ++bad;
            }
            if (object[987] != 0x5a || object[992] != 0x5a) { std::printf("neighbours\n"); ++bad; }
        }
    }
    std::printf(bad ? "failed\n" : "setWeapons__6Goblin passed\n");
    return bad != 0;
}
