// Observed msg_poke__6PhPropi at 0x00154ab8, 240 bytes. who is the second argument. The item record is at
// object+328; the doubleword at +336 is the flag word tested here.
//   Bit 39 (0x8000000000) set: for every player index 0 .. nmPlayers-1 (nmPlayers re-read after each
//   step) playerMaybeGetItem__FiPQ24item11description(index, object+328) is called; if any call returned
//   non-zero, the object's own table entry at (word +72) + 8 is called, on the object adjusted by that
//   entry's halfword, with the argument 3.
//   Bit 39 clear: the same call to playerMaybeGetItem is made once with `who`, and the table entry is
//   called with 3 when it returned non-zero.
// The table call is skipped when the object pointer is null (the object pointer was dereferenced before,
// so the test is spelled out with a barrier). Nothing is returned. Inferred: the table entry at +8 is the
// object's reaction to being picked up (name only); the bit selects "everyone gets a try" versus "just
// the poker". Unknown: what the bit means. Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
extern int nmPlayers;
int playerMaybeGetItem__FiPQ24item11description(int player, void* description);

void msg_poke__6PhPropi(void* self, int who) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    bool taken;
    if (dword(o, 336) & 0x8000000000ull) {
        int any = 0;
        for (int index = 0; index < nmPlayers; ++index)
            if (playerMaybeGetItem__FiPQ24item11description(index, o + 328)) any = 1;
        taken = any != 0;
    } else {
        taken = playerMaybeGetItem__FiPQ24item11description(who, o + 328) != 0;
    }
    if (!taken) return;
    __asm__("" : "+r"(o));
    if (o) call_entry(o, 8, 3);
}
}
