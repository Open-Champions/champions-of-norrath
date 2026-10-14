// Observed creatureAdvanceAnimation__8Creaturef at 0x00111830, 420 bytes. The
// object is a Creature; the float argument is a time step. Offsets are from the
// object address.
// 1. When the word at +236 is nonzero, the animation state at +288 is advanced by
//    animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps and its result
//    is kept as the return value (otherwise zero). Its arguments are the state,
//    the step, the address of +16 (null when bit 0x02000000 of the doubleword at
//    +160 is set), the address of +180 and the address of +188. The step is
//    multiplied first by float(60 - b) and then by 0x3c888888 (about 1/60) when
//    the unsigned byte b at +800 is in 1..60, where b is read as signed for the
//    subtraction.
// 2. The two floats at +180 and +184 are multiplied by the float at +224.
// 3. With a = the halfword at +56 minus 16384, c = icos__Fi(a) and s = isin__Fi(a)
//    and the floats x = [+180], y = [+184]: [+180] = (-x) * c + y * s and
//    [+184] = (-x) * s - y * c.
// 4. When b is in 1..60: k = float(60 - b) * 0x3c888888; [+180] and [+184] are each
//    multiplied by k, and the halfword at +188 becomes the truncation of
//    float(halfword) * k.
// Inferred: +180/+184 hold a planar motion and +188 a short companion value that
// the animation advance produces, and the byte at +800 is a slow-down count in
// frames; the image shows only the arithmetic.
// Unknown: the field meanings, and what the return value reports.
template <class T>
static inline T& field(void* object, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<char*>(object) + offset);
}

extern "C" {
int animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(void* state, float step, void* point3, void* point2,
                                                                   short* extra);
float icos__Fi(int angle);
float isin__Fi(int angle);

int creatureAdvanceAnimation__8Creaturef(void* self, float step) {
    const float sixtieth = __builtin_bit_cast(float, 0x3c888888u);
    int result = 0;
    if (field<int>(self, 236) != 0) {
        void* const position = (field<unsigned long long>(self, 160) & 0x02000000ull) == 0 ? static_cast<char*>(self) + 16 : nullptr;
        const signed char signed_count = field<signed char>(self, 800);
        float amount = step;
        if (static_cast<unsigned>(field<unsigned char>(self, 800) - 1) < 60u) {
            amount = step * static_cast<float>(60 - signed_count) * sixtieth;
        }
        result = animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(
            static_cast<char*>(self) + 288, amount, position, static_cast<char*>(self) + 180,
            &field<short>(self, 188));
    }
    const float scale = field<float>(self, 224);
    field<float>(self, 180) = field<float>(self, 180) * scale;
    field<float>(self, 184) = field<float>(self, 184) * scale;
    const int angle = field<short>(self, 56) - 16384;
    const float c = icos__Fi(angle);
    const float s = isin__Fi(field<short>(self, 56) - 16384);
    const float x = -field<float>(self, 180);
    const float y = field<float>(self, 184);
    field<float>(self, 180) = x * c + y * s;
    field<float>(self, 184) = x * s - y * c;
    if (static_cast<unsigned>(field<unsigned char>(self, 800) - 1) < 60u) {
        const float k = static_cast<float>(60 - field<signed char>(self, 800)) * sixtieth;
        const float old_x = field<float>(self, 180);
        const float old_y = field<float>(self, 184);
        const short extra = field<short>(self, 188);
        field<float>(self, 184) = old_y * k;
        field<float>(self, 180) = old_x * k;
        field<short>(self, 188) = static_cast<short>(static_cast<int>(static_cast<float>(extra) * k));
    }
    return result;
}
}
