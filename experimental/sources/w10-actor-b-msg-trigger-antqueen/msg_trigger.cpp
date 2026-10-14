#include "../w10-actor-b-common/fields.hpp"
// Observed msg_trigger__8AntQueeni at 0x2089a0, 136 bytes. The int argument is not read.
// In order: the text "AntQueen" is copied into the boss-meter name buffer; the single at
// object+560, converted to an integer, goes to bossMeterCurrent; soundResetStream(0);
// eRand(object) masked with 3, plus one, is formatted with sprintf into a local buffer
// by the format "mx_boss_generic%d_l.vag"; soundStartMusic(buffer, 259).
// Inferred: this is the boss-fight start; +560 is current health.
// Unknown: what stream 0 is, and what the 259 flags select.
extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void soundResetStream__Fi(int stream);
unsigned eRand__10GameObject(void* self);
int sprintf(char* buffer, const char* format, ...);
void soundStartMusic__FPci(char* name, int flags);
}

extern "C" void msg_trigger__8AntQueeni(void* self, int trigger) {
    using namespace actor_b;
    (void)trigger;
    char name[32];
    __builtin_strcpy(bossMeterName, "AntQueen");
    bossMeterCurrent = static_cast<int>(field<Single>(self, 560));
    soundResetStream__Fi(0);
    sprintf(name, "mx_boss_generic%d_l.vag", static_cast<int>(eRand__10GameObject(self) & 3u) + 1);
    soundStartMusic__FPci(name, 259);
}
