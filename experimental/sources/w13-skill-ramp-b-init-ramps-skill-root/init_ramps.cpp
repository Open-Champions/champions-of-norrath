// Observed initRamps__9SkillRoot at 0x002aceb8, 84 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__9SkillRoot");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x3eaa7cbfu);  // about 0.332982987
    *reinterpret_cast<float*>(bytes + 52) = __builtin_bit_cast(float, 0x40800017u);  // about 4.00001097
    *reinterpret_cast<short*>(bytes + 26) = 5;
    *reinterpret_cast<float*>(bytes + 40) = __builtin_bit_cast(float, 0x3eaa7ef9u);  // about 0.332999974
    *reinterpret_cast<short*>(bytes + 28) = 50;
    *reinterpret_cast<float*>(bytes + 44) = __builtin_bit_cast(float, 0x4000103cu);  // about 2.00099087
    *reinterpret_cast<short*>(bytes + 22) = 1;
    *reinterpret_cast<short*>(bytes + 32) = 88;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<short*>(bytes + 24) = 1;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 18) = 1;
}
