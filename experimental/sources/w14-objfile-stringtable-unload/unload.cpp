// Observed: Unload__13VIStringTable at 0x0101bbe8, 88 bytes. With the table as this: when the word at +8 is nonzero it is passed
// to __builtin_vec_delete; the word at +12 is read next and, when nonzero, passed to __builtin_vec_delete too. Then +12, +0,
// +4 and +8 are set to zero (the stores to +12 happen on both paths). Nothing is returned.
// Inferred: +8 and +12 are two heap blocks owned by the table (the text pointer block at +12 as GetString reads it, and a second
// block at +8); +0 and +4 are the row and column counts. Unload returns the table to the state __13VIStringTable leaves.
// Unknown: what the block at +8 holds.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline void* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline void* at(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
void __builtin_vec_delete(void* pointer);

void Unload__13VIStringTable(void* self) {
    unsigned* words = static_cast<unsigned*>(self);
    if (words[2] != 0) __builtin_vec_delete(at(words[2]));
    if (words[3] != 0) __builtin_vec_delete(at(words[3]));
    words[3] = 0;
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
}
}
