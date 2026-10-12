// Observed animLoadingComplete at 0x0048501d: 1 byte of initialized data, value 1 in the file. runDialog, dialogMsgCallback and playSoundForDlg read it as a
// signed byte; playSoundForDlg also stores a byte.
// Inferred: nothing beyond those accesses. No field meaning is claimed.
// Unknown: what the value represents and its full range.
extern "C" {
signed char animLoadingComplete = 1;
}
