// Host check only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include "init_type.cpp"

static int rolls;
static unsigned next_roll;
static void* roll_arg;
extern "C" unsigned eRand__10GameObject(void* self) { ++rolls; roll_arg = self; return next_roll; }

int main() {
    int bad = 0;
    const unsigned rolls_to_try[] = {0u, 1u, 2u, 3u, 8u, 9u, 10u, 11u, 12u, 13u, 100u, 0x7fffffffu, 0x80000000u, 0xffffffffu};
    const unsigned variants[] = {0u, 1u, 2u, 7u, 0xffffffffu};
    for (unsigned variant : variants)
        for (unsigned roll : rolls_to_try) {
            unsigned char object[1024];
            std::memset(object, 0x44, sizeof object);
            unsigned char before[1024];
            write_word(object + 964, variant);
            std::memcpy(before, object, sizeof object);
            rolls = 0; next_roll = roll;
            initType__5Gnome(object);
            unsigned want;
            if (variant == 0) want = 1u << (14 + roll % 9);
            else if (variant == 1) want = 1u << (12 + (roll & 1));
            else want = 1u << (roll % 12);
            unsigned got = read_word(object + 244);
            std::memcpy(object + 244, before + 244, 4);
            if (got != want) { std::printf("variant %u roll %u: %08x want %08x\n", variant, roll, got, want); ++bad; }
            if (rolls != 1 || roll_arg != object) { std::printf("one roll with the object\n"); ++bad; }
            if (std::memcmp(object, before, sizeof object) != 0) { std::printf("other bytes changed\n"); ++bad; }
        }
    std::printf(bad ? "failed\n" : "initType__5Gnome cases passed\n");
    return bad != 0;
}
