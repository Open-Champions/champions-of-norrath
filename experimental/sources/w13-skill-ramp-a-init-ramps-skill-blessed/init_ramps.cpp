// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a6e18 (84 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +22  halfword 1
//   +24  halfword 90
//   +26  halfword 30
//   +28  halfword 1
//   +32  halfword 1
//   +36  float 1e+01f (bits 0x41200000)
//   +40  float 0.0f (bits 0x00000000)
//   +44  float 0.33339998f (bits 0x3eaab367)
//   +52  float 1.0f (bits 0x3f800000)
extern "C" void init_ramps(void*) asm("initRamps__12SkillBlessed");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 90;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 30;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 1;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.4000000000000p+3f;  // 1e+01f
    *reinterpret_cast<float*>(bytes + 40) = 0.0f;  // 0.0f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.5566ce0000000p-2f;  // 0.33339998f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.0000000000000p+0f;  // 1.0f
}
