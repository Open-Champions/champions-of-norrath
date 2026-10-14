// Observed: SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE at 0x010b8870 is 204 bytes. With (this, mode) it stores
// the mode at this+748, calls Material__8VIRasteri(word at this+772, word at this+776), then for stored modes 0..4 calls
// SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, mode) with the argument equal to the mode number (a five-entry jump
// table whose entries pass 0,1,2,3,4). Modes of 5 or more only store the value.
// Inferred: the value is translated one to one into the material's layer-0 alpha blend mode.
// Unknown: the names of the five blend modes.
extern "C" {
void* Material__8VIRasteri(int a, int b);
void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void* material, int layer, int mode);

void SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE(char* self, unsigned mode) {
    *(unsigned*)(self + 748) = mode;
    void* material = Material__8VIRasteri(*(int*)(self + 772), *(int*)(self + 776));
    switch (*(unsigned*)(self + 748)) {
    case 0: SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, 0); break;
    case 1: SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, 1); break;
    case 2: SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, 2); break;
    case 3: SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, 3); break;
    case 4: SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, 4); break;
    default: break;
    }
}
}
