// Observed: SetHair__9VICSpriteiiiR7VIScene at 0x010aed40, 936 bytes. Arguments (this, a, b, c, scene); the scene is the fifth argument
// (register t0). raster = word at scene+19356 and dictionary = word at scene+19352.
//  1. A zero raster or dictionary returns -1. Otherwise, if any of the five words at this+4164, +4188, +4212, +4236, +4260 equals -1,
//     zero is returned without writing anything.
//  2. a, b, c are stored at this+4352, +4356, +4360. GetHairTexture__13VICSpriteCusti(a) is called with a clamped to 0..7 (a of 8
//     or more gives 7, negative gives 0). When the result is negative the image's _13VICSpriteCust$HairNull word replaces it, and when
//     that is also negative zero is returned. Call the chosen texture T and the HairNull word N.
//  3. Each of the following slots is a pair (material index word, texture id word) 24 bytes apart. For a slot, the material is
//     GetMaterialPalEntry__8VIRasterii(raster, this[416], index word) and SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary
//     (material, texture id word, T or N, raster, dictionary) is called:
//     - three slots k=0..2 at index words this+4092+24k with ids at +4096+24k: skipped when the index word is -1; T when k < c, else N.
//     - three slots k=0..2 at index words this+4284+24k with ids at +4288+24k: skipped when the index word is -1; T when k < c and
//       this[4420] is -1, else N.
//     - three slots k=0..2 at index words this+4164+24k with ids at +4168+24k (never skipped): T when k < b and this[4420] is -1, else N.
//     - one slot at index word this+4236, id this+4240: T when b >= 2 and c >= 2 and this[4420] is -1, else N.
//     - one slot at index word this+4260, id this+4264: always T.
//  4. Zero is returned.
// Inferred: this chooses hair textures for hair material layers of a customisable sprite; the offsets between slots indicate a table
// of 24-byte slot records. This+4420 acts as a flag that is -1 when some optional part is unset.
// Unknown: the meanings of a, b, c (they look like style/length selectors) and the field names; the type of this+416.
extern "C" {
extern int norrath_hair_null __asm__("_13VICSpriteCust$HairNull");

int GetHairTexture__13VICSpriteCusti(int index);
void* GetMaterialPalEntry__8VIRasterii(void* raster, int handle, int index);
void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void* material, int layer, int texture, void* raster, void* dictionary);

static inline int word(char* self, int offset) { return *(int*)(self + offset); }

int SetHair__9VICSpriteiiiR7VIScene(char* self, int a, int b, int c, char* scene) {
    void* raster = *(void**)(scene + 19356);
    if (!raster) return -1;
    void* dictionary = *(void**)(scene + 19352);
    if (!dictionary) return -1;
    for (int offset = 4164; offset <= 4260; offset += 24)
        if (word(self, offset) == -1) return 0;
    *(int*)(self + 4352) = a;
    *(int*)(self + 4356) = b;
    *(int*)(self + 4360) = c;
    int clamped = 7;
    if (a < 8) clamped = a < 0 ? 0 : a;
    int texture = GetHairTexture__13VICSpriteCusti(clamped);
    int null_texture = norrath_hair_null;
    if (texture < 0) {
        texture = null_texture;
        if (texture < 0) return 0;
    }
    for (int k = 0; k < 3; k++) {
        int index = word(self, 4092 + 24 * k);
        if (index == -1) continue;
        void* material = GetMaterialPalEntry__8VIRasterii(raster, word(self, 416), index);
        SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, word(self, 4096 + 24 * k), k < c ? texture : null_texture, raster, dictionary);
    }
    for (int k = 0; k < 3; k++) {
        int index = word(self, 4284 + 24 * k);
        if (index == -1) continue;
        void* material = GetMaterialPalEntry__8VIRasterii(raster, word(self, 416), index);
        bool use = k < c && word(self, 4420) == -1;
        SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, word(self, 4288 + 24 * k), use ? texture : null_texture, raster, dictionary);
    }
    for (int k = 0; k < 3; k++) {
        void* material = GetMaterialPalEntry__8VIRasterii(raster, word(self, 416), word(self, 4164 + 24 * k));
        bool use = k < b && word(self, 4420) == -1;
        SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, word(self, 4168 + 24 * k), use ? texture : null_texture, raster, dictionary);
    }
    {
        bool use = b >= 2 && c >= 2 && word(self, 4420) == -1;
        void* material = GetMaterialPalEntry__8VIRasterii(raster, word(self, 416), word(self, 4236));
        SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, word(self, 4240), use ? texture : null_texture, raster, dictionary);
    }
    void* material = GetMaterialPalEntry__8VIRasterii(raster, word(self, 416), word(self, 4260));
    SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, word(self, 4264), texture, raster, dictionary);
    return 0;
}
}
