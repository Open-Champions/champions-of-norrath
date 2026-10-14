// Observed: McErrSetMode__Fi at 0x0109fc70 is 56 bytes. With (mode) it calls
// scePrintf("McErrSetMode (Mode = %d)\n", mode) and stores mode in the word at +432 of the global
// memory-card error window object sWndMcErr (0x01190a20). Nothing is returned.
// Inferred: the word at +432 is the error window's mode; it is the same word McErr clears (to 0) when it
// opens the window.
// Unknown: what the individual mode values mean and the field's name.
extern "C" {
int scePrintf(const char* format, ...);
extern unsigned char sWndMcErr[];

void McErrSetMode__Fi(int mode) {
    scePrintf("McErrSetMode (Mode = %d)\n", mode);
    *reinterpret_cast<int*>(sWndMcErr + 432) = mode;
}
}
