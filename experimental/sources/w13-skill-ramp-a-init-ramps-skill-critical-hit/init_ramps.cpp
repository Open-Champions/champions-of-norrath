// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002adba0 (88 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +22  halfword 1
//   +24  halfword 10
//   +26  halfword 6
//   +28  halfword 20
//   +32  halfword 36
//   +36  float 2.999985f (bits 0x403fffc1)
//   +40  float 1.0009929f (bits 0x3f802089)
//   +44  float 0.999986f (bits 0x3f7fff15)
//   +52  float 3.9999938f (bits 0x407fffe6)
extern "C" void init_ramps(void*) asm("initRamps__16SkillCriticalHit");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 10;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 6;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 20;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 36;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.7fff820000000p+1f;  // 2.999985f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.0041120000000p+0f;  // 1.0009929f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.fffe2a0000000p-1f;  // 0.999986f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.ffffcc0000000p+1f;  // 3.9999938f
}
