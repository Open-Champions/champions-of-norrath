// Observed issueCommand__Fv at 0x00164b40, 300 bytes (a local symbol in the
// original). Reads the word at the start of `command` (0x6d86b0). Mode 1 calls
// sceCdRead and mode 2 calls sceCdReadIOPm; any other mode does nothing. In
// both cases the border color 0x00ffffff is pushed first and popped last, and
// the call is repeated until it returns nonzero. The arguments are command
// words 1, 2 and 3 followed by the address of CDmode. After each failed attempt
// engineSyncVBlank() runs, and a nonzero powerOffMode (signed byte) then spins
// forever. The result is not used. The word meanings are not established here.
extern "C" {
extern unsigned command[];
extern unsigned char CDmode[];
extern signed char powerOffMode;
void enginePushBorderColor__FUi(unsigned color);
void enginePopBorderColor__Fv();
void engineSyncVBlank__Fv();
int sceCdRead(unsigned first, unsigned second, unsigned third, void* mode);
int sceCdReadIOPm(unsigned first, unsigned second, unsigned third, void* mode);

void issueCommand__Fv() {
    unsigned mode = command[0];
    if (mode == 1) {
        enginePushBorderColor__FUi(0xffffffu);
        while (!sceCdRead(command[1], command[2], command[3], CDmode)) {
            engineSyncVBlank__Fv();
            if (powerOffMode != 0) for (;;) {}
        }
        enginePopBorderColor__Fv();
    } else if (mode == 2) {
        enginePushBorderColor__FUi(0xffffffu);
        while (!sceCdReadIOPm(command[1], command[2], command[3], CDmode)) {
            engineSyncVBlank__Fv();
            if (powerOffMode != 0) for (;;) {}
        }
        enginePopBorderColor__Fv();
    }
}
}
