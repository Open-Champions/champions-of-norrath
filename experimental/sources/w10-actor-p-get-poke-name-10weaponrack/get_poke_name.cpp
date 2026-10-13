// Observed msg_getPokeName__10WeaponRackPUsi at 0x0021cbc8, 60 bytes. Looks up the text key
// "SearchWeaponRack" with textGetString__FPc, copies the result into the caller's buffer (the
// second argument) with ustrcpy__FPUsT0 and returns 1. The object and the third argument are
// not read. Nothing here ran on the EE.
extern "C" {
const unsigned short* textGetString__FPc(const char* key);
void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source);
}

extern "C" int msg_getPokeName__10WeaponRackPUsi(void* self, unsigned short* buffer) {
    (void)self;
    ustrcpy__FPUsT0(buffer, textGetString__FPc("SearchWeaponRack"));
    return 1;
}
