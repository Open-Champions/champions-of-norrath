// Observed InitExecPS2 at 0x002dda10. Local helpers that share names with
// other startup copies are imported under distinct names. Original local
// spellings and addresses are in docs/STARTUP_INITSYS.md.
extern "C" {
int PatchIsNeeded();
void kernel_execps2_setup(unsigned first, unsigned second);
void kernel_execps2_copy(unsigned destination, const void* source, unsigned bytes);
unsigned kernel_execps2_get_entry(unsigned identifier);
int FlushCache(int mode);
extern unsigned SysExecPS2Entry[];
extern unsigned char osdsrc[];
void InitExecPS2() {
    if (PatchIsNeeded() == 0) return;
    kernel_execps2_setup(SysExecPS2Entry[0], SysExecPS2Entry[1]);
    // Observed destination 0x80074000 and 1960 bytes, the osdsrc symbol span.
    kernel_execps2_copy(0x80074000u, osdsrc, 1960);
    FlushCache(0);
    FlushCache(2);
    kernel_execps2_setup(SysExecPS2Entry[2], SysExecPS2Entry[3]);
    unsigned* cursor = SysExecPS2Entry + 4;
    int remaining_gate = 2;
    do {
        unsigned identifier = *cursor;
        ++remaining_gate;
        unsigned entry = kernel_execps2_get_entry(identifier);
        kernel_execps2_setup(*cursor, entry);
        cursor += 2;
    } while (remaining_gate < 3);
}
}
