// Observed stateData at 0x006e1c40 in .bss, 32 bytes (the next symbol, rumphole,
// starts 32 bytes later). padInit clears it with four memset calls of 8 bytes
// each (per subsystem-inits notes), so it is four 8-byte records of zero here.
// The record fields are not established.
extern "C" {
unsigned char stateData[32];
}
