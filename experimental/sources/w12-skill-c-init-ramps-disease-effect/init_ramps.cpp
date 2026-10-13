// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0015eae0 (44 bytes):
// stores halfword 1 at +66, float 1.0 at +76, halfword 3 at +24,
// halfword 1 at +14, float 1.0 at +36, and halfword 1 at +56.
// Member names/layout beyond these byte offsets are not inferred.
extern "C" void init_ramps(void*) asm("initRamps__18SkillDiseaseEffect");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 66) = 1;
    *reinterpret_cast<float*>(bytes + 76) = 1.0f;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 3;
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<float*>(bytes + 36) = 1.0f;
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
}
