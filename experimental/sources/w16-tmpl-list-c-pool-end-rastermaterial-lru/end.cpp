// Observed: End__Ct6VIPool1Z19VIRasterMaterialLRU at 0x01212e90, 12 bytes (a weak template
// instantiation). It ignores its argument and returns 0x7fffffff.
// Inferred: the value is the pool iterator's one-past-the-last cursor that Next/Erase routines return.
// Unknown: whether the template names it as a constant or computes it.
extern "C" {
int End__Ct6VIPool1Z19VIRasterMaterialLRU(const void* pool) {
    return 0x7fffffff;
}
}
