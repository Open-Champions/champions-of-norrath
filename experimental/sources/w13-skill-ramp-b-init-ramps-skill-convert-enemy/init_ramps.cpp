// Observed initRamps__17SkillConvertEnemy at 0x00298aa8, 88 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__17SkillConvertEnemy");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x3e4cccccu);  // about 0.199999988
    *reinterpret_cast<float*>(bytes + 52) = __builtin_bit_cast(float, 0x40dffe7cu);  // about 6.99981499
    *reinterpret_cast<short*>(bytes + 26) = 15;
    *reinterpret_cast<float*>(bytes + 40) = 1.0f;
    *reinterpret_cast<short*>(bytes + 28) = 40;
    *reinterpret_cast<float*>(bytes + 44) = __builtin_bit_cast(float, 0x3fffff92u);  // about 1.99998689
    *reinterpret_cast<short*>(bytes + 22) = 1;
    *reinterpret_cast<short*>(bytes + 32) = 97;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<short*>(bytes + 24) = 1;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 18) = 1;
}
