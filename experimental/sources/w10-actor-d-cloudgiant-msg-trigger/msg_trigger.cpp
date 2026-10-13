// Observed msg_trigger__10CloudGianti at 0x001e9aa0, 192 bytes.
// If the word at object+976 is zero, forwards (object, int) to the Creature trigger routine and
// returns. Otherwise, in order:
//  - copies the 15 bytes "CloudGiantKing" plus its terminator into the global bossMeterName;
//  - stores the float at object+560, truncated to int, in bossMeterCurrent;
//  - calls soundResetStream__Fi(0);
//  - n = eRand__10GameObject(object); formats (n & 3) + 1 into a local buffer with sprintf and the
//    format text "mx_boss_generic%d_l.vag"; calls soundStartMusic__FPci(buffer, 259).
// Inferred: this starts the boss music and meter when the fight begins. Unknown: whether 259 is a
// flag word or a channel number, and the type of +976. Nothing here ran on the EE.
extern "C" {
void msg_trigger__8Creaturei(void* self, int which);
extern char bossMeterName[];
extern int bossMeterCurrent;
void soundResetStream__Fi(int value);
int eRand__10GameObject(void* self);
int sprintf(char* destination, const char* format, ...);
void soundStartMusic__FPci(char* name, int flags);
}

extern "C" void msg_trigger__10CloudGianti(void* self, int which) {
    const unsigned char* object = static_cast<const unsigned char*>(self);
    if (*reinterpret_cast<const int*>(object + 976) == 0) {
        msg_trigger__8Creaturei(self, which);
        return;
    }
    static const char bossName[15] = {'C', 'l', 'o', 'u', 'd', 'G', 'i', 'a', 'n', 't', 'K', 'i', 'n', 'g', '\0'};
    for (int i = 0; i < 15; ++i) bossMeterName[i] = bossName[i];
    bossMeterCurrent = static_cast<int>(*reinterpret_cast<const float*>(object + 560));
    soundResetStream__Fi(0);
    char buffer[32];
    sprintf(buffer, "mx_boss_generic%d_l.vag", (eRand__10GameObject(self) & 3) + 1);
    soundStartMusic__FPci(buffer, 259);
}
