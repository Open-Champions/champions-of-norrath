// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0015e9a0 (32 bytes):
// store halfword 1 at receiver+56, float 2.0 at +76, and halfword 4 at +66.
// Member names/layout beyond these byte offsets are not inferred.
extern "C" void init_ramps(void*) asm("initRamps__19SkillDiseaseWeapons");
extern "C" void init_ramps(void* object) { auto* b=static_cast<unsigned char*>(object); *reinterpret_cast<unsigned short*>(b+56)=1; *reinterpret_cast<float*>(b+76)=2.0f; *reinterpret_cast<unsigned short*>(b+66)=4; }
