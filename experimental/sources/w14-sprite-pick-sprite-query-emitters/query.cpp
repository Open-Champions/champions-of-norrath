// Observed: QueryEmitters__8VISpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery at 0x01007f50, 408 bytes. Arguments: the sprite (S), the scene (not read), the matrix (X) and the emitter query (Q).
// A 64-byte matrix M is zero-filled and destroyed on return (_$_10VIMatrix44 with flag 2). The result is always 0.
// If the word at Q+0 equals the word at S+52:
//  1. D = squared length of the floats X[12], X[13], X[14] (summed in that order) and R = squared length of X[0], X[1], X[2].
//  2. The key is the float at S+44, times sqrt(R), times 1/sqrt(D) (the rsqrt form).
//  3. A two-word entry (Q+8, Q+12) is built, sceVu0CopyMatrix(M, X) runs, and the key and entry are inserted into the map at Q+16 with
//     Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry.
//  4. If the word at Q+4 is less than Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map) the first element is erased
//     (Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map))).
// The entry object occupies 16 bytes on the original's stack; only its first two words are written, so the other two words passed
// to Insert are whatever the stack held. This source declares them without writing them too.
// Inferred: Q holds a kind (+0), a limit (+4) and a record (+8/+12) for an emitter, and a map ordered by a distance-like key
// (the sprite's float at +44, scaled by the matrix scale, divided by the distance of the matrix translation from the origin). The
// local matrix copy is never read after the copy. Unknown: the names of the Q and S fields and the key's meaning.
extern "C" {
int Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(unsigned char* map, const float* key, const unsigned* entry);
int Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry(unsigned char* map);
int Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(unsigned char* map);
int Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(unsigned char* map, int iterator);
void sceVu0CopyMatrix(unsigned char* to, const unsigned char* from);
void _$_10VIMatrix44(unsigned char* matrix, int flags);

int QueryEmitters__8VISpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery(unsigned char* self, unsigned char*, unsigned char* matrix, unsigned char* query) {
    alignas(16) float local[16] = {0};
    unsigned char* M = reinterpret_cast<unsigned char*>(local);
    if (*reinterpret_cast<int*>(query) == *reinterpret_cast<int*>(self + 52)) {
        const float* x = reinterpret_cast<const float*>(matrix);
        float distanceSquared = x[12] * x[12] + x[13] * x[13] + x[14] * x[14];
        float scaleSquared = x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
        float key = *reinterpret_cast<float*>(self + 44) * __builtin_sqrtf(scaleSquared) * (1.0f / __builtin_sqrtf(distanceSquared));
        alignas(16) unsigned entry[4];
        entry[0] = *reinterpret_cast<unsigned*>(query + 8);
        entry[1] = *reinterpret_cast<unsigned*>(query + 12);
        sceVu0CopyMatrix(M, matrix);
        unsigned char* map = query + 16;
        Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(map, &key, entry);
        if (*reinterpret_cast<int*>(query + 4) < Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map))
            Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map));
    }
    _$_10VIMatrix44(M, 2);
    return 0;
}
}
