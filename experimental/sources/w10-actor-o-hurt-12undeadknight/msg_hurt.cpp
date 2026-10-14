// Observed msg_hurt__12UndeadKnightfP10GameObjecti at 0x0023f990, 104 bytes. Forwards its arguments
// unchanged to msg_hurt__8CreaturefP10GameObjecti, then copies the 13 bytes of the class name text
// "UndeadKnight" (with terminator, at 0x004732a8 in the image) into bossMeterName and stores the
// float at object+560 truncated to an int in bossMeterCurrent. Both are the same two globals other
// boss classes update on a hit.
// Unknown: what the two globals feed. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void msg_hurt__8CreaturefP10GameObjecti(void* self, float damage, void* attacker, int flag);

void msg_hurt__12UndeadKnightfP10GameObjecti(void* self, float damage, void* attacker, int flag) {
    using namespace w10o;
    msg_hurt__8CreaturefP10GameObjecti(self, damage, attacker, flag);
    __builtin_memcpy(bossMeterName, "UndeadKnight", 13);
    bossMeterCurrent = static_cast<int>(real(static_cast<unsigned char*>(self), 560));
}
}
