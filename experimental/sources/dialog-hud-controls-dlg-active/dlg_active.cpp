// Observed DLG_Active__Fv at 0x001d38b0, 12 bytes.
// It loads the word at curDlg and returns 1 when that word is nonzero.
// The compare is unsigned. dlgPos begins four bytes after curDlg. The ELF
// size of curDlg is 0. curDlg is not defined here.
extern "C" unsigned curDlg;

extern "C" int DLG_Active__Fv() {
    return curDlg != 0u ? 1 : 0;
}
