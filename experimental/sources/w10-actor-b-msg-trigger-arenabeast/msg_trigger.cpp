#include "../w10-actor-b-common/fields.hpp"
// Observed msg_trigger__10Arenabeasti at 0x236998, 112 bytes. The int argument is not read.
// In order: the text "ArenaBeast" is copied into the boss-meter name buffer; the single at
// object+560, converted to an integer, goes to bossMeterCurrent; soundResetStream(0);
// soundStartMusic("mx_boss_act4_arenabeast_l.vag", 259).
// Inferred: this is the boss-fight start; +560 is current health.
// Unknown: what stream 0 is, and what the 259 flags select.
extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void soundResetStream__Fi(int stream);
void soundStartMusic__FPci(char* name, int flags);
}

extern "C" void msg_trigger__10Arenabeasti(void* self, int trigger) {
    using namespace actor_b;
    (void)trigger;
    __builtin_strcpy(bossMeterName, "ArenaBeast");
    bossMeterCurrent = static_cast<int>(field<Single>(self, 560));
    soundResetStream__Fi(0);
    soundStartMusic__FPci(const_cast<char*>("mx_boss_act4_arenabeast_l.vag"), 259);
}
