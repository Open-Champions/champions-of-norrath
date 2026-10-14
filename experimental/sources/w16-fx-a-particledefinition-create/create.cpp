// Observed: Create__20VIParticleDefinitionR8VIRaster at 0x010b5f48, 200 bytes. With (this, raster) it calls CreateMaterial__8VIRasteri(raster, 0)
// and stores the result M at this+776 whatever it is. A result of -1 returns -1 (the value of M). Otherwise, in order:
// Material__8VIRasteri(raster, M) gives the material m; Init__10VIMateriali(m, 1); SetLayerFillType__10VIMateriali18VIMaterialFillType(m, 0, 0);
// SetLayerModulate__10VIMaterialii(m, 0, 1); SetDither__10VIMateriali(m, 0). It stores the raster pointer at this+772, then calls
// SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE(this, word at +748), SetZWrite__20VIParticleDefinitionb(this, word at
// +752) and SetZTest__20VIParticleDefinitionb(this, word at +756) and returns 0. The three words are passed as loaded.
// Inferred: this builds the particle material from the blend mode and z flags that the ESF parse stored in the definition earlier, and
// keeps the raster and material number for later release. A failed material creation leaves -1 at +776.
// Unknown: the enum names of the fill type and blend mode, and the exact types of the +748, +752 and +756 words.
extern "C" {
int CreateMaterial__8VIRasteri(void* raster, int flags);
void* Material__8VIRasteri(void* raster, int index);
void Init__10VIMateriali(void* material, int value);
void SetLayerFillType__10VIMateriali18VIMaterialFillType(void* material, int layer, int type);
void SetLayerModulate__10VIMaterialii(void* material, int layer, int value);
void SetDither__10VIMateriali(void* material, int value);
void SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE(void* self, int mode);
void SetZWrite__20VIParticleDefinitionb(void* self, int value);
void SetZTest__20VIParticleDefinitionb(void* self, int value);

int Create__20VIParticleDefinitionR8VIRaster(unsigned char* self, void* raster) {
    int created = CreateMaterial__8VIRasteri(raster, 0);
    *reinterpret_cast<int*>(self + 776) = created;
    if (created == -1) return created;
    void* material = Material__8VIRasteri(raster, created);
    Init__10VIMateriali(material, 1);
    SetLayerFillType__10VIMateriali18VIMaterialFillType(material, 0, 0);
    SetLayerModulate__10VIMaterialii(material, 0, 1);
    SetDither__10VIMateriali(material, 0);
    *reinterpret_cast<unsigned*>(self + 772) = static_cast<unsigned>(reinterpret_cast<unsigned long>(raster));
    SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE(self, *reinterpret_cast<int*>(self + 748));
    SetZWrite__20VIParticleDefinitionb(self, *reinterpret_cast<int*>(self + 752));
    SetZTest__20VIParticleDefinitionb(self, *reinterpret_cast<int*>(self + 756));
    return 0;
}
}
