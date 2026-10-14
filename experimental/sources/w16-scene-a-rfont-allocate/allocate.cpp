// Observed: Allocate__7VIRFont at 0x01136250 is 72 bytes in the pinned SLUS-20565 image. It reads the word at offset 80. If that is zero it
// returns zero without allocating. Otherwise it calls __builtin_vec_new(size), stores the result at offset 84 (even when it is null) and
// returns -1 if the result is null, else zero.
// Inferred: +80 is a byte size computed earlier (CalcDataSize__7VIRFont) and +84 the block that Init__7VIRFont... then uses as the base
// of four internal tables.
// Unknown: the allocator's behaviour (an import), alignment, and who frees the block. Nothing here ran on the EE.
extern "C" {
unsigned __builtin_vec_new(unsigned size);

int Allocate__7VIRFont(unsigned char* self) {
    unsigned size = *reinterpret_cast<unsigned*>(self + 80);
    if (size != 0) {
        unsigned block = __builtin_vec_new(size);
        *reinterpret_cast<unsigned*>(self + 84) = block;
        if (block == 0) return -1;
    }
    return 0;
}
}
