// Observed setMassAndIBody__7Vehicle at 0x00277d60, 176 bytes. With
//   m = float at +884, and x, y, z = floats at +888, +892, +896 each scaled by
//   K1 (the float word 0x3cd20d20) and squared:
//   +668 = m, +672 = 1 / m,
//   +1216 = (x2 + y2) * m * K2, +1220 = (x2 + z2) * m * K2,
//   +1212 = (z2 + y2) * m * K2 (K2 is the float word 0x3daaaaaa),
//   and the reciprocals of those three go to +1228, +1232 and +1224.
// Only the arithmetic and the stores are established here; what the fields
// mean (mass, inertia terms) is not. Nothing here was executed on the EE.
typedef float AliasedFloat __attribute__((may_alias));

extern "C" void setMassAndIBody__7Vehicle(unsigned char* vehicle) {
    AliasedFloat* const f = reinterpret_cast<AliasedFloat*>(vehicle);
    const float k1 = 0x1.a41a40p-6f;
    const float k2 = 0x1.555554p-4f;
    const float m = f[884 / 4];
    const float x = f[888 / 4] * k1;
    const float y = f[892 / 4] * k1;
    const float z = f[896 / 4] * k1;
    f[668 / 4] = m;
    const float x2 = x * x;
    const float y2 = y * y;
    const float z2 = z * z;
    f[672 / 4] = 1.0f / m;
    const float a = m * (x2 + y2) * k2;
    const float b = m * (x2 + z2) * k2;
    const float c = m * (z2 + y2) * k2;
    f[1216 / 4] = a;
    f[1220 / 4] = b;
    f[1212 / 4] = c;
    f[1228 / 4] = 1.0f / a;
    f[1232 / 4] = 1.0f / b;
    f[1224 / 4] = 1.0f / c;
}
