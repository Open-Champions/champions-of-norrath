// Observed initRamps__17SkillRegeneration at 0x002b6b88, 28 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__17SkillRegeneration");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x40a00025u);  // about 5.00001764
    *reinterpret_cast<short*>(bytes + 24) = 10;
}
