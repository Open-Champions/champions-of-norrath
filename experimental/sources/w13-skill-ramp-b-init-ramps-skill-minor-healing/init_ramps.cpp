// Observed initRamps__17SkillMinorHealing at 0x002a72f0, 48 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__17SkillMinorHealing");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<short*>(bytes + 24) = 15;
    *reinterpret_cast<float*>(bytes + 52) = __builtin_bit_cast(float, 0x40fffe6du);  // about 7.99980783
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x403ffeefu);  // about 2.99993491
    *reinterpret_cast<short*>(bytes + 22) = 1;
    *reinterpret_cast<short*>(bytes + 32) = 173;
    *reinterpret_cast<short*>(bytes + 14) = 1;
}
