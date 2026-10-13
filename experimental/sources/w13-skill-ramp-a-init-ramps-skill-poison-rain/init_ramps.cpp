// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002bf1c8 (104 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 1
//   +24  halfword 0
//   +26  halfword 0
//   +28  halfword 0
//   +36  float 0.0f (bits 0x00000000)
//   +40  float 0.0f (bits 0x00000000)
//   +44  float 0.0f (bits 0x00000000)
//   +56  halfword 1
//   +58  halfword 1
//   +60  halfword 1
//   +66  halfword 2
//   +68  halfword 2
//   +70  halfword 3
//   +76  float 2.0f (bits 0x40000000)
//   +80  float 0.099999994f (bits 0x3dcccccc)
//   +84  float 0.049999997f (bits 0x3d4ccccc)
extern "C" void init_ramps(void*) asm("initRamps__15SkillPoisonRain");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 0;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 0;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 0;
    *reinterpret_cast<float*>(bytes + 36) = 0.0f;  // 0.0f
    *reinterpret_cast<float*>(bytes + 40) = 0.0f;  // 0.0f
    *reinterpret_cast<float*>(bytes + 44) = 0.0f;  // 0.0f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 58) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 60) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 68) = 2;
    *reinterpret_cast<unsigned short*>(bytes + 70) = 3;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.0000000000000p+1f;  // 2.0f
    *reinterpret_cast<float*>(bytes + 80) = 0x1.9999980000000p-4f;  // 0.099999994f
    *reinterpret_cast<float*>(bytes + 84) = 0x1.9999980000000p-5f;  // 0.049999997f
}
