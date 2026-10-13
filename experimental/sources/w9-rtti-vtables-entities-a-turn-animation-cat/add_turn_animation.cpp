// Observed addTurnAnimation__3Cat at 0x001eee68, 180 bytes. A damped follower:
//   target = (float)(signed halfword at +284) * 2^-9, clamped to [-1, 1];
//   pos = float at +980, vel = float at +984;
//   vel += (target - pos) * 0.09; pos += vel;
//   then the float at +980 becomes pos, the float at +984 becomes vel - vel * 0.6
//   (just under 0.6), and the int at +976 becomes (int)(pos * 0.909091 * 2177.0)
//   (pos * 0.909091 is out). The pair at +368 and +400 receives
//   out > 0 ? (out, 0) : (0, -out).
// Constants are the singles at 0x480cb4, 0x480cb8, 0x480cbc and 0x480cc0.
// The order of the float operations is kept as observed, since single-precision
// rounding depends on it.
// Unknown: what the fields are called and what the int at +976 drives.
extern "C" {
extern const unsigned norrath_rtti_entities_a_view_480cb4;
extern const unsigned norrath_rtti_entities_a_view_480cb8;
extern const unsigned norrath_rtti_entities_a_view_480cbc;
extern const unsigned norrath_rtti_entities_a_view_480cc0;
}

namespace {
typedef float FloatWord __attribute__((may_alias));
typedef int IntWord __attribute__((may_alias));
typedef short Half __attribute__((may_alias));
float as_single(unsigned bits) {
    union { unsigned bits; float value; } u = {bits};
    return u.value;
}
}

extern "C" void addTurnAnimation__3Cat(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    float target = static_cast<float>(*reinterpret_cast<const Half*>(object + 284)) * 0.001953125f;
    if (1.0f < target) target = 1.0f;
    if (target < -1.0f) target = -1.0f;
    const float position = *reinterpret_cast<const FloatWord*>(object + 980);
    const float rate = *reinterpret_cast<const FloatWord*>(object + 984);
    const float pull = (target - position) * as_single(norrath_rtti_entities_a_view_480cb4);
    const float new_rate = rate + pull;
    const float new_position = position + new_rate;
    const float damped = new_rate * as_single(norrath_rtti_entities_a_view_480cb8);
    const float out = new_position * as_single(norrath_rtti_entities_a_view_480cbc);
    *reinterpret_cast<FloatWord*>(object + 980) = new_position;
    const float scaled = out * as_single(norrath_rtti_entities_a_view_480cc0);
    *reinterpret_cast<FloatWord*>(object + 984) = new_rate - damped;
    *reinterpret_cast<IntWord*>(object + 976) = static_cast<int>(scaled);
    if (0.0f < out) {
        *reinterpret_cast<FloatWord*>(object + 400) = 0.0f;
        *reinterpret_cast<FloatWord*>(object + 368) = out;
    } else {
        *reinterpret_cast<FloatWord*>(object + 368) = 0.0f;
        *reinterpret_cast<FloatWord*>(object + 400) = -out;
    }
}
