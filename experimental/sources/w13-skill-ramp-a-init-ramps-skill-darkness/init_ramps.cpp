// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00298180 (100 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +20  halfword 1
//   +22  halfword 1
//   +24  halfword 50
//   +26  halfword 20
//   +28  halfword 1
//   +30  halfword 1
//   +32  halfword 33
//   +36  float 1.99998f (bits 0x3fffff58)
//   +40  float 3.000002f (bits 0x40400008)
//   +44  float 0.33300298f (bits 0x3eaa7f5e)
//   +48  float 0.50000095f (bits 0x3f000010)
//   +52  float 3.9998338f (bits 0x407ffd47)
extern "C" void init_ramps(void*) asm("initRamps__13SkillDarkness");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 20) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 50;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 20;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 30) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 33;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.fffeb00000000p+0f;  // 1.99998f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.8000100000000p+1f;  // 3.000002f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.54febc0000000p-2f;  // 0.33300298f
    *reinterpret_cast<float*>(bytes + 48) = 0x1.0000200000000p-1f;  // 0.50000095f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.fffa8e0000000p+1f;  // 3.9998338f
}
