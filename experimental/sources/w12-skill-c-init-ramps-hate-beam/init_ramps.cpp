// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0015ea40 (32 bytes):
// store halfword 1 at receiver+56, float 2.0 at +76, and halfword 2 at +66.
// Member names/layout beyond these byte offsets are not inferred.
extern "C" void init_ramps(void*) asm("initRamps__13SkillHateBeam");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<float*>(bytes + 76) = 2.0f;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
}
