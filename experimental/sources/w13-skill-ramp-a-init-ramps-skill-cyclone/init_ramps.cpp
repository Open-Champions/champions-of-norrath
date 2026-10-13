// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0029a7c8 (84 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +22  halfword 1
//   +24  halfword 30
//   +26  halfword 1
//   +28  halfword 40
//   +32  halfword 24
//   +36  float 2.9999928f (bits 0x403fffe2)
//   +40  float 0.19999999f (bits 0x3e4ccccc)
//   +44  float 1.999967f (bits 0x3ffffeeb)
//   +52  float 2.9999008f (bits 0x403ffe60)
extern "C" void init_ramps(void*) asm("initRamps__12SkillCyclone");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 30;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 40;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 24;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.7fffc40000000p+1f;  // 2.9999928f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.9999980000000p-3f;  // 0.19999999f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.fffdd60000000p+0f;  // 1.999967f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.7ffcc00000000p+1f;  // 2.9999008f
}
