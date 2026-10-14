// Observed initRamps__16SkillFireWeapons at 0x002ab3c0, 112 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__16SkillFireWeapons");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<float*>(bytes + 76) = __builtin_bit_cast(float, 0x4000004fu);  // about 2.00001884
    *reinterpret_cast<short*>(bytes + 24) = 21;
    *reinterpret_cast<float*>(bytes + 40) = 2.0f;
    *reinterpret_cast<short*>(bytes + 28) = 2;
    *reinterpret_cast<float*>(bytes + 44) = __builtin_bit_cast(float, 0x3eaa7ef9u);  // about 0.332999974
    *reinterpret_cast<short*>(bytes + 20) = 6;
    *reinterpret_cast<short*>(bytes + 30) = 15;
    *reinterpret_cast<float*>(bytes + 48) = 1.0f;
    *reinterpret_cast<short*>(bytes + 56) = 1;
    *reinterpret_cast<short*>(bytes + 66) = 4;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<float*>(bytes + 36) = 2.0f;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 26) = 15;
    *reinterpret_cast<short*>(bytes + 18) = 6;
}
