// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0029fb70 (132 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +20  halfword 1
//   +22  halfword 1
//   +24  halfword 40
//   +26  halfword 498
//   +28  halfword 6
//   +30  halfword 3
//   +32  halfword 14
//   +36  float 5.0008416f (bits 0x40a006e5)
//   +40  float 5.6045146f (bits 0x40b3582f)
//   +44  float 4.9999785f (bits 0x409fffd3)
//   +48  float 0.332003f (bits 0x3ea9fc4c)
//   +52  float 2.99997f (bits 0x403fff82)
//   +56  halfword 1
//   +66  halfword 2
//   +76  float 2.0f (bits 0x40000000)
extern "C" void init_ramps(void*) asm("initRamps__14SkillColdArrow");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 20) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 40;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 498;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 6;
    *reinterpret_cast<unsigned short*>(bytes + 30) = 3;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 14;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.400dca0000000p+2f;  // 5.0008416f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.66b05e0000000p+2f;  // 5.6045146f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.3fffa60000000p+2f;  // 4.9999785f
    *reinterpret_cast<float*>(bytes + 48) = 0x1.53f8980000000p-2f;  // 0.332003f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.7fff040000000p+1f;  // 2.99997f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.0000000000000p+1f;  // 2.0f
}
