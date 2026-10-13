// Observed msg_trigger__12UndeadKnighti at 0x0023f8f8, 152 bytes. The integer argument is not read.
// In this order: the 13 bytes of "UndeadKnight" (with terminator) are copied into bossMeterName; the
// float at object+560 truncated to an int goes to bossMeterCurrent; soundResetStream(0); a random
// number r = eRand(object) picks the file name sprintf("mx_boss_generic%d_l.vag", (r & 3) + 1) into a
// stack buffer; soundStartMusic(that name, 259).
// Unknown: what the trigger value distinguishes elsewhere (it is ignored here), and the sound
// arguments beyond the 259 passed through. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void soundResetStream__Fi(int stream);
unsigned eRand__10GameObject(void* object);
int sprintf(char* buffer, const char* format, ...);
void soundStartMusic__FPci(char* name, int value);

void msg_trigger__12UndeadKnighti(void* self, int value) {
    using namespace w10o;
    (void)value;
    char name[32];
    __builtin_memcpy(bossMeterName, "UndeadKnight", 13);
    bossMeterCurrent = static_cast<int>(real(static_cast<unsigned char*>(self), 560));
    soundResetStream__Fi(0);
    sprintf(name, "mx_boss_generic%d_l.vag", static_cast<int>(eRand__10GameObject(self) & 3u) + 1);
    soundStartMusic__FPci(name, 259);
}
}
