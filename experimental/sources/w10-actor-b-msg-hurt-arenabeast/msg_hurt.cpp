#include "../w10-actor-b-common/fields.hpp"
// Observed msg_hurt__10ArenabeastfP10GameObjecti at 0x236a08, 100 bytes. Calls
// Creature::hurt with the same float, object and int; then copies the text "ArenaBeast"
// into the boss-meter name buffer and stores the single at object+560, converted to an
// integer, into bossMeterCurrent.
// Inferred: +560 is the creature's current health. Unknown: the unit of the meter.
extern "C" {
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* source, int kind);
extern char bossMeterName[];
extern int bossMeterCurrent;
}

extern "C" void msg_hurt__10ArenabeastfP10GameObjecti(void* self, float amount, void* source, int kind) {
    using namespace actor_b;
    msg_hurt__8CreaturefP10GameObjecti(self, amount, source, kind);
    __builtin_strcpy(bossMeterName, "ArenaBeast");
    bossMeterCurrent = static_cast<int>(field<Single>(self, 560));
}
