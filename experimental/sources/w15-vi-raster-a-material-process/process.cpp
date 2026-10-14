// Observed: Process__10VIMateriall at 0x010ceb70 is 264 bytes in the pinned SLUS-20565 image. The second argument is a 64-bit time.
// The material keeps a 64-bit last-processed time at +24. If it equals the argument the routine returns 0 and changes nothing.
// Otherwise dt = (float)(int)(time - last) * the float with bit pattern 0x3a83126e (about 0.001, i.e. milliseconds to seconds), the
// difference being truncated to 32 bits first. The word at +8 is a layer count; layers are 144 bytes apart starting at the material
// itself. For each layer whose word at +48 is not negative, two scroll offsets advance: the float at +112 gets += (float at +104) * dt
// and the float at +116 gets += (float at +108) * dt. After each update, if the new value is greater than 1.0 or less than -1.0 it is
// replaced by value - (float)(int)value (the integer part removed, truncating toward zero). Finally +24 is set to the argument and 0
// is returned.
// Inferred: this advances texture-coordinate scroll offsets of each animated layer, kept within (-1, 1); +104/+108 are per-second
// scroll rates, +112/+116 the current offsets and +48 a layer index or enable (negative = unused).
// Unknown: what the layer count at +8 counts relative to other fields, the layer base (the first layer overlaps the material header
// by these offsets), and the time unit. Nothing here ran on the EE.
extern "C" {
int Process__10VIMateriall(unsigned char* self, long long now) {
    long long last = *reinterpret_cast<long long*>(self + 24);
    if (last == now) return 0;
    const float scale = __builtin_bit_cast(float, 0x3a83126eu);
    float dt = static_cast<float>(static_cast<int>(now - last)) * scale;
    int count = *reinterpret_cast<int*>(self + 8);
    unsigned char* layer = self;
    for (int i = 0; i < count; ++i, layer += 144) {
        if (*reinterpret_cast<int*>(layer + 48) < 0) continue;
        float* u = reinterpret_cast<float*>(layer + 112);
        float* v = reinterpret_cast<float*>(layer + 116);
        float value = *u + *reinterpret_cast<float*>(layer + 104) * dt;
        *u = value;
        if (value > 1.0f || value < -1.0f) *u = value - static_cast<float>(static_cast<int>(value));
        value = *v + *reinterpret_cast<float*>(layer + 108) * dt;
        *v = value;
        if (value > 1.0f || value < -1.0f) *v = value - static_cast<float>(static_cast<int>(value));
    }
    *reinterpret_cast<long long*>(self + 24) = now;
    return 0;
}
}
