// Observed: padBigRumble__Fiff at 0x001682d0 is 164 bytes. It does nothing
// when the word padVibrationEnable (0x00484040) is zero. Otherwise the first
// float argument is multiplied by 60.0 and truncated to an integer, and the
// record is rumphole (0x006e1c60) plus 24 times the signed byte
// padIndexMap[index] (0x00484078). If the integer at record+4 is signed-less
// than the truncated value, the second float argument is clamped to the range
// 0.0 through 1.0 and stored as a float at record+8, the truncated value is
// stored at record+4, and record+12 receives the clamped float divided by the
// truncated value converted back to float. Otherwise nothing is written.
// Inferred: the first float is a duration in seconds (60 ticks per second)
// and the second a strength; record+12 is a per-tick decay of that strength.
// Unknown: who reads the record, and what happens for a zero truncated value
// (the division is executed in float and not guarded).
extern "C" {
extern int padVibrationEnable;
extern signed char padIndexMap[4];
extern unsigned rumphole[24];

static void store_float(unsigned* slot, float value) {
    __builtin_memcpy(slot, &value, sizeof value);
}

void padBigRumble__Fiff(int index, float first, float second) {
    if (padVibrationEnable == 0) return;
    const int ticks = static_cast<int>(first * 60.0f);
    unsigned* const record = rumphole + padIndexMap[index] * 6;
    if (static_cast<int>(record[1]) < ticks) {
        if (1.0f < second) second = 1.0f;
        if (second < 0.0f) second = 0.0f;
        store_float(record + 2, second);
        record[1] = static_cast<unsigned>(ticks);
        store_float(record + 3, second / static_cast<float>(ticks));
    }
}
}
