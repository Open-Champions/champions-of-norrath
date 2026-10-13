// Observed: ReleaseAllParticleDefinitions__16VIParticleSystemR12VIDictionary at
// 0x010b8f70, 124 bytes. With the object as this, pool = this + 84 and dict
// the second argument (a reference, passed as an address): repeat
// i = Begin__Ct6VIPool1ZP20VIParticleDefinition(pool); stop when i equals
// End__Ct6VIPool1ZP20VIParticleDefinition(pool); otherwise call
// ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(this, i, dict).
// After the loop, Clear__t6VIPool1ZP20VIParticleDefinition(pool); the result is
// zero. Begin is called again on every iteration.
// Inferred: the release call removes the entry at i so that Begin advances.
extern "C" {
int Begin__Ct6VIPool1ZP20VIParticleDefinition(void* pool);
int End__Ct6VIPool1ZP20VIParticleDefinition(void* pool);
void Clear__t6VIPool1ZP20VIParticleDefinition(void* pool);
int ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(void* self, int index, void* dictionary);

int ReleaseAllParticleDefinitions__16VIParticleSystemR12VIDictionary(void* self, void* dictionary) {
    void* pool = static_cast<char*>(self) + 84;
    for (;;) {
        int i = Begin__Ct6VIPool1ZP20VIParticleDefinition(pool);
        if (i == End__Ct6VIPool1ZP20VIParticleDefinition(pool)) break;
        ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(self, i, dictionary);
    }
    Clear__t6VIPool1ZP20VIParticleDefinition(pool);
    return 0;
}
}
