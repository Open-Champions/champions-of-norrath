// Observed initRamps__17SkillDiseaseTrail at 0x00295458, 52 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__17SkillDiseaseTrail");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<short*>(bytes + 24) = 10;
    *reinterpret_cast<float*>(bytes + 40) = __builtin_bit_cast(float, 0x3eaa7ef9u);  // about 0.332999974
    *reinterpret_cast<float*>(bytes + 36) = 2.0f;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 26) = 3;
    *reinterpret_cast<short*>(bytes + 14) = 1;
}
