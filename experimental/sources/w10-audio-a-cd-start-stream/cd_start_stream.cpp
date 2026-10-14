// Observed: cdStartStream__FPci at 0x001667e0 is 212 bytes. Arguments are (char* name,
// int offset). It zeroes streamOutput and streamSizeLeft, then calls
// cdDirectoryLookup__FPcPiT1i(name, &sector, &size, 0). A zero result prints the format at
// 0x004633f0 ("'%s' not found\n") with the name and returns. Otherwise it sets the byte
// streaming to 1, pushes border colour 0x00ffffff, and loops on
// sceCdStStart(sector + (offset >> 11, arithmetic), &CDmode) until it returns non-zero. Each
// failed attempt calls engineSyncVBlank__Fv; if the byte powerOffMode is then non-zero it
// spins forever in a loop of five nops. Finally it pops the border colour. The size value
// from the lookup is not used.
// Inferred: nothing.
// Unknown: the layout of CDmode (only its address is passed) and the argument list of sceCdStStart
// beyond the two observed registers.

extern "C" {
extern signed char streaming;
extern signed char powerOffMode;
extern char* streamOutput;
extern int streamSizeLeft;
extern char CDmode;
int cdDirectoryLookup__FPcPiT1i(char* name, int* sector, int* size, int mode);
int scePrintf(const char* format, ...);
void enginePushBorderColor__FUi(unsigned color);
void enginePopBorderColor__Fv();
void engineSyncVBlank__Fv();
int sceCdStStart(int sector, char* mode);
}

extern "C" void cdStartStream__FPci(char* name, int offset) {
    int size;
    int sector;
    streamOutput = 0;
    streamSizeLeft = 0;
    if (cdDirectoryLookup__FPcPiT1i(name, &sector, &size, 0) == 0) {
        scePrintf("'%s' not found\n", name);
        return;
    }
    streaming = 1;
    enginePushBorderColor__FUi(0x00ffffffu);
    const int sector_shift = offset >> 11;
    while (sceCdStStart(sector + sector_shift, &CDmode) == 0) {
        engineSyncVBlank__Fv();
        if (powerOffMode != 0) {
            for (;;) {
                __asm__ volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop");
            }
        }
    }
    enginePopBorderColor__Fv();
}
