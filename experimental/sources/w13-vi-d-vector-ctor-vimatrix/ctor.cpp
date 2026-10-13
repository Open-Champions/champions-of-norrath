// Observed: __t8VIVector1Z10VIMatrix44 at 0x01203440 is a constructor: it stores zero at +8, +0 and +4 of the object and returns
// nothing. It calls nothing and reads nothing.
// Inferred: by the same shape as the vector Clear/Add routines of this family, +0 is a count, +4 a
// capacity and +8 the element storage; the element type comes only from the mangled name.
// Unknown: the element size and whether more fields exist beyond +12.
extern "C" {
void __t8VIVector1Z10VIMatrix44(void* vector) {
    unsigned* words = static_cast<unsigned*>(vector);
    words[2] = 0;
    words[0] = 0;
    words[1] = 0;
}
}
