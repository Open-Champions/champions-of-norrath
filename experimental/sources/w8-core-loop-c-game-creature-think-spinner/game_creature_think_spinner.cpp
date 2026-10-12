// gameCreatureThinkSpinner at 0x00484f98, 1 bytes, in SLUS_205.65.
// Observed: zero-initial byte in .data, read with an unsigned byte load and written with byte stores. Unknown: what the spinner selects.
extern "C" {
unsigned char gameCreatureThinkSpinner = 0;
}
