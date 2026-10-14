// Observed soundMakeSequenceUnpausable__Fi at 0x0024f9e0, 64 bytes.
// An argument of -1 returns. Bit 0x8000 must be set. The low byte selects a
// 76-byte record of sequence at 0x007d8110, and byte 59 of that record
// becomes 1.
extern "C" {
extern unsigned char sequence[];

void soundMakeSequenceUnpausable__Fi(int id) {
    if (id == -1 || (id & 0x8000) == 0) return;
    sequence[(id & 0xff) * 76 + 59] = 1;
}
}
