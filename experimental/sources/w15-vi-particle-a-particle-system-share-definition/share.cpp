// Observed: ShareParticleDefinition__16VIParticleSystemi at 0x010b8f40 is 48 bytes. Arguments: the system and an index. The word at
// system+92 is the base of an array of 12-byte records; the word at the start of record [index] points at a definition. The
// routine increments the word at definition+768 (that is, +4 of the address definition+764) and returns 0.
// Inferred: +768 is a reference count and the 12-byte records belong to the system's definition table.
// Unknown: the other fields of the record, the bounds of the index (none is checked here), and the meaning of the zero return.
extern "C" {
#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define POINTER_OF(word) norrath_host_pointer(word)
#else
#define POINTER_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

int ShareParticleDefinition__16VIParticleSystemi(unsigned char* system, int index) {
    unsigned char* record = POINTER_OF(*reinterpret_cast<unsigned*>(system + 92)) + index * 12;
    unsigned char* definition = POINTER_OF(*reinterpret_cast<unsigned*>(record));
    *reinterpret_cast<int*>(definition + 768) += 1;
    return 0;
}
}
