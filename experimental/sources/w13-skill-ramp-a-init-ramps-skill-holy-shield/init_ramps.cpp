// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002ae470 (68 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +22  halfword 1
//   +24  halfword 5
//   +26  halfword 10
//   +32  halfword 24
//   +36  float 2.9999788f (bits 0x403fffa7)
//   +40  float 0.250002f (bits 0x3e800043)
//   +52  float 2.99999f (bits 0x403fffd6)
extern "C" void init_ramps(void*) asm("initRamps__15SkillHolyShield");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 10;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 24;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.7fff4e0000000p+1f;  // 2.9999788f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.0000860000000p-2f;  // 0.250002f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.7fffac0000000p+1f;  // 2.99999f
}
