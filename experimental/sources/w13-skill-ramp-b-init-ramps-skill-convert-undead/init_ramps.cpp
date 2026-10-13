// Observed initRamps__18SkillConvertUndead at 0x00299500, 88 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__18SkillConvertUndead");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x3e4ccd0fu);  // about 0.200000986
    *reinterpret_cast<float*>(bytes + 52) = __builtin_bit_cast(float, 0x409ffeb2u);  // about 4.99984074
    *reinterpret_cast<short*>(bytes + 26) = 15;
    *reinterpret_cast<float*>(bytes + 40) = __builtin_bit_cast(float, 0x3f8020a3u);  // about 1.00099599
    *reinterpret_cast<short*>(bytes + 28) = 40;
    *reinterpret_cast<float*>(bytes + 44) = 2.0f;
    *reinterpret_cast<short*>(bytes + 22) = 1;
    *reinterpret_cast<short*>(bytes + 32) = 104;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<short*>(bytes + 24) = 1;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 18) = 1;
}
