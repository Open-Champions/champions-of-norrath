// Observed: DeltaTime__C16VIParticleSystem at 0x010b8ff8 is 8 bytes. It returns the float at
// offset 16 of the particle system. The neighbouring Time__C16VIParticleSystem (not authored
// here) is the same shape at a different offset.
// Inferred: the float is the time step of the last update; the name comes from the symbol.
// Unknown: which routine writes it.
extern "C" {
float DeltaTime__C16VIParticleSystem(const unsigned char* system) {
    return *reinterpret_cast<const float*>(system + 16);
}
}
