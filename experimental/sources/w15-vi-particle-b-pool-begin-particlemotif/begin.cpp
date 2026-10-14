// Observed: Begin__Ct6VIPool1ZP15VIParticleMotif at 0x012088c8, 44 bytes (a weak template
// instantiation). It reads the signed word at byte 16 of the pool. If that word is -1 the result
// is End__Ct6VIPool1ZP15VIParticleMotif(pool) (a tail call); otherwise the result is the word.
// Inferred: the word is the index of the first live entry and -1 means the pool is empty.
// Unknown: the member name and how the word is maintained.
extern "C" {
int End__Ct6VIPool1ZP15VIParticleMotif(const void* pool);

int Begin__Ct6VIPool1ZP15VIParticleMotif(const void* pool) {
    int first = *reinterpret_cast<const int*>(static_cast<const unsigned char*>(pool) + 16);
    if (first == -1) return End__Ct6VIPool1ZP15VIParticleMotif(pool);
    return first;
}
}
