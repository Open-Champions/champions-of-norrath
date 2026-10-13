// Observed: DestroyPool__t7VIArray1Z7VIPlane at 0x01207b08, 76 bytes. The container is the only argument.
//  1. The count is the word at +0. When it is zero the function returns 0 at once.
//  2. Otherwise a cursor starts at 0 and, until it reaches 0x7fffffff, becomes 0x7fffffff when it equals count - 1
//     and cursor + 1 otherwise. That is one step per index from 0 to count - 1.
//  3. Returns 0. Nothing is written, read from the storage or freed.
// Inferred: the loop is the Begin/End/Next walk of the pool instances compiled inline for a dense container; it does nothing
// per element, so it is what remains of a per-element teardown. The template argument comes only from the mangled name.
// A negative count would make the walk run for about 2^31 steps.
extern "C" {
int DestroyPool__t7VIArray1Z7VIPlane(void* container) {
    int count = *static_cast<const int*>(container);
    int cursor = count != 0 ? 0 : 0x7fffffff;
    if (cursor != 0x7fffffff) {
        int last = count - 1;
        do {
            cursor = cursor == last ? 0x7fffffff : cursor + 1;
            __asm__ volatile("" : "+r"(cursor));
        } while (cursor != 0x7fffffff);
    }
    return 0;
}
}
