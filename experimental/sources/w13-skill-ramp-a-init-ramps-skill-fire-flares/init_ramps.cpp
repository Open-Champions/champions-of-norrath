// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a4888 (88 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 7
//   +18  halfword 1
//   +20  halfword 1
//   +24  halfword 86
//   +26  halfword 32
//   +28  halfword 8
//   +30  halfword 1
//   +36  float 3.99999f (bits 0x407fffd6)
//   +40  float 0.999999f (bits 0x3f7fffef)
//   +44  float 0.999994f (bits 0x3f7fff9b)
//   +48  float 0.33299997f (bits 0x3eaa7ef9)
extern "C" void init_ramps(void*) asm("initRamps__15SkillFireFlares");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 7;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 20) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 86;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 32;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 8;
    *reinterpret_cast<unsigned short*>(bytes + 30) = 1;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.ffffac0000000p+1f;  // 3.99999f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.ffffde0000000p-1f;  // 0.999999f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.ffff360000000p-1f;  // 0.999994f
    *reinterpret_cast<float*>(bytes + 48) = 0x1.54fdf20000000p-2f;  // 0.33299997f
}
