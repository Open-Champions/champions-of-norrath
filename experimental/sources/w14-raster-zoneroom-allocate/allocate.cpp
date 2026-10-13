// Observed: Allocate__10VIZoneRoom at 0x0111f5a8 is 72 bytes in the pinned SLUS-20565 image. It reads the size word at +112. If it
// is zero it returns zero without allocating. Otherwise it calls __builtin_vec_new(size), stores the result at +116 (even when
// it is null) and returns -1 if the pointer is null, else zero.
// Inferred: +112 is the byte size computed by CalcDataSize__10VIZoneRoom and +116 is the data block; the result is an int status.
// Unknown: the allocator's behaviour (an import), alignment, and who frees the block. Nothing here ran on the EE.
extern "C" {
unsigned __builtin_vec_new(unsigned size);

int Allocate__10VIZoneRoom(unsigned char* self) {
    unsigned size = *reinterpret_cast<unsigned*>(self + 112);
    if (size != 0) {
        unsigned block = __builtin_vec_new(size);
        *reinterpret_cast<unsigned*>(self + 116) = block;
        if (block == 0) return -1;
    }
    return 0;
}
}
