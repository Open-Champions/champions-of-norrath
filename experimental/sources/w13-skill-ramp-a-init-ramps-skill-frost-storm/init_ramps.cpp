// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b3510 (180 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +20  halfword 1
//   +22  halfword 1
//   +24  halfword 20
//   +26  halfword 2
//   +28  halfword 2
//   +30  halfword 31
//   +32  halfword 40
//   +36  float 3.9999828f (bits 0x407fffb8)
//   +40  float 0.336983f (bits 0x3eac8909)
//   +44  float 0.333001f (bits 0x3eaa7f1b)
//   +48  float 2.0469859f (bits 0x400301d1)
//   +52  float 4.999758f (bits 0x409ffe04)
//   +56  halfword 1
//   +58  halfword 1
//   +60  halfword 1
//   +62  halfword 1
//   +66  halfword 2
//   +68  halfword 2
//   +70  halfword 3
//   +72  halfword 31
//   +76  float 2.0f (bits 0x40000000)
//   +80  float 0.099999994f (bits 0x3dcccccc)
//   +84  float 0.049999997f (bits 0x3d4ccccc)
//   +88  float 1.0f (bits 0x3f800000)
extern "C" void init_ramps(void*) asm("initRamps__15SkillFrostStorm");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 20) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 20;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 30) = 31;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 40;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.ffff700000000p+1f;  // 3.9999828f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.5912120000000p-2f;  // 0.336983f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.54fe360000000p-2f;  // 0.333001f
    *reinterpret_cast<float*>(bytes + 48) = 0x1.0603a20000000p+1f;  // 2.0469859f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.3ffc080000000p+2f;  // 4.999758f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 58) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 60) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 62) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 68) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 70) = 3;
    *reinterpret_cast<unsigned short*>(bytes + 72) = 31;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.0000000000000p+1f;  // 2.0f
    *reinterpret_cast<float*>(bytes + 80) = 0x1.9999980000000p-4f;  // 0.099999994f
    *reinterpret_cast<float*>(bytes + 84) = 0x1.9999980000000p-5f;  // 0.049999997f
    *reinterpret_cast<float*>(bytes + 88) = 0x1.0000000000000p+0f;  // 1.0f
}
