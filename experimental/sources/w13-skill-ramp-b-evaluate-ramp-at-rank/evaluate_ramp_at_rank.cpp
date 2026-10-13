// Observed evaluateRampAtRank__5Skillii at 0x00313548, 156 bytes (symbol size).
// The first integer argument (here "slot") selects one of five entries and the second ("level") is
// the point count to evaluate. Reads only, from the receiver and its arguments:
//  - level 0 returns 0;
//  - a threshold halfword at +14+2*slot (signed); level below it returns 0;
//  - otherwise the value is  float(halfword at +24+2*slot) + float(word at +36+4*slot) * float(level - threshold)
//    and the result is that value rounded half away from zero to an integer: +0.5 then truncation when
//    the value is >= 0.0f, -0.5 then truncation otherwise.
// Nothing is written. The slot is not range checked; the layout of the three runs is only known by offset.
// Inferred: threshold / base / step per slot, as filled in by the initRamps functions of the classes.
// Unknown: the member names; behaviour for a slot outside 0..4.
extern "C" int evaluate_ramp_at_rank(const void*, int, int) asm("evaluateRampAtRank__5Skillii");
extern "C" int evaluate_ramp_at_rank(const void* self, int slot, int level) {
    const unsigned char* bytes = static_cast<const unsigned char*>(self);
    if (level == 0) return 0;
    const int threshold = *reinterpret_cast<const short*>(bytes + 14 + 2 * slot);
    if (level < threshold) return 0;
    const float base = static_cast<float>(*reinterpret_cast<const short*>(bytes + 24 + 2 * slot));
    const float step = *reinterpret_cast<const float*>(bytes + 36 + 4 * slot);
    const float value = base + step * static_cast<float>(level - threshold);
    if (value >= 0.0f) return static_cast<int>(value + 0.5f);
    return static_cast<int>(value - 0.5f);
}
