// Observed _$_4Line at 0x00279c60, 84 bytes. Releases the heap block whose
// pointer is the word at object+20 with memFree, then, when bit 0 of the incoming
// flag is set, frees the object itself with blockFree (a tail call). Other flag
// bits do not free. The word at +20 is released whatever the flag is, and it is
// not tested for zero first.
// Unknown: what the block at +20 holds; no field name is claimed.

extern "C" {
void memFree__FPv(void* pointer);
void norrath_block_free(void* object);
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
}

extern "C" void dtor_4Line(void* object_storage, int flags) asm("_$_4Line");
extern "C" void dtor_4Line(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    memFree__FPv(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(read_word(object + 20u))));
    if ((flags & 1) != 0) norrath_block_free(object);
}
