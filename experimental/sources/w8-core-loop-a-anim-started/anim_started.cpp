// Observed animStarted at 0x00485020: 1 byte of initialized data, value 0 in the file. runDialog reads it mostly as an unsigned byte and stores one
// byte; playSoundForDlg stores one byte.
// Inferred: nothing beyond those accesses. No field meaning is claimed.
// Unknown: what the value represents and its full range.
extern "C" {
unsigned char animStarted = 0;
}
