// Observed initRamps__17SkillShockOfFrost at 0x002be038, 140 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__17SkillShockOfFrost");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<short*>(bytes + 24) = 30;
    *reinterpret_cast<float*>(bytes + 84) = 2.0f;
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x40bfffd3u);  // about 5.99997854
    *reinterpret_cast<float*>(bytes + 40) = 0.25f;
    *reinterpret_cast<short*>(bytes + 18) = 5;
    *reinterpret_cast<short*>(bytes + 28) = 27;
    *reinterpret_cast<float*>(bytes + 44) = 4.0f;
    *reinterpret_cast<short*>(bytes + 66) = 3;
    *reinterpret_cast<float*>(bytes + 76) = __builtin_bit_cast(float, 0x4040002eu);  // about 3.00001097
    *reinterpret_cast<short*>(bytes + 68) = 1;
    *reinterpret_cast<float*>(bytes + 80) = __builtin_bit_cast(float, 0x3dccccccu);  // about 0.099999994
    *reinterpret_cast<short*>(bytes + 60) = 10;
    *reinterpret_cast<short*>(bytes + 70) = 14;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<short*>(bytes + 16) = 5;
    *reinterpret_cast<short*>(bytes + 26) = 3;
    *reinterpret_cast<short*>(bytes + 56) = 1;
    *reinterpret_cast<short*>(bytes + 58) = 10;
}
