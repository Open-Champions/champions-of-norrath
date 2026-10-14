// Observed: DestroyPool__t6VIPool1ZP13VISpellEffect at 0x011f3938, 88 bytes. With the pool as this: it = Begin__t6VIPool1ZP13VISpellEffect
// (pool). Then, as long as it differs from End__Ct6VIPool1ZP13VISpellEffect(pool) (called afresh every pass, before the test), it
// becomes Next__Ct6VIPool1ZP13VISpellEffecti(pool, it). The result is 0. Nothing is stored and the iterator values are not used for
// anything else, so no element is destroyed here.
// Inferred: the template's destroy-all loop over live entries, with a trivial pointer-element destructor so the body is empty.
// Unknown: whether the original loop body held a destructor call that the compiler removed.
extern "C" {
int Begin__Ct6VIPool1ZP13VISpellEffect(const void* pool);
int End__Ct6VIPool1ZP13VISpellEffect(const void* pool);
int Next__Ct6VIPool1ZP13VISpellEffecti(const void* pool, int iterator);

int DestroyPool__t6VIPool1ZP13VISpellEffect(void* pool) {
    int iterator = Begin__Ct6VIPool1ZP13VISpellEffect(pool);
    while (iterator != End__Ct6VIPool1ZP13VISpellEffect(pool))
        iterator = Next__Ct6VIPool1ZP13VISpellEffecti(pool, iterator);
    return 0;
}
}
