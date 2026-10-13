// Observed decodeQ at 0x006e9180 (.bss), 512 bytes. The span to the next symbol (loadKillList) is 512 bytes. The authored texture-decompress thread indexes it in 8-byte entries with indices masked by 63.
// Inferred: 64 entries of {texture pointer, second word}.
// Unknown: The meaning of the second word.
extern "C" {
unsigned char decodeQ[512];
}
