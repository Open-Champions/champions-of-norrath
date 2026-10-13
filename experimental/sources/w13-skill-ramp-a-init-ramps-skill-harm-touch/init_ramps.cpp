// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x0029aed8 (100 bytes): straight-line stores to the
// receiver, no calls and no reads of the receiver. Each line below is one store (byte offset,
// width, value). Member names and the rest of the class layout are not inferred.
//   +14  halfword 1
//   +16  halfword 1
//   +22  halfword 1
//   +24  halfword 420
//   +26  halfword 5
//   +32  halfword 210
//   +36  float 11.999881f (bits 0x413fff83)
//   +40  float 0.33299997f (bits 0x3eaa7ef9)
//   +52  float 7.9998007f (bits 0x40fffe5e)
//   +56  halfword 1
//   +58  halfword 1
//   +66  halfword 5
//   +68  halfword 4
//   +76  float 5.0000057f (bits 0x40a0000c)
//   +80  float 0.0f (bits 0x00000000)
extern "C" void init_ramps(void*) asm("initRamps__14SkillHarmTouch");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned short*>(bytes + 14) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 16) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 22) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 24) = 420;
    *reinterpret_cast<unsigned short*>(bytes + 26) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 32) = 210;
    *reinterpret_cast<float*>(bytes + 36) = 0x1.7fff060000000p+3f;  // 11.999881f
    *reinterpret_cast<float*>(bytes + 40) = 0x1.54fdf20000000p-2f;  // 0.33299997f
    *reinterpret_cast<float*>(bytes + 52) = 0x1.fffcbc0000000p+2f;  // 7.9998007f
    *reinterpret_cast<unsigned short*>(bytes + 56) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 58) = 1;
    *reinterpret_cast<unsigned short*>(bytes + 66) = 5;
    *reinterpret_cast<unsigned short*>(bytes + 68) = 4;
    *reinterpret_cast<float*>(bytes + 76) = 0x1.4000180000000p+2f;  // 5.0000057f
    *reinterpret_cast<float*>(bytes + 80) = 0.0f;  // 0.0f
}
