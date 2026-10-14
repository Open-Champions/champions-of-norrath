// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b6a10 (92 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +24  halfword 275
//   +26  halfword 30
//   +36  float 5.0f (bits 0x40a00000)
//   +40  float 0.19999999f (bits 0x3e4ccccc)
//   +56  halfword 1
//   +58  halfword 1
//   +66  halfword 2
//   +68  halfword 30
//   +76  float 2.0f (bits 0x40000000)
//   +80  float 0.049999997f (bits 0x3d4ccccc)
extern "C" void init_ramps(void*) asm("initRamps__15SkillConeOfFire");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 275;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 30;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.4000000000000p+2f;  // 5.0f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.9999980000000p-3f;  // 0.19999999f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 58) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 68) = 30;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.0000000000000p+1f;  // 2.0f
    *reinterpret_cast<float*>(bytes + 80) = 0x1.9999980000000p-5f;  // 0.049999997f
}
