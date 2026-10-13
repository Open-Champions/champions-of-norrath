// Observed: End__Ct6VIPool1ZP13VISpellEffect at 0x011f35f0, 12 bytes (a weak
// template instantiation). It ignores its argument and returns 0x7fffffff.
// Inferred: the value is the one-past-the-last cursor of the pool, compared
// against by iteration code. Unknown: whether the template names it as a
// constant or computes it.
extern "C" {
int End__Ct6VIPool1ZP13VISpellEffect(const void* pool) {
    return 0x7fffffff;
}
}
