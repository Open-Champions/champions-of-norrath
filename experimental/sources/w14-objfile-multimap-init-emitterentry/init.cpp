// Observed: Init__t10VIMultiMap2ZfZ14VIEmitterEntryi at 0x01226638, 92 bytes. With (this, count) it calls
// Clear__t10VIMultiMap2ZfZ14VIEmitterEntry(this); a negative result returns -1 with nothing stored. Otherwise it stores
// count + 1 at this+4, calls Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(this) and returns -1 when that result is negative, else 0.
// Inferred: the word at this+4 is the number of pool slots that Allocate consumes (one more than the requested count);
// the word at this+0 is not touched here. The template arguments come only from the mangled name.
// Unknown: the other fields of the container and the value ranges of Clear and Allocate results.
extern "C" {
int Clear__t10VIMultiMap2ZfZ14VIEmitterEntry(void* self);
int Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(void* self);

int Init__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* self, int count) {
    if (Clear__t10VIMultiMap2ZfZ14VIEmitterEntry(self) < 0) return -1;
    static_cast<int*>(self)[1] = count + 1;
    return Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(self) < 0 ? -1 : 0;
}
}
