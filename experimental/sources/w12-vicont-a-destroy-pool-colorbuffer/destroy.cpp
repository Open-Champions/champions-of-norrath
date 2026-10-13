// Observed: DestroyPool__t6VIPool1ZP13VIColorBuffer at 0x01212d30, 88 bytes. The container is the only argument.
//  1. Calls Begin__Ct6VIPool1ZP13VIColorBuffer(container) and keeps the result as a cursor.
//  2. While the cursor differs from End__Ct6VIPool1ZP13VIColorBuffer(container), which is called again every time
//     round, the cursor becomes Next__Ct6VIPool1ZP13VIColorBufferi(container, cursor).
//  3. Returns 0. Nothing is read, written or freed here.
// Inferred: the loop is a walk over every entry that visits each one and does nothing, so it
// is what remains of a per-entry teardown for this element type. The template arguments come
// only from the mangled name. The same loop appears, instruction for instruction, in the
// instances for the other element types.
extern "C" {
int Begin__Ct6VIPool1ZP13VIColorBuffer(const void* container);
int End__Ct6VIPool1ZP13VIColorBuffer(const void* container);
int Next__Ct6VIPool1ZP13VIColorBufferi(const void* container, int cursor);

int DestroyPool__t6VIPool1ZP13VIColorBuffer(void* container) {
    int cursor = Begin__Ct6VIPool1ZP13VIColorBuffer(container);
    while (cursor != End__Ct6VIPool1ZP13VIColorBuffer(container)) cursor = Next__Ct6VIPool1ZP13VIColorBufferi(container, cursor);
    return 0;
}
}
