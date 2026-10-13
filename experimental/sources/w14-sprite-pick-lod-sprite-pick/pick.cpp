// Observed: Pick__11VILODSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster at 0x01076470, 364 bytes. Arguments: the sprite (S), the pick segment (G), the scene filter (F), the
// scene (V) and the raster (R). Takes the model-view matrix of R (GetModelView__C8VIRaster), forms the difference
// between its three floats at +48/+52/+56 and the three floats at G+0/+4/+8, and squares and sums it (D, a squared distance).
// S holds a count at +68 and 8-byte records from +72: a float threshold at +0 and a sprite index at +4. The first record
// whose squared threshold is >= D is chosen (a record is skipped while its squared threshold is below D). If none is
// chosen (count reached, including count 0) the result is 0. Otherwise Sprite__7VIScenei(V, index) returns the sprite P, its
// word at +60 is a table, and the 8-byte entry at table+48 (signed halfword adjustment, then function word) is called with
// (P + adjustment, G, F, V, R) and its result returned.
// Inferred: this is level-of-detail selection by distance from the segment origin; the table slot is the pick virtual.
// Observed also: the original may read one threshold past the count before leaving its loop; that read cannot change the choice.
// Unknown: the names of the record fields and of the other table slots.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_WORD_TO_FUNCTION(word) norrath_host_function(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_WORD_TO_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
unsigned char* GetModelView__C8VIRaster(unsigned char* raster);
unsigned Sprite__7VIScenei(unsigned char* scene, int index);

int Pick__11VILODSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(unsigned char* self, unsigned char* segment, unsigned char* filter, unsigned char* scene, unsigned char* raster) {
    const float* view = reinterpret_cast<const float*>(GetModelView__C8VIRaster(raster));
    const float* origin = reinterpret_cast<const float*>(segment);
    float delta[3];
    delta[0] = view[12] - origin[0];
    delta[1] = view[13] - origin[1];
    delta[2] = view[14] - origin[2];
    float distance = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    int count = *reinterpret_cast<int*>(self + 68);
    int index = 0;
    while (index < count) {
        float threshold = *reinterpret_cast<float*>(self + 72 + 8 * index);
        if (!(threshold * threshold < distance)) break;
        ++index;
    }
    if (index == count) return 0;
    unsigned char* sprite = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, *reinterpret_cast<int*>(self + 76 + 8 * index)));
    unsigned char* entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 48;
    typedef int (*Slot)(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*);
    Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4)));
    return function(sprite + *reinterpret_cast<short*>(entry), segment, filter, scene, raster);
}
}
