// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x002b9398 (88 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +18  halfword 5
//   +20  halfword 5
//   +24  halfword 38
//   +26  halfword 6
//   +28  halfword 1
//   +30  halfword 4
//   +36  float 2.0f (bits 0x40000000)
//   +40  float 0.33299997f (bits 0x3eaa7ef9)
//   +44  float 0.33299997f (bits 0x3eaa7ef9)
//   +48  float 0.19999999f (bits 0x3e4ccccc)
extern "C" void init_ramps(void*) asm("initRamps__15SkillWizardBeam");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 18) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 20) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 38;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 6;
    *reinterpret_cast<unsigned short*>(bytes + 28) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 30) = 4;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.0000000000000p+1f;  // 2.0f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.54fdf20000000p-2f;  // 0.33299997f
    *reinterpret_cast<float*>(bytes + 44) = 0x1.54fdf20000000p-2f;  // 0.33299997f
    *reinterpret_cast<float*>(bytes + 48) = 0x1.9999980000000p-3f;  // 0.19999999f
}
