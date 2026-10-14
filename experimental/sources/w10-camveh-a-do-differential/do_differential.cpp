// Observed doDifferential__7Vehicle at 0x00276118, 324 bytes. Nothing is
// returned. Let r be the float at vehicle+748, n the signed word at +1000 and
// w_i the wheel pointers at +1052 + 4 * i.
//   1. r0 = r. When r0 < 0 the float at +748 is set to 0.0. ratio = 1.0 when
//      r0 == 0.0, otherwise (float at +748 now) / r0.
//   2. For i in 0 .. n - 1 (nothing when n <= 0):
//      SetNaturalRPM__5Wheel(w_i);
//      ModifyRPM__5Wheelf(w_i, (float at w_i + 364) * ratio);
//      when the word at w_i + 388 is non-zero: s = r / (spec[0] / float(+912))
//      with spec the pointer at +2052 and r re-read from +748; if the word at
//      +2152 is non-zero s is negated, otherwise a negative s becomes 0.0; then
//      ModifyRPM__5Wheelf(w_i, s * (1.0 - float(+744))).
// A negative r0 gives a ratio of (0.0 / r0). Which of these floats are engine
// speed or gearing is not established; ModifyRPM and SetNaturalRPM are named in
// the original image only. Nothing here was executed on the EE.
typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));

extern "C" {
void SetNaturalRPM__5Wheel(unsigned char* wheel);
void ModifyRPM__5Wheelf(unsigned char* wheel, float value);
}

namespace {
inline unsigned char* wheel_at(unsigned char* vehicle, int i) {
    return *reinterpret_cast<AliasedPointer*>(vehicle + 1052 + 4 * i);
}
inline float real(unsigned char* base, int offset) { return *reinterpret_cast<AliasedFloat*>(base + offset); }
inline int word(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }
}

extern "C" void doDifferential__7Vehicle(unsigned char* vehicle) {
    AliasedFloat& rpm = *reinterpret_cast<AliasedFloat*>(vehicle + 748);
    const float old_rpm = rpm;
    if (old_rpm < 0.0f) rpm = 0.0f;
    float ratio;
    if (old_rpm == 0.0f) {
        ratio = 1.0f;
    } else {
        ratio = rpm / old_rpm;
    }
    for (int i = 0; i < word(vehicle, 1000); ++i) {
        SetNaturalRPM__5Wheel(wheel_at(vehicle, i));
        ModifyRPM__5Wheelf(wheel_at(vehicle, i), real(wheel_at(vehicle, i), 364) * ratio);
        if (word(wheel_at(vehicle, i), 388) != 0) {
            const AliasedFloat* const spec = *reinterpret_cast<AliasedFloat* const*>(vehicle + 2052);
            float s = rpm / (spec[0] / real(vehicle, 912));
            if (word(vehicle, 2152) != 0) {
                s = -s;
            } else if (s < 0.0f) {
                s = 0.0f;
            }
            ModifyRPM__5Wheelf(wheel_at(vehicle, i), s * (1.0f - real(vehicle, 744)));
        }
    }
}
