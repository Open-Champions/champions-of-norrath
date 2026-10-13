// Observed msg_getPokeName__8RollDoorPUsi at 0x0021c4a0, 60 bytes. Looks up the text for the
// fixed key "OpenDoor" with textGetString__FPc, copies it into the 16-bit-character buffer given
// as the second argument with ustrcpy__FPUsT0 and returns 1. Nothing here ran on the EE.
extern "C" {
unsigned short* textGetString__FPc(const char* name);
void ustrcpy__FPUsT0(unsigned short* to, unsigned short* from);

int msg_getPokeName__8RollDoorPUsi(void*, unsigned short* out) {
    ustrcpy__FPUsT0(out, textGetString__FPc("OpenDoor"));
    return 1;
}
}
