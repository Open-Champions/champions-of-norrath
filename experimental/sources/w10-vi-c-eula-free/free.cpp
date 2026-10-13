// Observed: Free__9VIWndEula at 0x010b58a8 is 56 bytes. With the object as this, a nonzero word
// at this+1552 is passed to __builtin_vec_delete and then cleared. Nothing is returned.
extern "C" {
void __builtin_vec_delete(void* pointer);

void Free__9VIWndEula(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    void* block = *reinterpret_cast<void**>(bytes + 1552);
    if (block != 0) {
        __builtin_vec_delete(block);
        *reinterpret_cast<void**>(bytes + 1552) = 0;
    }
}
}
