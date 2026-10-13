// Observed msg_trigger__11SpiderQueeni at 0x0020f9b8, 112 bytes. Copies the 7-byte text "Shelox"
// (with its terminator) into bossMeterName, stores the float at object+560 converted to an
// integer into bossMeterCurrent, calls soundResetStream__Fi(0), then starts the music file named
// "mx_boss_act1_spidboss_l.vag" with soundStartMusic__FPci and the integer 259. The integer
// argument is not read. Returns nothing. Unknown: the meaning of 259 and of the float at +560
// (the name suggests a meter value; that is a reading of the destination, not proven).
// Nothing here ran on the EE.
extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void soundResetStream__Fi(int stream);
void soundStartMusic__FPci(const char* name, int flags);
}

typedef float Single __attribute__((may_alias));

extern "C" void msg_trigger__11SpiderQueeni(void* self, int) {
    unsigned char* object = static_cast<unsigned char*>(self);
    __builtin_memcpy(bossMeterName, "Shelox", 7);
    bossMeterCurrent = static_cast<int>(*reinterpret_cast<Single*>(object + 560));
    soundResetStream__Fi(0);
    soundStartMusic__FPci("mx_boss_act1_spidboss_l.vag", 259);
}
