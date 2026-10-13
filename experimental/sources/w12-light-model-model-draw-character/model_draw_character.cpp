// Observed modelDrawCharacter__FP6PacketP12_modelHeaderli at 0x0016c680, extent 192.
// Arguments: a Packet (its first word is the output cursor, its second the last tag written),
// a model header, a 64-bit mask (the 'l' in the symbol; the original shifts it with 64-bit
// instructions) and an index. Returns nothing.
// Header bytes (signed): +38 is a count, +18 is a part count p. A table of 32-bit words starts at
// header + 40; row r begins at header + 40 + r * (p * 4 + 4). The row is clamped to 0..count-1
// (with count 0 the clamp gives -1, as the original does). If p is negative the call returns.
// Bit p of the mask is cleared. With start = row[0], for i = 0..p: if bit i of the mask is
// clear and row[i] differs from start, one 16-byte tag is written at the cursor and the cursor
// moves on 16: the low doubleword is ((row[i] - start) >> 4, arithmetic, sign-extended to 64
// bits) | (start << 32) | 0x30000000, the high doubleword is 0, and packet.last is the tag
// address; then start = row[i + 1]. Bits that are set leave start unchanged, so a run of set
// bits is emitted as a single tag when the next clear bit is reached. The mask shifts right by
// one (arithmetic) every iteration.
// Inferred: tags with 0x30000000 in the low word look like DMA reference tags and the table
// words look like addresses, so each emitted tag draws one contiguous span of the model's parts.
// Unknown: the meaning of the table words, that of the 0x3 tag id field here, and why the
// last part is never drawn.
typedef unsigned long long __attribute__((may_alias)) u64;
typedef int __attribute__((may_alias)) word;
struct Packet {
    unsigned char* cursor;
    unsigned char* last;
};
extern "C" {
void modelDrawCharacter__FP6PacketP12_modelHeaderli(Packet* packet, const unsigned char* model, long long mask, int index) {
    int count = static_cast<signed char>(model[38]);
    int parts = static_cast<signed char>(model[18]);
    if (index < 0) index = 0;
    if (count - 1 < index) index = count - 1;
    if (parts < 0) return;
    mask &= ~(1LL << parts);
    const word* row = reinterpret_cast<const word*>(model + 40 + index * (parts * 4 + 4));
    unsigned start = row[0];
    for (int i = 0; i <= parts; ++i) {
        if ((mask & 1) == 0) {
            unsigned current = row[i];
            if (start != current) {
                unsigned char* tag = packet->cursor;
                long long count16 = static_cast<int>(current - start) >> 4;
                packet->cursor = tag + 16;
                *reinterpret_cast<u64*>(tag) = static_cast<u64>(count16) | (static_cast<u64>(start) << 32) | 0x30000000ull;
                *reinterpret_cast<u64*>(tag + 8) = 0;
                packet->last = tag;
            }
            start = row[i + 1];
        }
        mask >>= 1;
    }
}
}
