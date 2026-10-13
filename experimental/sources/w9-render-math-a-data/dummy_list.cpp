// Observed dummyList at 0x008d5d70. pnums begins 0x30 bytes later. The authored select
// routine indexes it as four 12-byte records (name pointer, number pointer, zero word).
// The 48-byte extent is the symbol gap and agrees with that stride.
extern "C" {
unsigned char dummyList[48];
}
