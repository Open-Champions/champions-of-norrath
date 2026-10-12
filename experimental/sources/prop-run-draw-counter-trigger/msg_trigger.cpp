#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_trigger__7Counteri at 0x2162a0, 224 bytes.
// The limit starts at 1. When findTagString for the tag "count" (occurrence
// 0) is non-null, the limit is findTagInt of the same tag and occurrence.
// A nonzero argument adds one to the word at object+76 and reaches the goal
// when the new word equals the limit. A zero argument subtracts one and
// reaches the goal when the word before the subtraction equals the limit.
// Without the goal the method returns. With it, findTagInt reads "onOnly" and
// "offOnly" (occurrence 0 each), and objectDoMyActions gets the object and
// the argument: 0 when "offOnly" is non-zero, else 1 when "onOnly" is
// non-zero, else the trigger argument itself.
// Observed: the tag strings are at 0x4853e8, 0x4853a0 and 0x4853a8 in .sdata.
// norrath_actor_find_tag_string and norrath_game_object_find_tag_int are the
// existing findTagString and findTagInt bodies. Unknown: what the word at
// object+76 counts.
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
extern "C" void objectDoMyActions__FP10GameObjecti(void* object, int value);

extern "C" void msg_trigger__7Counteri(void* self, int value) {
    using namespace vslots_b;
    const Address base = address(self);
    static char count[] = {'c', 'o', 'u', 'n', 't', '\0'};
    static char on_only[] = {'o', 'n', 'O', 'n', 'l', 'y', '\0'};
    static char off_only[] = {'o', 'f', 'f', 'O', 'n', 'l', 'y', '\0'};
    int limit = 1;
    if (norrath_actor_find_tag_string(self, count, 0) != nullptr) limit = norrath_game_object_find_tag_int(self, count, 0);
    bool goal;
    if (value != 0) {
        const unsigned next = read_word(base + 76u) + 1u;
        write_word(base + 76u, next);
        goal = static_cast<int>(next) == limit;
    } else {
        const unsigned previous = read_word(base + 76u);
        write_word(base + 76u, previous - 1u);
        goal = static_cast<int>(previous) == limit;
    }
    if (!goal) return;
    const int on = norrath_game_object_find_tag_int(self, on_only, 0);
    const int off = norrath_game_object_find_tag_int(self, off_only, 0);
    int argument = on != 0 ? 1 : value;
    if (off != 0) argument = 0;
    objectDoMyActions__FP10GameObjecti(self, argument);
}
