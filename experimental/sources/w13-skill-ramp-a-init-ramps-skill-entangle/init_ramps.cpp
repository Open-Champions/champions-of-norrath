// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002c6e00 (80 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +22  halfword 1
//   +24  halfword 1
//   +26  halfword 5
//   +28  halfword 50
//   +32  halfword 30
//   +36  float 0.33299997f (bits 0x3eaa7ef9)
//   +40  float 0.33299997f (bits 0x3eaa7ef9)
//   +44  float 1.9999709f (bits 0x3fffff0c)
//   +52  float 3.00099f (bits 0x40401038)
extern "C" void init_ramps(void*) asm("initRamps__13SkillEntangle");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 50;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 30;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.54fdf20000000p-2f;  // 0.33299997f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.54fdf20000000p-2f;  // 0.33299997f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.fffe180000000p+0f;  // 1.9999709f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.8020700000000p+1f;  // 3.00099f
}
