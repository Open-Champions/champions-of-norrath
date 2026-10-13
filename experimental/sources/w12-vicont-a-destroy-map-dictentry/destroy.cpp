// Observed: DestroyPool__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry at 0x0120a8e8, 88 bytes. The container is the only argument.
//  1. Calls Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(container) and keeps the result as a cursor.
//  2. While the cursor differs from End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(container), which is called again every time
//     round, the cursor becomes Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(container, cursor).
//  3. Returns 0. Nothing is read, written or freed here.
// Inferred: the loop is a walk over every entry that visits each one and does nothing, so it
// is what remains of a per-entry teardown for this element type. The template arguments come
// only from the mangled name. The same loop appears, instruction for instruction, in the
// instances for the other element types.
extern "C" {
int Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* container);
int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* container);
int Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(const void* container, int cursor);

int DestroyPool__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* container) {
    int cursor = Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(container);
    while (cursor != End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(container)) cursor = Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(container, cursor);
    return 0;
}
}
