// Zero-initialised message table: inMsg at 0x008d8938, 1008 bytes up to the next symbol
// (msgTimeout at 0x008d8d28). The importers treat it as eight entries of 126 bytes
// (1008 = 8 * 126). It is BSS in the original and zero here.
// Unknown: the entry layout; this unit defines only the storage extent.
extern "C" {
unsigned char inMsg[1008];
}
