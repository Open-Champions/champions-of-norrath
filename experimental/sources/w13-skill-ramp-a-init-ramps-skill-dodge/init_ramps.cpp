// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ae6e8 (48 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +24  halfword 5
//   +36  float 2.9999838f (bits 0x403fffbc)
//   +56  halfword 1
//   +66  halfword 5
//   +76  float 1.0f (bits 0x3f800000)
extern "C" void init_ramps(void*) asm("initRamps__10SkillDodge");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 5;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.7fff780000000p+1f;  // 2.9999838f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 5;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.0000000000000p+0f;  // 1.0f
}
