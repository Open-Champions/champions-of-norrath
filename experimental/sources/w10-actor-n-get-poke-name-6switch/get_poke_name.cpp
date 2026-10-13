// Observed msg_getPokeName__6SwitchPUsi at 0x0021be40, 60 bytes. Looks up the text for the key
// "FlipSwitch" with textGetString__FPc, copies that text into the caller's halfword buffer (the
// first argument after the object) with ustrcpy__FPUsT0, and returns 1. The object and the
// integer argument are not read. Unknown: the length the caller reserves for the buffer.
// Nothing here ran on the EE.
extern "C" {
unsigned short* textGetString__FPc(const char* key);
void ustrcpy__FPUsT0(unsigned short* destination, unsigned short* source);
}

extern "C" int msg_getPokeName__6SwitchPUsi(void* self, unsigned short* buffer, int) {
    ustrcpy__FPUsT0(buffer, textGetString__FPc("FlipSwitch"));
    return 1;
}
