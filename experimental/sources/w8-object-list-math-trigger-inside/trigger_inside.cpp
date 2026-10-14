// Observed objectInside__7TriggerP10GameObject at 0x1d1ae0, extent 80.
// Reads two floats from the object (byte offsets 16 and 20, called a and b here)
// and four from the trigger (offsets 76, 80, 84, 88: lower a, lower b, upper a,
// upper b). Returns 1 when
//   lower_a <= a  and  lower_b <= b  and  a <= upper_a  and  b <= upper_b
// with each ordered compare tested in that sequence, and 0 otherwise, so a NaN
// anywhere gives 0 (the positive form is written so that the EE compiler keeps c.le.s). Unknown: whether a and b are x and z or x and y; the names
// stay neutral.
struct TriggerBounds {
    unsigned char before[76];
    float lower_a, lower_b, upper_a, upper_b;
};

struct ObjectPlane {
    unsigned char before[16];
    float a, b;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(TriggerBounds, lower_a) == 76 && __builtin_offsetof(TriggerBounds, upper_b) == 88 &&
                  __builtin_offsetof(ObjectPlane, b) == 20, "observed offsets");
#endif

extern "C" int objectInside__7TriggerP10GameObject(const TriggerBounds* trigger, const ObjectPlane* object) {
    const float a = object->a;
    const float b = object->b;
    if (trigger->lower_a <= a && trigger->lower_b <= b && a <= trigger->upper_a && b <= trigger->upper_b) return 1;
    return 0;
}
