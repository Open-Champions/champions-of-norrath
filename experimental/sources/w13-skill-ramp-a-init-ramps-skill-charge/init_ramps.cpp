// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x00291fc8 (112 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 10
//   +18  halfword 10
//   +20  halfword 8
//   +22  halfword 1
//   +24  halfword 120
//   +26  halfword 55
//   +28  halfword 3
//   +30  halfword 1
//   +32  halfword 35
//   +36  float 9.999759f (bits 0x411fff03)
//   +40  float 2.99998f (bits 0x403fffac)
//   +44  float 1.3309809f (bits 0x3faa5d95)
//   +48  float 0.332995f (bits 0x3eaa7e52)
//   +52  float 2.99995f (bits 0x403fff2e)
extern "C" void init_ramps(void*) asm("initRamps__11SkillCharge");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 10;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 10;
    *reinterpret_cast<unsigned short*>(bytes + 20) = 8;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 120;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 55;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 3;
    *reinterpret_cast<unsigned short*>(bytes + 30) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 35;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.3ffe060000000p+3f;  // 9.999759f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.7fff580000000p+1f;  // 2.99998f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.54bb2a0000000p+0f;  // 1.3309809f
    *reinterpret_cast<float*>(bytes + 48) = 0x1.54fca40000000p-2f;  // 0.332995f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.7ffe5c0000000p+1f;  // 2.99995f
}
