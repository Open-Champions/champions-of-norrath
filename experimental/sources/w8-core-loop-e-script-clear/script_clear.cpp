// Observed scriptClear__Fv at 0x001d4000, 16 bytes.
// It stores zero to nmDeferedCalls, nextNmDeferedCalls and scriptFrame, in
// that order, and returns. All three are GP-relative four-byte words
// (gp-3672, gp-3668, gp-10600). Their storage is not defined here.
extern "C" int nmDeferedCalls;
extern "C" int nextNmDeferedCalls;
extern "C" int scriptFrame;

extern "C" void scriptClear__Fv() {
    nmDeferedCalls = 0;
    nextNmDeferedCalls = 0;
    scriptFrame = 0;
}
