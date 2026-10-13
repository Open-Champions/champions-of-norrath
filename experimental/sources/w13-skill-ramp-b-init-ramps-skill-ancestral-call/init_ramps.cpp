// Observed initRamps__18SkillAncestralCall at 0x002a8420, 108 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__18SkillAncestralCall");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<short*>(bytes + 24) = 125;
    *reinterpret_cast<float*>(bytes + 52) = __builtin_bit_cast(float, 0x408007a6u);  // about 4.00093365
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x409fffdcu);  // about 4.99998283
    *reinterpret_cast<short*>(bytes + 26) = 110;
    *reinterpret_cast<float*>(bytes + 40) = __builtin_bit_cast(float, 0x4000202du);  // about 2.00196385
    *reinterpret_cast<short*>(bytes + 28) = 20;
    *reinterpret_cast<float*>(bytes + 44) = __builtin_bit_cast(float, 0x3f804156u);  // about 1.00199389
    *reinterpret_cast<short*>(bytes + 30) = 4;
    *reinterpret_cast<float*>(bytes + 48) = __builtin_bit_cast(float, 0x3f01cad1u);  // about 0.507000983
    *reinterpret_cast<short*>(bytes + 22) = 1;
    *reinterpret_cast<short*>(bytes + 32) = 85;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 18) = 1;
    *reinterpret_cast<short*>(bytes + 20) = 1;
}
