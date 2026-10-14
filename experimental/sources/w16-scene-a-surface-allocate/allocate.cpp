// Observed: Allocate__9VISurface at 0x0112cac8 is 72 bytes in the pinned SLUS-20565 image. It reads the word at offset 308 (0x134). If that
// is zero it returns zero without allocating. Otherwise it calls __builtin_vec_new(size), stores the result at offset 312 (0x138) (even
// when it is null) and returns -1 if the result is null, else zero.
// Inferred: +308 is a byte size for the surface's pixel data and +312 the block (Deallocate__9VISurface frees +312 and clears +308).
// Unknown: the allocator's behaviour (an import), alignment, and what the block holds exactly. Nothing here ran on the EE.
extern "C" {
unsigned __builtin_vec_new(unsigned size);

int Allocate__9VISurface(unsigned char* self) {
    unsigned size = *reinterpret_cast<unsigned*>(self + 308);
    if (size != 0) {
        unsigned block = __builtin_vec_new(size);
        *reinterpret_cast<unsigned*>(self + 312) = block;
        if (block == 0) return -1;
    }
    return 0;
}
}
