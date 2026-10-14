// Observed: Deallocate__9VISurface at 0x0112cb10 is 108 bytes in the pinned SLUS-20565 image. If the word at offset 312 (0x138) is zero it
// does nothing and returns zero. Otherwise it calls __builtin_vec_delete(that pointer) and then stores: +16 = 2, +28 = -1, +32 = 1,
// +308 = 0, +8 = 0, +12 = 0, +20 = -1, +24 = -1, +64 = 0, +312 = 0. It returns zero either way.
// Inferred: this undoes Allocate__9VISurface (+308 size, +312 block) and puts the other fields back to what look like default values
// (the -1 words are probably "none" ids); the field meanings are not known.
// Unknown: the roles of the reset fields and the deleter's behaviour (an import). Nothing here ran on the EE.
extern "C" {
void __builtin_vec_delete(unsigned block);

int Deallocate__9VISurface(unsigned char* self) {
    unsigned block = *reinterpret_cast<unsigned*>(self + 312);
    if (block != 0) {
        __builtin_vec_delete(block);
        *reinterpret_cast<int*>(self + 16) = 2;
        *reinterpret_cast<int*>(self + 28) = -1;
        *reinterpret_cast<int*>(self + 32) = 1;
        *reinterpret_cast<int*>(self + 308) = 0;
        *reinterpret_cast<int*>(self + 8) = 0;
        *reinterpret_cast<int*>(self + 12) = 0;
        *reinterpret_cast<int*>(self + 20) = -1;
        *reinterpret_cast<int*>(self + 24) = -1;
        *reinterpret_cast<int*>(self + 64) = 0;
        *reinterpret_cast<int*>(self + 312) = 0;
    }
    return 0;
}
}
