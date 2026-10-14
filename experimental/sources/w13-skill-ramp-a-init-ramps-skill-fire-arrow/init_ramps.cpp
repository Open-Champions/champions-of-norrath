// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0029e2f8 (112 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +22  halfword 1
//   +24  halfword 31
//   +26  halfword 395
//   +28  halfword 8
//   +32  halfword 25
//   +36  float 5.000855f (bits 0x40a00701)
//   +40  float 22.43477f (bits 0x41b37a69)
//   +44  float 0.41499898f (bits 0x3ed47abf)
//   +52  float 2.999981f (bits 0x403fffb0)
//   +56  halfword 1
//   +66  halfword 2
//   +76  float 2.0f (bits 0x40000000)
extern "C" void init_ramps(void*) asm("initRamps__14SkillFireArrow");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 31;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 395;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 8;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 25;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.400e020000000p+2f;  // 5.000855f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.66f4d20000000p+4f;  // 22.43477f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.a8f57e0000000p-2f;  // 0.41499898f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.7fff600000000p+1f;  // 2.999981f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.0000000000000p+1f;  // 2.0f
}
