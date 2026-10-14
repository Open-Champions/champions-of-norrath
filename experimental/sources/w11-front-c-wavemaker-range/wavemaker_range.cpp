// Observed range at 0x00601030, bss, symbol size 0; splashInfoBits begins 4000 bytes later,
// so 4000 bytes are reserved (the gap, not a measured size). The WaveMaker constructor and
// msg_run index it by a byte offset and read one unsigned byte. Unknown: the index meaning.
extern "C" {
unsigned char range[4000];
}
