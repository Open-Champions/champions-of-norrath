// Observed: VINameDeferSort__FPCvT0 at 0x010a12a8 is 44 bytes. It takes two pointers, loads the
// single-precision float at offset 0 of each, and returns -1 if the first is less than the
// second, 0 if they are equal, otherwise 1. No other memory is touched.
// Inferred: a comparison callback for a qsort-style routine ordering name-sprite defer records by
// their first float field.
extern "C" {
int VINameDeferSort__FPCvT0(const void* a, const void* b) {
    float x = *static_cast<const float*>(a);
    float y = *static_cast<const float*>(b);
    if (x < y) return -1;
    if (x == y) return 0;
    return 1;
}
}
