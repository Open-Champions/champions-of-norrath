// Observed lerpShortRot__Fssfff at 0x0015a610, 168 bytes.
// Arguments: two signed halfwords (from, to) and three floats (rate, lo, hi).
// The difference from - to is wrapped to a signed halfword. Its magnitude,
// converted to float and multiplied by rate, is the step. A lo other than -1.0
// raises a step below lo to lo; a hi other than -1.0 lowers a step above hi to
// hi (-1.0 means "no limit" on that side). The step is truncated to an int and
// subtracted from `from` when the wrapped difference is positive, added
// otherwise; the result is wrapped to a signed halfword. It moves `from`
// toward `to` without wrapping around the circle. The unit (angle) is inferred
// from the name only. A step larger than the distance overshoots `to`; the
// original does not clamp it either.
extern "C" short lerpShortRot__Fssfff(short from, short to, float rate, float lo, float hi) {
    short diff = static_cast<short>(from - to);
    int magnitude = diff < 0 ? -diff : diff;
    int sign = diff > 0 ? -1 : 1;
    float step = static_cast<float>(magnitude) * rate;
    if (lo != -1.0f && step < lo) step = lo;
    if (hi != -1.0f && hi < step) step = hi;
    return static_cast<short>(sign * static_cast<int>(step) + from);
}
