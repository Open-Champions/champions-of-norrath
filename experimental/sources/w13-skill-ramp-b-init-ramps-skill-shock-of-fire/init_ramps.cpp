// Observed initRamps__16SkillShockOfFire at 0x002b31a0, 184 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__16SkillShockOfFire");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<float*>(bytes + 88) = 1.0f;
    *reinterpret_cast<short*>(bytes + 24) = 162;
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x40fffe82u);  // about 7.99981785
    *reinterpret_cast<short*>(bytes + 26) = 45;
    *reinterpret_cast<float*>(bytes + 40) = __builtin_bit_cast(float, 0x4000201cu);  // about 2.0019598
    *reinterpret_cast<float*>(bytes + 44) = __builtin_bit_cast(float, 0x3fa665adu);  // about 1.2999779
    *reinterpret_cast<float*>(bytes + 48) = __builtin_bit_cast(float, 0x40bc51b2u);  // about 5.88497257
    *reinterpret_cast<short*>(bytes + 32) = 81;
    *reinterpret_cast<float*>(bytes + 52) = __builtin_bit_cast(float, 0x40a01722u);  // about 5.00282383
    *reinterpret_cast<short*>(bytes + 66) = 3;
    *reinterpret_cast<float*>(bytes + 76) = __builtin_bit_cast(float, 0x403fffc9u);  // about 2.99998689
    *reinterpret_cast<short*>(bytes + 68) = 5;
    *reinterpret_cast<short*>(bytes + 60) = 10;
    *reinterpret_cast<short*>(bytes + 70) = 2;
    *reinterpret_cast<float*>(bytes + 84) = __builtin_bit_cast(float, 0x3dccccccu);  // about 0.099999994
    *reinterpret_cast<short*>(bytes + 62) = 1;
    *reinterpret_cast<short*>(bytes + 72) = 243;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<short*>(bytes + 16) = 5;
    *reinterpret_cast<short*>(bytes + 18) = 5;
    *reinterpret_cast<short*>(bytes + 28) = 2;
    *reinterpret_cast<short*>(bytes + 20) = 1;
    *reinterpret_cast<short*>(bytes + 30) = 243;
    *reinterpret_cast<short*>(bytes + 22) = 1;
    *reinterpret_cast<short*>(bytes + 56) = 1;
    *reinterpret_cast<short*>(bytes + 58) = 5;
    *reinterpret_cast<float*>(bytes + 80) = 1.0f;
}
