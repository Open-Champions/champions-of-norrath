// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b2d08 (72 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 5
//   +24  halfword 18
//   +26  halfword 35
//   +28  halfword 1
//   +36  float 4.0000057f (bits 0x4080000c)
//   +40  float 9.999926f (bits 0x411fffb2)
//   +44  float 0.25f (bits 0x3e800000)
extern "C" void init_ramps(void*) asm("initRamps__15SkillHolyStrike");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 18;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 35;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 1;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.0000180000000p+2f;  // 4.0000057f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.3fff640000000p+3f;  // 9.999926f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.0000000000000p-2f;  // 0.25f
}
