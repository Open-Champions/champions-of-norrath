// Observed initRamps__16SkillPoisonArrow at 0x002a0bb0, 124 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__16SkillPoisonArrow");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<short*>(bytes + 24) = 40;
    *reinterpret_cast<float*>(bytes + 76) = __builtin_bit_cast(float, 0x3fffff92u);  // about 1.99998689
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x403fff64u);  // about 2.99996281
    *reinterpret_cast<short*>(bytes + 26) = 366;
    *reinterpret_cast<float*>(bytes + 40) = __builtin_bit_cast(float, 0x41f5e8d4u);  // about 30.7386856
    *reinterpret_cast<float*>(bytes + 44) = __builtin_bit_cast(float, 0x3f19930bu);  // about 0.599899948
    *reinterpret_cast<short*>(bytes + 30) = 3;
    *reinterpret_cast<float*>(bytes + 48) = __builtin_bit_cast(float, 0x3e808355u);  // about 0.251001984
    *reinterpret_cast<short*>(bytes + 32) = 37;
    *reinterpret_cast<float*>(bytes + 52) = __builtin_bit_cast(float, 0x403fff9fu);  // about 2.99997687
    *reinterpret_cast<short*>(bytes + 56) = 1;
    *reinterpret_cast<short*>(bytes + 66) = 2;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 18) = 1;
    *reinterpret_cast<short*>(bytes + 28) = 1;
    *reinterpret_cast<short*>(bytes + 20) = 1;
    *reinterpret_cast<short*>(bytes + 22) = 1;
}
