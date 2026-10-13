// Observed: Init__14VIEmitterQueryi12VISpriteType at 0x0114f330 is 72 bytes. Arguments: query
// (a0), an int (a1) and a sprite type (a2). It stores a2 at +0, a1 at +4, -1 at +8 and
// -1 at +12, then calls Init__t10VIMultiMap2ZfZ14VIEmitterEntryi(query + 16). If that
// returns a negative value the function returns -1, otherwise it returns 0.
// Inferred: the query holds a sprite type and a count/capacity (+4), two cursor-like
// words reset to -1, and an embedded multimap of emitter entries at +16.
// Unknown: the meaning of +4, +8 and +12, and the multimap layout.
extern "C" {
int Init__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* map);

int Init__14VIEmitterQueryi12VISpriteType(unsigned char* query, int arg, int spriteType) {
    *reinterpret_cast<int*>(query + 0) = spriteType;
    *reinterpret_cast<int*>(query + 4) = arg;
    *reinterpret_cast<int*>(query + 8) = -1;
    *reinterpret_cast<int*>(query + 12) = -1;
    return Init__t10VIMultiMap2ZfZ14VIEmitterEntryi(query + 16) < 0 ? -1 : 0;
}
}
