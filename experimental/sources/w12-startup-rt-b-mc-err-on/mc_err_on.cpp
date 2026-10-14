// Observed mcErrOn: a 4-byte object at 0x01190c30 (symbol table size 4). The address lies inside the
// executable's .srs section (0x01000000..0x013590ac, which is loaded from the file), and the four bytes
// there are zero in the file image, so the initial value is 0. The only reader seen is the window
// SetFocus routine (w10-vi-c-wnd-setfocus), which tests it against zero and, when it is set, prints a
// diagnostic about the window. No writer has been found among the authored sources.
// The object belongs to the .srs overlay data; this unit supplies only the storage and its initial
// value, not its position in the .srs layout. Its meaning and type beyond "int flag, read as zero or
// not" are not established. Nothing was executed on the EE.
extern "C" {
int mcErrOn = 0;
}
