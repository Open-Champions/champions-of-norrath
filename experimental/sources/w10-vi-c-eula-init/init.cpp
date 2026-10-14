// Observed: Init__9VIWndEula at 0x010b57a8 is 68 bytes. With the object as this it stores 0 at
// this+1552 and 1 at this+1548, then allocates 0x10000 bytes with __builtin_vec_new. If the
// result is null it returns -1. Otherwise it passes the block straight to
// __builtin_vec_delete and returns 0; the block is not stored anywhere.
// Inferred: the allocation is a probe only (possibly a stub of a text-loading step).
// Unknown: why the image allocates and immediately frees; word 1548 is read by no code here.
extern "C" {
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);

int Init__9VIWndEula(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes + 1552) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1548) = 1;
    void* block = __builtin_vec_new(0x10000);
    if (block == 0) return -1;
    __builtin_vec_delete(block);
    return 0;
}
}
