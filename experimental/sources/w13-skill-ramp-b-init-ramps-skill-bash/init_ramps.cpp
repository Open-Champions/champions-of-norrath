// Observed initRamps__9SkillBash at 0x002a5d58, 108 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__9SkillBash");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<short*>(bytes + 24) = 50;
    *reinterpret_cast<float*>(bytes + 52) = __builtin_bit_cast(float, 0x403fffbcu);  // about 2.99998379
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x411fffb4u);  // about 9.99992752
    *reinterpret_cast<short*>(bytes + 26) = 20;
    *reinterpret_cast<float*>(bytes + 40) = __builtin_bit_cast(float, 0x3fffff15u);  // about 1.99997199
    *reinterpret_cast<short*>(bytes + 28) = 4;
    *reinterpret_cast<float*>(bytes + 44) = __builtin_bit_cast(float, 0x3f000021u);  // about 0.500001967
    *reinterpret_cast<float*>(bytes + 48) = 0.25f;
    *reinterpret_cast<short*>(bytes + 22) = 1;
    *reinterpret_cast<short*>(bytes + 32) = 16;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 18) = 1;
    *reinterpret_cast<short*>(bytes + 20) = 1;
    *reinterpret_cast<short*>(bytes + 30) = 1;
}
