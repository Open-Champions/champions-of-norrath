// Observed: Begin__Ct6VIPool1ZP20VIParticleDefinition at 0x01208fa0, 44 bytes (a weak
// template instantiation). With the pool as this it reads the signed word at
// this + 16. If that word is -1 the result is End__Ct6VIPool1ZP20VIParticleDefinition(this)
// (a tail call), otherwise the result is the word itself.
// Inferred: the word is the index of the first live entry, with -1 meaning that
// the pool is empty. Unknown: the member name and how the word is maintained.
extern "C" {
int End__Ct6VIPool1ZP20VIParticleDefinition(const void* pool);

int Begin__Ct6VIPool1ZP20VIParticleDefinition(const void* pool) {
    int first = *reinterpret_cast<const int*>(static_cast<const unsigned char*>(pool) + 16);
    if (first == -1) return End__Ct6VIPool1ZP20VIParticleDefinition(pool);
    return first;
}
}
