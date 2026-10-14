// Observed: NextMotif__C22VIParticleDefinitionExP15VIParticleMotif at 0x010b4ef0, 224 bytes. With (definition, motif) the pool is definition + 724
// and its entries are 12 bytes at the pointer in the word at pool + 8 (re-read after each call); the first word of an entry is a motif.
// i = Begin__Ct6VIPool1ZP15VIParticleMotif(pool). While i < End__Ct6VIPool1ZP15VIParticleMotif(pool) (End is called afresh on every test):
//  - when the first word of entry i equals motif, n = Next__Ct6VIPool1ZP15VIParticleMotifi(pool, i) and End is called; when n < End the
//    function calls Next(pool, i) a second time and returns the first word of the entry at that index;
//  - in every other case, including a match whose n is not below End, i = Next(pool, i) and the loop test runs again.
// When the loop ends it returns 0. Nothing is stored.
// Inferred: the successor of a motif in pool order; the answer is zero when motif is last or absent. FirstMotif__C22VIParticleDefinitionEx
// and this make the iteration pair used by the emitter code.
// Unknown: why Next is called twice for the same index on the match path; the pool's other fields.
extern "C" {
int Begin__Ct6VIPool1ZP15VIParticleMotif(const void* pool);
int End__Ct6VIPool1ZP15VIParticleMotif(const void* pool);
int Next__Ct6VIPool1ZP15VIParticleMotifi(const void* pool, int index);

#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define ENTRIES_OF(word) norrath_host_pointer(word)
#else
#define ENTRIES_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

unsigned NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(const unsigned char* definition, unsigned motif) {
    const unsigned char* pool = definition + 724;
    int index = Begin__Ct6VIPool1ZP15VIParticleMotif(pool);
    while (index < End__Ct6VIPool1ZP15VIParticleMotif(pool)) {
        const unsigned char* entries = ENTRIES_OF(*reinterpret_cast<const unsigned*>(pool + 8));
        if (*reinterpret_cast<const unsigned*>(entries + index * 12) == motif) {
            int following = Next__Ct6VIPool1ZP15VIParticleMotifi(pool, index);
            if (following < End__Ct6VIPool1ZP15VIParticleMotif(pool)) {
                int again = Next__Ct6VIPool1ZP15VIParticleMotifi(pool, index);
                entries = ENTRIES_OF(*reinterpret_cast<const unsigned*>(pool + 8));
                return *reinterpret_cast<const unsigned*>(entries + again * 12);
            }
        }
        index = Next__Ct6VIPool1ZP15VIParticleMotifi(pool, index);
    }
    return 0;
}
}
