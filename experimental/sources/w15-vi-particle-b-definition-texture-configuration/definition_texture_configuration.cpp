// Observed: TextureConfiguration__C22VIParticleDefinitionEx at 0x010b4c98, 8 bytes. It returns the signed int word at byte 760 of its argument and writes nothing.
// Inferred: a word of the extended particle definition, apparently a texture configuration value. Unknown: the member name.
// Nothing here ran on the EE; the paired comparison against the original used a private interpreter.
extern "C" {
int TextureConfiguration__C22VIParticleDefinitionEx(const void* object) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 760);
}
}
