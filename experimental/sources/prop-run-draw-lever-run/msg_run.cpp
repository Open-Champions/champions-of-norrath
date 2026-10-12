#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__5Lever at 0x215e28, 292 bytes.
// A zero signed byte at object+81 returns. Otherwise the single at object+76
// gains (signed byte object+80 as a single * 1.5) * K0 and is stored. A result
// of 1.0 or more stores 1.0 and marks the move done. The stored value below
// 0.0 stores 0.0 and marks it done. When not done the method returns.
// When done, objectRemoveFromRunList runs and object+81 is cleared. The
// argument is true when the signed byte at object+80 is 1. findTagInt for the
// tag "onOnly" (occurrence 0) found sets the argument to 1 and a seen flag.
// "offOnly" found then sets the argument to 0 and the seen flag. The object
// is passed to objectDoMyActions with the argument, the byte at object+80 is
// negated, and, when neither tag was found, bit 0x100000 of the word at
// object+12 is set.
// Observed: K0 is the .lit4 word 0x3c888888 at 0x481598. The tag strings are
// at 0x4853a0 and 0x4853a8 in .sdata. norrath_game_object_find_tag_int is the
// existing findTagInt body. Unknown: what object+76/+80/+81 and the flag mean.
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
extern "C" void objectRemoveFromRunList__FP10GameObject(void* object);
extern "C" void objectDoMyActions__FP10GameObjecti(void* object, int value);

extern "C" void msg_run__5Lever(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    if (static_cast<signed char>(read_u8(base + 81u)) == 0) return;
    const float k0 = 0x1.111110p-6f;
    const int direction = static_cast<signed char>(read_u8(base + 80u));
    bool done = false;
    const float next = read_single(base + 76u) + static_cast<float>(direction) * 1.5f * k0;
    write_single(base + 76u, next);
    if (1.0f <= next) {
        write_single(base + 76u, 1.0f);
        done = true;
    }
    if (read_single(base + 76u) < 0.0f) {
        write_single(base + 76u, 0.0f);
        done = true;
    }
    if (!done) return;
    objectRemoveFromRunList__FP10GameObject(self);
    write_u8(base + 81u, 0u);
    static char on_only[] = {'o', 'n', 'O', 'n', 'l', 'y', '\0'};
    static char off_only[] = {'o', 'f', 'f', 'O', 'n', 'l', 'y', '\0'};
    int argument = direction == 1 ? 1 : 0;
    bool seen = false;
    if (norrath_game_object_find_tag_int(self, on_only, 0) != 0) {
        argument = 1;
        seen = true;
    }
    if (norrath_game_object_find_tag_int(self, off_only, 0) != 0) {
        argument = 0;
        seen = true;
    }
    objectDoMyActions__FP10GameObjecti(self, argument);
    write_u8(base + 80u, static_cast<unsigned>(-static_cast<int>(static_cast<signed char>(read_u8(base + 80u)))));
    if (!seen) write_word(base + 12u, read_word(base + 12u) | 0x100000u);
}
