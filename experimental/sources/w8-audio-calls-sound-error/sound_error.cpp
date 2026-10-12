/* soundError__FPc at 0x0024f9a0 is 64 bytes. It prints the text with
 * scePrintf using the format "SOUND ERROR: %s\n" (rodata 0x004747b0), then
 * calls the assert routine with file "sound.cc", line 1904, condition "0",
 * and the live return register (the address after the scePrintf call). The
 * assert is reached on every call, so this is a reporting path that does
 * not return normally unless the assert routine returns.
 */
extern "C" {
void scePrintf(const char* format, ...);
void norrath_sound_assert(const char* file, unsigned line, const char* condition, int captured_ra);

void soundError__FPc(char* text) {
    scePrintf("SOUND ERROR: %s\n", text);
    int captured;
#if defined(NORRATH_HOST_CONTRACT)
    captured = 0x24f9b8;
#else
    asm volatile("move %0,$31" : "=r"(captured)::"memory");
#endif
    norrath_sound_assert("sound.cc", 1904, "0", captured);
}
}
