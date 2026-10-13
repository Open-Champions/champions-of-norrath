// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ac360 (116 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 6
//   +20  halfword 6
//   +24  halfword 14
//   +26  halfword 15
//   +28  halfword 12
//   +30  halfword 25
//   +36  float 2.0f (bits 0x40000000)
//   +40  float 2.0f (bits 0x40000000)
//   +44  float 1.0f (bits 0x3f800000)
//   +48  float 0.999989f (bits 0x3f7fff47)
//   +56  halfword 1
//   +66  halfword 2
//   +76  float 2.0000148f (bits 0x4000003e)
extern "C" void init_ramps(void*) asm("initRamps__16SkillColdWeapons");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 6;
    *reinterpret_cast<unsigned short*>(bytes + 20) = 6;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 14;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 15;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 12;
    *reinterpret_cast<unsigned short*>(bytes + 30) = 25;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.0000000000000p+1f;  // 2.0f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.0000000000000p+1f;  // 2.0f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.0000000000000p+0f;  // 1.0f
    *reinterpret_cast<float*>(bytes + 48) = 0x1.fffe8e0000000p-1f;  // 0.999989f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.00007c0000000p+1f;  // 2.0000148f
}
