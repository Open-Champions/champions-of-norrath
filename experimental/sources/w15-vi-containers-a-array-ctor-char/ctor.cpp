// Observed: __t7VIArray1Zc at 0x011f61b8, 12 bytes. With (this) it stores zero at this+4 and then zero at this+0 and returns nothing.
// Inferred: the default constructor of the byte-element array: +0 is the element count and +4 the storage pointer, as the matching
// destructor and Clear routines of this family read them.
// Unknown: whether the original constructor ran any further member initialisation that the compiler removed.
extern "C" {
void __t7VIArray1Zc(void* self) {
    unsigned* words = static_cast<unsigned*>(self);
    words[1] = 0;
    words[0] = 0;
}
}
