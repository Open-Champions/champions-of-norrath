// Observed: InitTextSlots__9VICSpriteR8VIRaster at 0x010b2ee8 is 168 bytes in the pinned SLUS-20565 image. Arguments: the sprite (this)
// and a raster. It walks 19 records of 24 bytes; record k starts at sprite offset 3900 + 24 * k and is used as follows: the word at
// +0 is a material entry index (-1 means the record is skipped), the word at +4 is a layer number, and the word at +8 receives a
// result. For each record whose first word is not -1 it calls GetMaterialPalEntry__8VIRasterii(raster, the sprite word at +416,
// entry index) and then GetLayerTexture__10VIMateriali(that result, layer number) and stores the result in the record's +8 word.
// It returns 0.
// Inferred: the records are text slots that bind a layer of a palette material to a texture word; the sprite word at +416 is the
// material palette index used for all of them.
// Unknown: the record names, the size of the sprite, what the +12 .. +23 bytes of each record hold, and the two callees' behaviour
// (both are imports of this unit). Nothing here ran on the EE.
extern "C" {
unsigned GetMaterialPalEntry__8VIRasterii(unsigned char* raster, int palette, int entry);
unsigned GetLayerTexture__10VIMateriali(unsigned material, int layer);
int InitTextSlots__9VICSpriteR8VIRaster(unsigned char* self, unsigned char* raster) {
    for (int k = 0; k < 19; ++k) {
        unsigned char* record = self + 3900 + 24 * k;
        int entry = *reinterpret_cast<int*>(record);
        if (entry != -1) {
            unsigned material = GetMaterialPalEntry__8VIRasterii(raster, *reinterpret_cast<int*>(self + 416), entry);
            *reinterpret_cast<unsigned*>(record + 8) = GetLayerTexture__10VIMateriali(material, *reinterpret_cast<int*>(record + 4));
        }
    }
    return 0;
}
}
