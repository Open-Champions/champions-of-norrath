// Observed: ParticleDefinition__16VIParticleSystemi at 0x010b8f20 is 28 bytes. It reads the
// pointer at offset 92 of the particle system, adds 12 * index (no bounds check), and
// returns the word at that address.
// Inferred: offset 92 points to an array of 12-byte records whose first word is the
// particle definition pointer; the name comes from the symbol. The result is typed as a
// 32-bit word here because the EE pointer and the word are the same size.
// Unknown: the other eight bytes of each record and the array's capacity.
extern "C" {
#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define RECORDS_OF(word) norrath_host_pointer(word)
#else
#define RECORDS_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

unsigned ParticleDefinition__16VIParticleSystemi(const unsigned char* system, int index) {
    const unsigned char* records = RECORDS_OF(*reinterpret_cast<const unsigned*>(system + 92));
    return *reinterpret_cast<const unsigned*>(records + index * 12);
}
}
