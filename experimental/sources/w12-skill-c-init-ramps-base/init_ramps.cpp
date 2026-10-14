// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00313a08 (76 bytes):
// For i in [0,4], store zero halfword at +14+2*i, float 2.0 at +36+4*i,
// and halfword 1 at +24+2*i. Other object fields are not written.
extern "C" void init_ramps(void*) asm("initRamps__5Skill");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    for (unsigned i = 0; i != 5; ++i) {
        *reinterpret_cast<unsigned short*>(bytes + 14 + 2 * i) = 0;
        *reinterpret_cast<float*>(bytes + 36 + 4 * i) = 2.0f;
        *reinterpret_cast<unsigned short*>(bytes + 24 + 2 * i) = 1;
    }
}
