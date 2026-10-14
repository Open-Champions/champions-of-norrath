// Observed: CreateMotif__22VIParticleDefinitionEx at 0x010b4ca8, 120 bytes. With the definition as this it calls __builtin_new(760). When
// the result M is non-null it calls __20VIParticleAttributes(M), copies the first byte of an empty string constant (zero) to M+724, and
// stores VIHashResourceID of that empty string at M+756. A pointer word (M, or zero when the allocation failed) is then put in a local and
// Add__t6VIPool1ZP15VIParticleMotifRCP15VIParticleMotif(this+724, &local) is called, even for a null M (its result is ignored). It returns the
// local, which is M or 0.
// Inferred: a 760-byte motif is an attribute block (constructor __20VIParticleAttributes) with a name buffer at +724 and a resource id at
// +756; a new motif starts with an empty name and the hash of the empty string. The pool at this+724 is the same pool FirstMotif
// and NextMotif walk.
// Unknown: the real size of the name buffer, and why a null result is still added.
extern "C" {
void* __builtin_new(unsigned size);
void __20VIParticleAttributes(void* self);
unsigned VIHashResourceID(const char* name);
int Add__t6VIPool1ZP15VIParticleMotifRCP15VIParticleMotif(void* pool, const unsigned* motif);

unsigned CreateMotif__22VIParticleDefinitionEx(unsigned char* definition) {
    static const char kEmpty[] = "";
    unsigned char* motif = static_cast<unsigned char*>(__builtin_new(760));
    unsigned word = 0;
    if (motif != nullptr) {
        __20VIParticleAttributes(motif);
        motif[724] = static_cast<unsigned char>(kEmpty[0]);
        *reinterpret_cast<unsigned*>(motif + 756) = VIHashResourceID(kEmpty);
        word = static_cast<unsigned>(reinterpret_cast<unsigned long>(motif));
    }
    Add__t6VIPool1ZP15VIParticleMotifRCP15VIParticleMotif(definition + 724, &word);
    return word;
}
}
