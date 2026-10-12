// Observed DLG_Clear__Fv at 0x001d3830, 128 bytes, pinned SLUS_205.65.
// Observed: two 16-byte records starting at the ELF symbol `anim`
// (0x007d0dc8) are visited in order. Each record whose first word is nonzero
// has that word passed to memFree__FPv and then zeroed. Afterwards ten words
// are stored: dialogCreature, dialogLumpName, dialogMutex, curDlg,
// activePlayer, optionsShowing, swooping and DLG_SwoopMode become 0, while
// realDlgPos and dlgPos become -1.
// Inferred: the first word of each record is a heap block pointer. The other
// twelve bytes of each record are untouched. Not executed on hardware.
// The signedness and meaning of the cleared words beyond their stores is unknown.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#endif
extern "C" {
extern unsigned anim[];
extern int dialogCreature, dialogLumpName, dialogMutex, activePlayer, optionsShowing;
extern int swooping, DLG_SwoopMode, realDlgPos, dlgPos;
extern unsigned curDlg;
void memFree__FPv(void* block);
}

extern "C" void DLG_Clear__Fv() {
    for (int i = 0; i < 2; ++i) {
        unsigned block = anim[i * 4];
        if (block != 0) {
            memFree__FPv(NORRATH_PTR(block));
            anim[i * 4] = 0;
        }
    }
    dialogCreature = 0;
    realDlgPos = -1;
    dialogLumpName = 0;
    dialogMutex = 0;
    curDlg = 0;
    activePlayer = 0;
    dlgPos = -1;
    optionsShowing = 0;
    swooping = 0;
    DLG_SwoopMode = 0;
}
