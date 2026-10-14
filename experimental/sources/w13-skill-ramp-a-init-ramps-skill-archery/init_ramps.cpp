// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a2b68 (48 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +22  halfword 1
//   +24  halfword 15
//   +32  halfword 1
//   +36  float 4.99995f (bits 0x409fff97)
//   +52  float 1.0f (bits 0x3f800000)
extern "C" void init_ramps(void*) asm("initRamps__12SkillArchery");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 15;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 1;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.3fff2e0000000p+2f;  // 4.99995f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.0000000000000p+0f;  // 1.0f
}
