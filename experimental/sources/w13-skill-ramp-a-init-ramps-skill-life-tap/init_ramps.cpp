// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002be1e8 (48 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +24  halfword 10
//   +26  halfword 15
//   +36  float 0.748994f (bits 0x3f3fbe12)
//   +40  float 4.999902f (bits 0x409fff32)
extern "C" void init_ramps(void*) asm("initRamps__12SkillLifeTap");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 10;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 15;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.7f7c240000000p-1f;  // 0.748994f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.3ffe640000000p+2f;  // 4.999902f
}
