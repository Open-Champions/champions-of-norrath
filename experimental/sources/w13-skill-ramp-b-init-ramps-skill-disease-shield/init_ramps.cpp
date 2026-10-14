// Observed initRamps__18SkillDiseaseShield at 0x00296790, 116 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__18SkillDiseaseShield");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<short*>(bytes + 24) = 20;
    *reinterpret_cast<float*>(bytes + 84) = __builtin_bit_cast(float, 0x3dccccccu);  // about 0.099999994
    *reinterpret_cast<short*>(bytes + 26) = 2;
    *reinterpret_cast<float*>(bytes + 40) = __builtin_bit_cast(float, 0x3e80010cu);  // about 0.250007987
    *reinterpret_cast<float*>(bytes + 44) = __builtin_bit_cast(float, 0x3e7fffbcu);  // about 0.249998987
    *reinterpret_cast<float*>(bytes + 76) = 2.0f;
    *reinterpret_cast<short*>(bytes + 68) = 5;
    *reinterpret_cast<float*>(bytes + 80) = __builtin_bit_cast(float, 0x3d4cccccu);  // about 0.049999997
    *reinterpret_cast<short*>(bytes + 70) = 1;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<float*>(bytes + 36) = 2.0f;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 18) = 1;
    *reinterpret_cast<short*>(bytes + 28) = 5;
    *reinterpret_cast<short*>(bytes + 56) = 1;
    *reinterpret_cast<short*>(bytes + 66) = 1;
    *reinterpret_cast<short*>(bytes + 58) = 1;
    *reinterpret_cast<short*>(bytes + 60) = 1;
}
