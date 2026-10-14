// Observed: __t7VIArray1Z20VICSpritePartEmitter at 0x012082b8 is a constructor: it stores zero at +4 and then at +0 of the object and
// returns nothing. It calls nothing and reads nothing.
// Inferred: for the array template the word at +4 is the element storage that the matching Clear and
// destructor release, and the word at +0 the element count; the template argument comes only from the
// mangled name.
// Unknown: the element size and whether more fields exist beyond +8.
extern "C" {
void __t7VIArray1Z20VICSpritePartEmitter(void* array) {
    unsigned* words = static_cast<unsigned*>(array);
    words[1] = 0;
    words[0] = 0;
}
}
