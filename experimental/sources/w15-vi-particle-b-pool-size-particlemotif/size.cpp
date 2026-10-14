// Observed: Size__Ct6VIPool1ZP15VIParticleMotif at 0x01208938, 8 bytes (a weak template
// instantiation). It returns the signed word at byte 0 of the pool and writes nothing.
// Inferred: the word is the count of live entries; Erase__t6VIPool1ZP15VIParticleMotifi
// decrements it. Unknown: the member name.
extern "C" {
int Size__Ct6VIPool1ZP15VIParticleMotif(const void* pool) {
    return *static_cast<const int*>(pool);
}
}
