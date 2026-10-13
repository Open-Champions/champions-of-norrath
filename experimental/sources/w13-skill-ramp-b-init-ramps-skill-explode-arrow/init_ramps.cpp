// Observed initRamps__17SkillExplodeArrow at 0x002a2678, 164 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__17SkillExplodeArrow");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<float*>(bytes + 84) = 1.0f;
    *reinterpret_cast<short*>(bytes + 24) = 125;
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x409fffedu);  // about 4.99999094
    *reinterpret_cast<short*>(bytes + 26) = 417;
    *reinterpret_cast<float*>(bytes + 40) = __builtin_bit_cast(float, 0x414d8cd5u);  // about 12.8468828
    *reinterpret_cast<short*>(bytes + 18) = 5;
    *reinterpret_cast<short*>(bytes + 28) = 50;
    *reinterpret_cast<float*>(bytes + 44) = __builtin_bit_cast(float, 0x403ffd08u);  // about 2.9998188
    *reinterpret_cast<short*>(bytes + 32) = 68;
    *reinterpret_cast<float*>(bytes + 52) = __builtin_bit_cast(float, 0x403fff75u);  // about 2.99996686
    *reinterpret_cast<short*>(bytes + 66) = 2;
    *reinterpret_cast<float*>(bytes + 76) = 2.0f;
    *reinterpret_cast<short*>(bytes + 68) = 400;
    *reinterpret_cast<float*>(bytes + 80) = 5.0f;
    *reinterpret_cast<short*>(bytes + 60) = 1;
    *reinterpret_cast<short*>(bytes + 70) = 45;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 22) = 1;
    *reinterpret_cast<short*>(bytes + 56) = 1;
    *reinterpret_cast<short*>(bytes + 58) = 1;
}
