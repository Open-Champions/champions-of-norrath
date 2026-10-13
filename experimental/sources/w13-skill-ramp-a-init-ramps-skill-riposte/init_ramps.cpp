// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a5088 (84 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 0
//   +16  halfword 1
//   +24  halfword 20
//   +26  halfword 10
//   +36  float 4.0f (bits 0x40800000)
//   +40  float 0.0f (bits 0x00000000)
//   +56  halfword 1
//   +58  halfword 1
//   +66  halfword 2
//   +68  halfword 15
//   +76  float 2.0000088f (bits 0x40000025)
//   +80  float 0.0f (bits 0x00000000)
extern "C" void init_ramps(void*) asm("initRamps__12SkillRiposte");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 0;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 20;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 10;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.0000000000000p+2f;  // 4.0f
    *reinterpret_cast<float*>(bytes + 40) = 0.0f;  // 0.0f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 58) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 68) = 15;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.00004a0000000p+1f;  // 2.0000088f
    *reinterpret_cast<float*>(bytes + 80) = 0.0f;  // 0.0f
}
