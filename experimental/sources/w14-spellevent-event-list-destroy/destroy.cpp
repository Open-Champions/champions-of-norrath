// Observed: Destroy__16VISpellEventList at 0x010001c0 is 256 bytes in the pinned SLUS-20565 image
// (a .srs function); returns nothing. Field words of the list object: +0 scene pointer, +4 record
// count, +8 pointer to the record array (records of 44 bytes, first int is the record type).
//   1. When the word at +8 is zero it stores zero at +0 and returns; +4 and +8 stay as they were.
//   2. Otherwise for every record index below the count (the count and the array pointer are read
//      again after each release): type 3 with its int at record+8 not -1 calls
//      ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(scene + 1352, that value,
//      the word at scene + 19352); type 9 with its int at record+8 not -1 calls
//      ReleaseSprite__7VIScenei(scene, that value); other types do nothing.
//   3. Then, when the array pointer is non-zero, __builtin_vec_delete(array) is called; then the words
//      at +8, +4 and +0 are all set to zero.
// Inferred: the destroy half of a spell event list: record types 3 (create emitter) and 9 (create
// sprite) are the two that hold a resource number at record+8 which the list acquired when it was
// created, so they are released here; the scene offset 1352 is the particle system and the word at
// scene+19352 a dictionary, as in NewCreateEmitter.
// Unknown: the record layout beyond type and +8, and whether the dictionary word is a pointer or a
// reference to an embedded object. Nothing here ran on the EE.
extern "C" {
void ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(void* system, int index, void* dictionary);
void ReleaseSprite__7VIScenei(void* scene, int sprite);
void __builtin_vec_delete(void* array);

void Destroy__16VISpellEventList(unsigned char* self) {
    unsigned char* records = *reinterpret_cast<unsigned char**>(self + 8);
    if (records == 0) {
        *reinterpret_cast<unsigned*>(self + 0) = 0;
        return;
    }
    int count = *reinterpret_cast<int*>(self + 4);
    for (int i = 0; i < count; ++i) {
        unsigned char* record = records + 44 * i;
        int type = *reinterpret_cast<int*>(record);
        if (type == 3) {
            int id = *reinterpret_cast<int*>(record + 8);
            if (id != -1) {
                unsigned char* scene = *reinterpret_cast<unsigned char**>(self + 0);
                ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(scene + 1352, id, *reinterpret_cast<void**>(scene + 19352));
                records = *reinterpret_cast<unsigned char**>(self + 8);
                count = *reinterpret_cast<int*>(self + 4);
            }
        } else if (type == 9) {
            int id = *reinterpret_cast<int*>(record + 8);
            if (id != -1) {
                ReleaseSprite__7VIScenei(*reinterpret_cast<void**>(self + 0), id);
                records = *reinterpret_cast<unsigned char**>(self + 8);
                count = *reinterpret_cast<int*>(self + 4);
            }
        }
    }
    if (records != 0) {
        __builtin_vec_delete(records);
    }
    *reinterpret_cast<unsigned*>(self + 8) = 0;
    *reinterpret_cast<unsigned*>(self + 4) = 0;
    *reinterpret_cast<unsigned*>(self + 0) = 0;
}
}
