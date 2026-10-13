// Observed getEngineTorque__7Vehicle at 0x00278120, 180 bytes. Returns a
// float. Reads the word at +1024: when it is non-zero the result is 0.0.
// Otherwise, with r = float at +748, t = float at +740, and `table` the float
// array that starts 8 bytes after the pointer stored at +2052:
//   position = (int)(r * K), K being the float word 0x3a83126e (0.001);
//   lo = position, or 9 when position >= 10;
//   hi = position + 1, or 9 when position + 1 >= 10;
//   blend = table[lo] + (table[hi] - table[lo]) * ((r - (float)(lo * 1000)) * K);
//   if t == 0.0 return blend * t - r * (float at +916), otherwise return blend * t.
// There is no lower clamp: a negative position indexes below the table. The
// table looks like torque samples at 1000-unit steps of the value at +748 and
// the +740 factor like a throttle, but those are inferred, not established.
// Nothing here was executed on the EE.
typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef AliasedFloat* AliasedFloatPointer __attribute__((may_alias));

extern "C" float getEngineTorque__7Vehicle(unsigned char* vehicle) {
    if (*reinterpret_cast<AliasedInt*>(vehicle + 1024) != 0) return 0.0f;
    const float k = 0x1.0624dcp-10f;
    const float r = *reinterpret_cast<AliasedFloat*>(vehicle + 748);
    const float t = *reinterpret_cast<AliasedFloat*>(vehicle + 740);
    const AliasedFloat* const table = *reinterpret_cast<AliasedFloatPointer*>(vehicle + 2052) + 2;
    const int position = static_cast<int>(r * k);
    const int lo = position < 10 ? position : 9;
    const int next = position + 1;
    const int hi = next < 10 ? next : 9;
    const float base = table[lo];
    float torque = base + (table[hi] - base) * ((r - static_cast<float>(lo * 1000)) * k);
    torque = torque * t;
    if (t == 0.0f) {
        torque = torque - r * *reinterpret_cast<AliasedFloat*>(vehicle + 916);
    }
    return torque;
}
