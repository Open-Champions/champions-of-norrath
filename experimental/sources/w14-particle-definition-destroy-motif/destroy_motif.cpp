// Observed: DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif at 0x010b4d20 is 180
// bytes. With pool = definition + 724 it walks the pool: index = Begin(pool), and while
// index < End(pool) it reads the word at (word at pool + 8) + 12 * index. On the first entry
// equal to the motif argument it calls Erase__t6VIPool1ZP15VIParticleMotifi(pool, index);
// then, if that entry word was not zero, it calls __builtin_delete with it, and returns.
// Otherwise it advances with Next(pool, index). End is re-evaluated each pass. When no
// entry matches nothing is erased or deleted.
// Inferred: the pool holds owned motif pointers in 12-byte entries, so this removes one
// motif from the definition and frees it. Begin/End/Next are the const VIPool iterators
// (Begin__Ct6..., End__Ct6..., Next__Ct6...i).
// Unknown: whether a null motif argument (which would match a null entry and skip the
// delete) is ever passed, and what the other eight bytes of an entry hold.
extern "C" {
int Begin__Ct6VIPool1ZP15VIParticleMotif(const void* pool);
int End__Ct6VIPool1ZP15VIParticleMotif(const void* pool);
int Next__Ct6VIPool1ZP15VIParticleMotifi(const void* pool, int index);
void Erase__t6VIPool1ZP15VIParticleMotifi(void* pool, int index);
void __builtin_delete(void* pointer);

#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define ENTRIES_OF(word) norrath_host_pointer(word)
#else
#define ENTRIES_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

void DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif(unsigned char* definition, unsigned motif) {
    unsigned char* pool = definition + 724;
    for (int index = Begin__Ct6VIPool1ZP15VIParticleMotif(pool);
         index < End__Ct6VIPool1ZP15VIParticleMotif(pool);
         index = Next__Ct6VIPool1ZP15VIParticleMotifi(pool, index)) {
        unsigned entry = *reinterpret_cast<unsigned*>(ENTRIES_OF(*reinterpret_cast<unsigned*>(pool + 8)) + index * 12);
        if (entry == motif) {
            Erase__t6VIPool1ZP15VIParticleMotifi(pool, index);
            if (entry != 0) {
#ifdef NORRATH_HOST_ARENA
                __builtin_delete(norrath_host_pointer(entry));
#else
                __builtin_delete(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(entry)));
#endif
            }
            return;
        }
    }
}
}
