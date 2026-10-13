// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002afcd0 (84 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +22  halfword 1
//   +24  halfword 225
//   +26  halfword 1
//   +28  halfword 20
//   +32  halfword 36
//   +36  float 5.0009446f (bits 0x40a007bd)
//   +40  float 3.999971f (bits 0x407fff86)
//   +44  float 1.0009539f (bits 0x3f801f42)
//   +52  float 3.0009768f (bits 0x40401001)
extern "C" void init_ramps(void*) asm("initRamps__16SkillConeOfFrost");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 225;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 20;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 36;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.400f7a0000000p+2f;  // 5.0009446f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.ffff0c0000000p+1f;  // 3.999971f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.003e840000000p+0f;  // 1.0009539f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.8020020000000p+1f;  // 3.0009768f
}
