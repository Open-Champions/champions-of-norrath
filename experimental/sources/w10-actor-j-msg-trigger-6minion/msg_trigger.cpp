// Observed msg_trigger__6Minioni at 0x00235338, 120 bytes. The integer argument is unused.
// Order of events: the 14 bytes "GreaterMinion" plus its terminator are copied into the
// start of the boss-meter name buffer (bossMeterName, 128 bytes at 0x0034b988); the float at
// object+560 is truncated to an integer and stored in bossMeterCurrent (0x00483f48); the
// sound stream is reset with 0; then the music "mx_boss_act4_grdemon_1.vag" is started
// with the value 259. The meaning of those values is unknown beyond the symbol names in the
// executable. Nothing here ran on the EE.
extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void soundResetStream__Fi(int stream);
void soundStartMusic__FPci(char* name, int value);

void msg_trigger__6Minioni(void* self, int) {
    __builtin_memcpy(bossMeterName, "GreaterMinion", 14);
    bossMeterCurrent = static_cast<int>(*reinterpret_cast<float*>(static_cast<unsigned char*>(self) + 560));
    soundResetStream__Fi(0);
    soundStartMusic__FPci(const_cast<char*>("mx_boss_act4_grdemon_1.vag"), 259);
}
}
