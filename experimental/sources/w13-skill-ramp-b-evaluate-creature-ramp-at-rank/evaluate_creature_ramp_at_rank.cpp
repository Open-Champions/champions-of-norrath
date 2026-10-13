// Observed evaluateCreatureRampAtRank__5Skillii at 0x003135e8, 156 bytes (symbol size).
// Same computation as evaluateRampAtRank__5Skillii at 0x00313548 over a second set of three runs:
//  - level 0 returns 0;
//  - a threshold halfword at +56+2*slot (signed); level below it returns 0;
//  - otherwise  float(halfword at +66+2*slot) + float(word at +76+4*slot) * float(level - threshold),
//    rounded half away from zero to an integer (+0.5 then truncation when >= 0.0f, -0.5 otherwise).
// Nothing is written. The slot is not range checked.
// Inferred: a second threshold / base / step set kept beside the first. Unknown: what it is used for
// (the name of the method is the only evidence) and behaviour for a slot outside 0..4.
extern "C" int evaluate_creature_ramp_at_rank(const void*, int, int) asm("evaluateCreatureRampAtRank__5Skillii");
extern "C" int evaluate_creature_ramp_at_rank(const void* self, int slot, int level) {
    const unsigned char* bytes = static_cast<const unsigned char*>(self);
    if (level == 0) return 0;
    const int threshold = *reinterpret_cast<const short*>(bytes + 56 + 2 * slot);
    if (level < threshold) return 0;
    const float base = static_cast<float>(*reinterpret_cast<const short*>(bytes + 66 + 2 * slot));
    const float step = *reinterpret_cast<const float*>(bytes + 76 + 4 * slot);
    const float value = base + step * static_cast<float>(level - threshold);
    if (value >= 0.0f) return static_cast<int>(value + 0.5f);
    return static_cast<int>(value - 0.5f);
}
