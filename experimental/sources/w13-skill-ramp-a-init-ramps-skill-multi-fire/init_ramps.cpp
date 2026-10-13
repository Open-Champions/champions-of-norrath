// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002a1be0 (148 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +22  halfword 1
//   +24  halfword 120
//   +26  halfword 478
//   +28  halfword 2
//   +32  halfword 5
//   +36  float 10.000848f (bits 0x41200379)
//   +40  float 7.294914f (bits 0x40e96fef)
//   +44  float 0.20100199f (bits 0x3e4dd377)
//   +52  float 1.999977f (bits 0x3fffff3f)
//   +56  halfword 1
//   +58  halfword 1
//   +60  halfword 1
//   +66  halfword 2
//   +68  halfword 450
//   +70  halfword 2
//   +76  float 2.0f (bits 0x40000000)
//   +80  float 4.0f (bits 0x40800000)
//   +84  float 0.099999994f (bits 0x3dcccccc)
extern "C" void init_ramps(void*) asm("initRamps__14SkillMultiFire");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 120;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 478;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 5;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.4006f20000000p+3f;  // 10.000848f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.d2dfde0000000p+2f;  // 7.294914f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.9ba6ee0000000p-3f;  // 0.20100199f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.fffe7e0000000p+0f;  // 1.999977f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 58) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 60) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 68) = 450;
    *reinterpret_cast<unsigned short*>(bytes + 70) = 2;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.0000000000000p+1f;  // 2.0f
    *reinterpret_cast<float*>(bytes + 80) = 0x1.0000000000000p+2f;  // 4.0f
    *reinterpret_cast<float*>(bytes + 84) = 0x1.9999980000000p-4f;  // 0.099999994f
}
