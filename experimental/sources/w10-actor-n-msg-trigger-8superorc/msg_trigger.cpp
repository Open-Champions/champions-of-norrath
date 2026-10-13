// Observed msg_trigger__8SuperOrci at 0x0022b9d8, 136 bytes. Copies the 9-byte text "SuperOrc"
// (with terminator) into bossMeterName, stores the float at object+560 converted to an integer
// into bossMeterCurrent, calls soundResetStream__Fi(0), draws eRand__10GameObject on the object,
// formats "mx_boss_generic%d_l.vag" with the value (roll & 3) + 1 into a stack buffer with
// sprintf, and starts that music file with soundStartMusic__FPci and the integer 259. The integer
// argument is not read. Returns nothing. Unknown: the meaning of 259 and of the float at +560.
// Nothing here ran on the EE.
extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void soundResetStream__Fi(int stream);
unsigned eRand__10GameObject(void* self);
int sprintf(char* out, const char* format, ...);
void soundStartMusic__FPci(const char* name, int flags);
}

typedef float Single __attribute__((may_alias));

extern "C" void msg_trigger__8SuperOrci(void* self, int) {
    unsigned char* object = static_cast<unsigned char*>(self);
    char name[32];
    __builtin_memcpy(bossMeterName, "SuperOrc", 9);
    bossMeterCurrent = static_cast<int>(*reinterpret_cast<Single*>(object + 560));
    soundResetStream__Fi(0);
    unsigned roll = eRand__10GameObject(self);
    sprintf(name, "mx_boss_generic%d_l.vag", static_cast<int>(roll & 3u) + 1);
    soundStartMusic__FPci(name, 259);
}
