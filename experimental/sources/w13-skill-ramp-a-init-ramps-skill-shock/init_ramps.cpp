// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0028d840 (108 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 5
//   +22  halfword 1
//   +24  halfword 57
//   +26  halfword 1
//   +32  halfword 32
//   +36  float 6.9998956f (bits 0x40dfff25)
//   +40  float 0.33299598f (bits 0x3eaa7e73)
//   +52  float 5.999881f (bits 0x40bfff06)
//   +56  halfword 1
//   +58  halfword 10
//   +66  halfword 2
//   +68  halfword 1
//   +76  float 2.000002f (bits 0x40000008)
//   +80  float 0.099999994f (bits 0x3dcccccc)
extern "C" void init_ramps(void*) asm("initRamps__10SkillShock");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 57;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 32;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.bffe4a0000000p+2f;  // 6.9998956f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.54fce60000000p-2f;  // 0.33299598f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.7ffe0c0000000p+2f;  // 5.999881f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 58) = 10;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 68) = 1;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.0000100000000p+1f;  // 2.000002f
    *reinterpret_cast<float*>(bytes + 80) = 0x1.9999980000000p-4f;  // 0.099999994f
}
