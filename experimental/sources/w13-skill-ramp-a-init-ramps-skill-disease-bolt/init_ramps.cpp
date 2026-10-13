// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00293768 (88 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 5
//   +24  halfword 23
//   +26  halfword 1
//   +36  float 4.0f (bits 0x40800000)
//   +40  float 0.33299997f (bits 0x3eaa7ef9)
//   +56  halfword 1
//   +58  halfword 1
//   +66  halfword 2
//   +68  halfword 1
//   +76  float 2.0000038f (bits 0x40000010)
//   +80  float 0.099999994f (bits 0x3dcccccc)
extern "C" void init_ramps(void*) asm("initRamps__16SkillDiseaseBolt");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 23;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 1;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.0000000000000p+2f;  // 4.0f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.54fdf20000000p-2f;  // 0.33299997f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 58) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 68) = 1;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.0000200000000p+1f;  // 2.0000038f
    *reinterpret_cast<float*>(bytes + 80) = 0x1.9999980000000p-4f;  // 0.099999994f
}
