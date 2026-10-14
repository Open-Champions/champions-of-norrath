// Observed: Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry at 0x011f4de0, 20 bytes (a weak template
// instantiation). It reads the signed word at byte 0 of the map, subtracts one, and returns the
// result, or zero when the result is negative. Nothing is written.
// Inferred: the word counts nodes including the null node at index zero, so the result is the
// number of live entries. Unknown: the member name.
extern "C" {
int Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void* map) {
    int n = *static_cast<const int*>(map) - 1;
    return n < 0 ? 0 : n;
}
}
