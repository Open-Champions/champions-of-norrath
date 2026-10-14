// Observed: End__Ct6VIPool1ZP15VIParticleMotif at 0x012088f8, 12 bytes (a weak template
// instantiation). It ignores its argument and returns 0x7fffffff.
// Inferred: the value is the cursor that iteration compares against; the same value is used by
// the VIParticleDefinition instantiation. Unknown: whether the template names it as a constant.
extern "C" {
int End__Ct6VIPool1ZP15VIParticleMotif(const void* pool) {
    return 0x7fffffff;
}
}
