// Observed: Allocate__13VIHSpriteAnim at 0x0105d228, 72 bytes. Let S be the word at this+48. If S is zero it returns 0 and writes nothing.
// Otherwise it calls __builtin_vec_new(S) and stores the result at this+52 (a null result is stored too, in the branch delay slot);
// a null result returns -1, any other result returns 0.
// Inferred: this+48 is the byte size computed by CalcDataSize__13VIHSpriteAnim and this+52 is the block that Init relocates its offsets
// against. A second call would overwrite +52 without releasing the earlier block.
// Unknown: where the block is released.
extern "C" {
void* __builtin_vec_new(unsigned size);

int Allocate__13VIHSpriteAnim(unsigned char* self) {
    unsigned size = *reinterpret_cast<unsigned*>(self + 48);
    if (size != 0) {
        void* block = __builtin_vec_new(size);
        *reinterpret_cast<unsigned*>(self + 52) = static_cast<unsigned>(reinterpret_cast<unsigned long>(block));
        if (block == nullptr) return -1;
    }
    return 0;
}
}
