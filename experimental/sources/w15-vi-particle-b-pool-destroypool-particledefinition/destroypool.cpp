// Observed: DestroyPool__t6VIPool1ZP20VIParticleDefinition at 0x01208e08, 92 bytes (a weak template
// instantiation). It calls Begin__Ct6VIPool1ZP20VIParticleDefinition(pool). While the cursor is not
// 0x7fffffff (the End value, inlined as a constant) it calls
// Next__Ct6VIPool1ZP20VIParticleDefinitioni(pool, cursor) and takes the result as the new cursor.
// The result is always zero. The loop body does nothing else, so no per-entry destructor runs here.
// Inferred: the walk visits every live entry of the pool; the loop is the remains of a destroy step
// whose element type needs no destruction. Unknown: whether the template names the constant.
extern "C" {
int Begin__Ct6VIPool1ZP20VIParticleDefinition(const void* pool);
int Next__Ct6VIPool1ZP20VIParticleDefinitioni(const void* pool, int index);

int DestroyPool__t6VIPool1ZP20VIParticleDefinition(void* pool) {
    int cursor = Begin__Ct6VIPool1ZP20VIParticleDefinition(pool);
    while (cursor != 0x7fffffff) cursor = Next__Ct6VIPool1ZP20VIParticleDefinitioni(pool, cursor);
    return 0;
}
}
