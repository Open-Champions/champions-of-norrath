// Observed: __16VIParticleSystem at 0x010b8df0 is 72 bytes. It calls __7VILList
// on offsets 52 and 64, then __t6VIPool1ZP20VIParticleDefinition on offset 84.
// It then stores zero in the word at 80, an eight-byte zero at 8, and zero words
// at 76 and 0. It returns nothing observable (v0 is left over from the calls).
// Unknown: the meaning of the zeroed fields.
extern "C" {
void __7VILList(void* list);
void __t6VIPool1ZP20VIParticleDefinition(void* pool);

void __16VIParticleSystem(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    __7VILList(bytes + 52);
    __7VILList(bytes + 64);
    __t6VIPool1ZP20VIParticleDefinition(bytes + 84);
    *reinterpret_cast<int*>(bytes + 80) = 0;
    *reinterpret_cast<unsigned long long*>(bytes + 8) = 0;
    *reinterpret_cast<int*>(bytes + 76) = 0;
    *reinterpret_cast<int*>(bytes + 0) = 0;
}
}
