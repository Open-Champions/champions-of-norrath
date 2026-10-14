// Observed initRamps__18SkillRepulseUndead at 0x002ba630, 56 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__18SkillRepulseUndead");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<short*>(bytes + 24) = 200;
    *reinterpret_cast<float*>(bytes + 40) = 0.5f;
    *reinterpret_cast<float*>(bytes + 36) = 10.0f;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 26) = 6;
    *reinterpret_cast<short*>(bytes + 14) = 1;
}
