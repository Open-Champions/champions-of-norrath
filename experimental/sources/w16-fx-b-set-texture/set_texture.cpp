// Observed: SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary at 0x010b89c8 is 96 bytes. With (this, id, raster, dictionary) it
// stores id at this+780, calls Material__8VIRasteri(word at this+772, word at this+776), then calls
// SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, 0, word at this+780, raster, dictionary).
// Inferred: installs a texture id/raster as layer 0 of the definition's material.
// Unknown: the meaning of the id beyond being passed through.
extern "C" {
void* Material__8VIRasteri(int a, int b);
void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void* material, int layer, int id, void* raster, void* dictionary);

void SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary(char* self, int id, void* raster, void* dictionary) {
    *(int*)(self + 780) = id;
    void* material = Material__8VIRasteri(*(int*)(self + 772), *(int*)(self + 776));
    SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, 0, *(int*)(self + 780), raster, dictionary);
}
}
