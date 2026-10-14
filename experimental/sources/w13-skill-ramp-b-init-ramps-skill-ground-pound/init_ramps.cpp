// Observed initRamps__16SkillGroundPound at 0x002908b8, 112 bytes. Stores constants through the receiver only, in the order below;
// nothing is read, called or returned. Observed offsets, from the evaluators evaluateRampAtRank__5Skillii and
// evaluateCreatureRampAtRank__5Skillii: five halfword thresholds at +14 and +56, five halfword bases at +24 and +66, and five
// floats at +36 and +76 (steps). A float that is not a short exact value is written as its observed 32-bit pattern.
// Unknown: the member names, and the game meaning of the numbers.
extern "C" void init_ramps(void*) asm("initRamps__16SkillGroundPound");
extern "C" void init_ramps(void* object) {
    auto* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<short*>(bytes + 24) = 70;
    *reinterpret_cast<float*>(bytes + 52) = __builtin_bit_cast(float, 0x403fff6du);  // about 2.99996495
    *reinterpret_cast<float*>(bytes + 36) = __builtin_bit_cast(float, 0x411fff6fu);  // about 9.99986172
    *reinterpret_cast<short*>(bytes + 26) = 6;
    *reinterpret_cast<float*>(bytes + 40) = __builtin_bit_cast(float, 0x3f7ffec1u);  // about 0.999980986
    *reinterpret_cast<float*>(bytes + 44) = __builtin_bit_cast(float, 0x403fffb4u);  // about 2.99998188
    *reinterpret_cast<short*>(bytes + 20) = 10;
    *reinterpret_cast<short*>(bytes + 30) = 2;
    *reinterpret_cast<float*>(bytes + 48) = 0.25f;
    *reinterpret_cast<short*>(bytes + 22) = 1;
    *reinterpret_cast<short*>(bytes + 32) = 28;
    *reinterpret_cast<short*>(bytes + 14) = 1;
    *reinterpret_cast<short*>(bytes + 16) = 1;
    *reinterpret_cast<short*>(bytes + 18) = 10;
    *reinterpret_cast<short*>(bytes + 28) = 10;
}
