// Observed initial bytes in the pinned SLUS_205.65, all zero:
//   dialogCreature      0x00485008, 4 bytes (small data)
//   dialogIdleAnimation 0x00485014, 4 bytes (small data)
//   dialogMutex         0x00485080, 4 bytes (small data)
//   DLG_NewWorld        0x0042a4c0, 64 bytes (.data)
//   dlgArrayPtr         0x00486b7c, 4 bytes (small bss)
//   dlgPos              0x00486b84, 4 bytes (small bss; curDlg is the word before)
//   dlgName             0x007d0cc8, bss; the next symbol, anim, starts 256 bytes on
// Inferred: dlgName spans up to 256 bytes (the gap to the next symbol; its
// elements are halfwords at the observed accesses); dlgPos is an int; the
// two pointer-like words hold 32-bit addresses on the EE.
// Unknown: the exact declared extent of dlgName and DLG_NewWorld beyond the
// gaps above, and whether the first three live in .sdata or .sbss originally
// (zero-valued objects land in bss with this compiler).
extern "C" {
unsigned dialogCreature = 0;
unsigned dialogIdleAnimation = 0;
int dialogMutex = 0;
char DLG_NewWorld[64] = {};
unsigned dlgArrayPtr = 0;
int dlgPos = 0;
unsigned short dlgName[128] = {};
}
