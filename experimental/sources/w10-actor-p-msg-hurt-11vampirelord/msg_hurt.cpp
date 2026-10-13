// Observed msg_hurt__11VampireLordfP10GameObjecti at 0x00243628, 172 bytes. Calls the Creature
// version, msg_hurt__8CreaturefP10GameObjecti, with the same four arguments (its result is
// unused). Then, unconditionally, copies the 12-byte text "VampireLord" (with terminator, padded
// to 12) into bossMeterName and stores the float at object+560 converted to an integer into
// bossMeterCurrent. Finally, when that float is below half the float at object+564 and the word
// at object+1008 is zero, it calls the table entry at byte offset 184 of the table at object+72
// with the argument 21, and sets the word at +1008 to 1.
// Inferred: +560/+564 as a current and a maximum value (they are compared and shown on the boss
// meter). Unknown: what table entry 184 does and what +1008 records. Nothing here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* source, int kind);
}

extern "C" void msg_hurt__11VampireLordfP10GameObjecti(void* self, float amount, void* source, int kind) {
    using namespace w10p;
    unsigned char* const o = static_cast<unsigned char*>(self);
    msg_hurt__8CreaturefP10GameObjecti(self, amount, source, kind);
    __builtin_memcpy(bossMeterName, "VampireLord", 12);
    const float current = real(o, 560);
    const float limit = real(o, 564) * 0.5f;
    bossMeterCurrent = static_cast<int>(current);
    if (current < limit && word(o, 1008) == 0u) {
        call_entry(o, 184, 21);
        word(o, 1008) = 1u;
    }
}
