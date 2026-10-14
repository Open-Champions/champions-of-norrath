// Observed: FirstMotif__C22VIParticleDefinitionEx at 0x010b4ea0 is 80 bytes. With pool =
// definition + 724 it calls Size__Ct6VIPool1ZP15VIParticleMotif(pool). A zero size returns
// that zero. Otherwise it calls Begin__Ct6VIPool1ZP15VIParticleMotif(pool) and returns the
// word at (word at pool + 8) + 12 * begin.
// Inferred: the pool at 724 is a VIPool of motif pointers stored in 12-byte entries (first
// word the pointer, items array pointer at pool + 8), so the result is the first live
// motif or null. The result is typed as a 32-bit word because the EE pointer is that size.
// Unknown: the pool's other fields and what the other eight bytes of an entry hold.
extern "C" {
int Size__Ct6VIPool1ZP15VIParticleMotif(const void* pool);
int Begin__Ct6VIPool1ZP15VIParticleMotif(const void* pool);

#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define ENTRIES_OF(word) norrath_host_pointer(word)
#else
#define ENTRIES_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

unsigned FirstMotif__C22VIParticleDefinitionEx(const unsigned char* definition) {
    const unsigned char* pool = definition + 724;
    if (Size__Ct6VIPool1ZP15VIParticleMotif(pool) == 0) return 0;
    int first = Begin__Ct6VIPool1ZP15VIParticleMotif(pool);
    const unsigned char* entries = ENTRIES_OF(*reinterpret_cast<const unsigned*>(pool + 8));
    return *reinterpret_cast<const unsigned*>(entries + first * 12);
}
}
