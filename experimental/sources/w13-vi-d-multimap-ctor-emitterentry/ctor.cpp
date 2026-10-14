// Observed: __t10VIMultiMap2ZfZ14VIEmitterEntry at 0x01224fd0 is a constructor: it stores zero at +0, minus one at +16, then zero at
// +4, +8 and +12 of the object and returns nothing. It calls nothing and reads nothing.
// Inferred: +8 is the entry storage that the matching destructor releases; the template arguments come
// only from the mangled name.
// Unknown: the meaning of the other words and whether more fields follow +20.
extern "C" {
void __t10VIMultiMap2ZfZ14VIEmitterEntry(void* map) {
    unsigned* words = static_cast<unsigned*>(map);
    words[0] = 0;
    words[4] = static_cast<unsigned>(-1);
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
}
}
