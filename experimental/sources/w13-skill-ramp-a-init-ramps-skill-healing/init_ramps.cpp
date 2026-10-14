// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002af238 (84 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 5
//   +18  halfword 5
//   +22  halfword 1
//   +24  halfword 15
//   +26  halfword 5
//   +28  halfword 15
//   +32  halfword 88
//   +36  float 5.0000567f (bits 0x40a00077)
//   +40  float 0.499957f (bits 0x3efffa5d)
//   +44  float 4.9999766f (bits 0x409fffcf)
//   +52  float 9.999557f (bits 0x411ffe2f)
extern "C" void init_ramps(void*) asm("initRamps__12SkillHealing");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 15;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 15;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 88;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.4000ee0000000p+2f;  // 5.0000567f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.fff4ba0000000p-2f;  // 0.499957f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.3fff9e0000000p+2f;  // 4.9999766f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.3ffc5e0000000p+3f;  // 9.999557f
}
