// Observed: QueryEmitters__19VIStreamAudioSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery at 0x01018a20, 728 bytes. Arguments: the sprite (S), the scene (not read), the matrix (X) and the emitter query (Q).
// If the word at Q+0 differs from the word at S+52 the result is 0 and nothing else happens. Otherwise two 64-byte matrices M0 and
// M1 are zero-filled (and always destroyed: _$_10VIMatrix44 with flag 2, M1 first then M0), a three-float point P starts at zero, and:
//  1. InvertAffine__10VIMatrix44RC10VIMatrix44(M1, X); P is the translation floats 12, 13, 14 of M1.
//  2. Contains__C6VIBBoxRC7VIVect3(S+8, P): if false the result is 0 (after the destructors).
//  3. The word at S+68 picks a scale: 0 gives 1.0, 1 gives 2.0, 2 gives 3.0. Any other value returns -1 (after the destructors).
//  4. P becomes P minus the centre of the box at S+8, whose centre is half the sum of floats S+8..S+16 and S+20..S+28 per axis
//     (x: (S[8]+S[20])/2, y: (S[12]+S[24])/2, z: (S[16]+S[28])/2).
//  5. The key is the scale minus 0.0001 times Magnitude__C7VIVect3(P).
//  6. A two-word entry (Q+8, Q+12) is built, sceVu0CopyMatrix(M0, X) runs, and the key and entry are inserted into the map at Q+16.
//  7. If the word at Q+4 is less than the map's size the first element is erased. The result is 0.
// The entry object occupies 16 bytes on the original's stack; only its first two words are written.
// Inferred: this is the stream-audio sprite's volume test: the query position, taken into the sprite's local space through the
// inverse of X, lies inside the box (S+8) or not; the key ranks emitters by how close the point is to the box centre. The word at
// S+68 is a falloff or shape selector with three known values. Unknown: all field names, and whether M0's copy of X is used elsewhere.
extern "C" {
int Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(unsigned char* map, const float* key, const unsigned* entry);
int Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry(unsigned char* map);
int Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(unsigned char* map);
int Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(unsigned char* map, int iterator);
void InvertAffine__10VIMatrix44RC10VIMatrix44(unsigned char* out, unsigned char* in);
int Contains__C6VIBBoxRC7VIVect3(unsigned char* box, unsigned char* point);
float Magnitude__C7VIVect3(unsigned char* vector);
void sceVu0CopyMatrix(unsigned char* to, const unsigned char* from);
void _$_10VIMatrix44(unsigned char* matrix, int flags);

int QueryEmitters__19VIStreamAudioSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery(unsigned char* self, unsigned char*, unsigned char* matrix, unsigned char* query) {
    if (*reinterpret_cast<int*>(query) != *reinterpret_cast<int*>(self + 52)) return 0;
    alignas(16) float point[3] = {0.0f, 0.0f, 0.0f};
    alignas(16) float local0[16] = {0}, local1[16] = {0};
    unsigned char* P = reinterpret_cast<unsigned char*>(point);
    unsigned char* M0 = reinterpret_cast<unsigned char*>(local0);
    unsigned char* M1 = reinterpret_cast<unsigned char*>(local1);
    InvertAffine__10VIMatrix44RC10VIMatrix44(M1, matrix);
    point[0] = local1[12];
    point[1] = local1[13];
    point[2] = local1[14];
    if (!Contains__C6VIBBoxRC7VIVect3(self + 8, P)) {
        _$_10VIMatrix44(M1, 2);
        _$_10VIMatrix44(M0, 2);
        return 0;
    }
    float key;
    switch (*reinterpret_cast<int*>(self + 68)) {
    case 0: key = 1.0f; break;
    case 1: key = 2.0f; break;
    case 2: key = 3.0f; break;
    default:
        _$_10VIMatrix44(M1, 2);
        _$_10VIMatrix44(M0, 2);
        return -1;
    }
    const float* box = reinterpret_cast<const float*>(self + 8);
    point[0] = point[0] - (box[0] + box[3]) * 0.5f;
    point[1] = point[1] - (box[1] + box[4]) * 0.5f;
    point[2] = point[2] - (box[2] + box[5]) * 0.5f;
    key = key - Magnitude__C7VIVect3(P) * 0.0001f;
    unsigned char* map = query + 16;
    alignas(16) unsigned entry[4];
    entry[0] = *reinterpret_cast<unsigned*>(query + 8);
    entry[1] = *reinterpret_cast<unsigned*>(query + 12);
    sceVu0CopyMatrix(M0, matrix);
    Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry(map, &key, entry);
    if (*reinterpret_cast<int*>(query + 4) < Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map))
        Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi(map, Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry(map));
    _$_10VIMatrix44(M1, 2);
    _$_10VIMatrix44(M0, 2);
    return 0;
}
}
