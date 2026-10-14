// Observed: SetTextureConfiguration__20VIParticleDefinitioni at 0x010b8a28 is 8 bytes. It stores its second argument at this+760.
// Unknown: what the stored configuration value means to the renderer.
extern "C" {
void SetTextureConfiguration__20VIParticleDefinitioni(char* self, int configuration) {
    *(int*)(self + 760) = configuration;
}
}
