// Observed: Next__Ct6VIPool1ZP15VIParticleMotifi at 0x01208908, 44 bytes (a weak template
// instantiation). The entry table is the pointer at byte 8 of the pool and entries are 12 bytes.
// The result is the word at byte 4 of entry[index], or 0x7fffffff when that word is -1. Nothing is
// written and the index is not bounds-checked.
// Inferred: the word is the index of the following live entry and 0x7fffffff is the end cursor
// (as returned by End__Ct6VIPool1ZP15VIParticleMotif, which is inlined here as a constant).
// Unknown: the entry type beyond the two link words used by Erase__t6VIPool1ZP15VIParticleMotifi.
extern "C" {
int Next__Ct6VIPool1ZP15VIParticleMotifi(const void* pool, int index) {
    const unsigned char* table = *reinterpret_cast<const unsigned char* const*>(static_cast<const unsigned char*>(pool) + 8);
    int next = *reinterpret_cast<const int*>(table + 12 * index + 4);
    return next == -1 ? 0x7fffffff : next;
}
}
