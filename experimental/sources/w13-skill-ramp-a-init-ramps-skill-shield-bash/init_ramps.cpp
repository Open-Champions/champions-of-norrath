// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002c89d8 (104 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +20  halfword 1
//   +22  halfword 1
//   +24  halfword 30
//   +26  halfword 20
//   +28  halfword 4
//   +30  halfword 1
//   +32  halfword 27
//   +36  float 4.9999847f (bits 0x409fffe0)
//   +40  float 2.0000048f (bits 0x40000014)
//   +44  float 0.499999f (bits 0x3effffde)
//   +48  float 0.25000098f (bits 0x3e800021)
//   +52  float 4.0019736f (bits 0x4080102b)
extern "C" void init_ramps(void*) asm("initRamps__15SkillShieldBash");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 20) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 30;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 20;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 4;
    *reinterpret_cast<unsigned short*>(bytes + 30) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 27;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.3fffc00000000p+2f;  // 4.9999847f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.0000280000000p+1f;  // 2.0000048f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.ffffbc0000000p-2f;  // 0.499999f
    *reinterpret_cast<float*>(bytes + 48) = 0x1.0000420000000p-2f;  // 0.25000098f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.0020560000000p+2f;  // 4.0019736f
}
