// Observed DLG_GetDialogCreature__Fv at 0x001d39a8, 20 bytes. It reads the word
// curDlg (0x00486b80) and the word dialogCreature (0x00485008) and returns the
// second when the first is nonzero, otherwise zero.
// Unknown: the type behind the dialogCreature word; it is carried as a pointer.
extern "C" {
extern int curDlg;
extern void* dialogCreature;
void* DLG_GetDialogCreature__Fv() {
    void* creature = dialogCreature;
    if (curDlg == 0) return 0;
    return creature;
}
}
