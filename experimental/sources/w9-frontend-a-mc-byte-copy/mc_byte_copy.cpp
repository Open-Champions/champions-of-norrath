// Observed: MC_ByteCopy__FPvT0i at 0x0025b528 is 52 bytes. Arguments are (destination, source,
// count). It copies count bytes one at a time, lowest address first, and does nothing when the
// count is zero or negative. It returns nothing.
// Inferred: nothing.
// Unknown: nothing.

extern "C" void MC_ByteCopy__FPvT0i(void* destination, void* source, int count) {
    unsigned char* to = static_cast<unsigned char*>(destination);
    unsigned char* from = static_cast<unsigned char*>(source);
    for (int i = 0; i < count; ++i) {
        to[i] = from[i];
    }
}
