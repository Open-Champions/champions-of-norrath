// Observed: __t6VIPool1Z19VIHSpriteAttachment at 0x01203460 is a constructor: it stores zero at +0, +4 and +8 and minus one at +20, +12
// and +16 of the object and returns nothing. It calls nothing and reads nothing. The stores are the
// same as the ones the matching Clear makes after releasing storage.
// Inferred: +8 is the entry storage, +4 the capacity and +0 the live count; +12, +16 and +20 hold entry
// indices (-1 meaning none) as the pool Erase routines of this family show.
// Unknown: the entry size for this type and whether more fields follow +24.
extern "C" {
void __t6VIPool1Z19VIHSpriteAttachment(void* pool) {
    unsigned* words = static_cast<unsigned*>(pool);
    words[0] = 0;
    words[5] = static_cast<unsigned>(-1);
    words[1] = 0;
    words[2] = 0;
    words[3] = static_cast<unsigned>(-1);
    words[4] = static_cast<unsigned>(-1);
}
}
