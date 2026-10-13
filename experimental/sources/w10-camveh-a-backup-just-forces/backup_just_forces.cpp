// Observed backupJustForces__7Vehicle at 0x00272bb8, 260 bytes. Nothing is
// returned. If the word at vehicle+2164 is non-zero nothing happens. Otherwise,
// with F the Point3 at +2004, T the Point3 at +1992, vel the Point3 at +1188,
// w the Point3 at +1200, minv the float at +672, I the three floats at +1224,
// +1228, +1232 and K the float word 0x3c888888:
//   vel -= (F * minv) * K, componentwise;
//   the word at +2164 becomes 1;
//   w -= ((T.x * I[0]) * K, (T.y * I[1]) * K, (T.z * I[2]) * K);
//   F and T are set to zero (all six floats).
// setMassAndIBody__7Vehicle stores +672 as 1 / mass and +1224..+1232 as the
// reciprocals of three inertia-like terms, so this reads as one step of
// integrating the accumulated force and torque into the two velocities, and
// the K as one sixtieth of a second; both are inferences, not established.
// Nothing here was executed on the EE.
typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));

extern "C" void backupJustForces__7Vehicle(unsigned char* vehicle) {
    AliasedFloat* const f = reinterpret_cast<AliasedFloat*>(vehicle);
    AliasedInt* const w = reinterpret_cast<AliasedInt*>(vehicle);
    if (w[2164 / 4] != 0) return;
    const float k = 0x1.111110p-6f;
    const float minv = f[672 / 4];
    const float fx = f[2004 / 4] * minv;
    const float fy = f[2008 / 4] * minv;
    const float fz = f[2012 / 4] * minv;
    f[1192 / 4] = f[1192 / 4] - fy * k;
    f[1196 / 4] = f[1196 / 4] - fz * k;
    w[2164 / 4] = 1;
    f[1188 / 4] = f[1188 / 4] - fx * k;
    f[2012 / 4] = 0.0f;
    const float tx = f[1992 / 4] * f[1224 / 4];
    const float ty = f[1996 / 4] * f[1228 / 4];
    const float tz = f[2000 / 4] * f[1232 / 4];
    f[2000 / 4] = 0.0f;
    f[1996 / 4] = 0.0f;
    f[1992 / 4] = 0.0f;
    f[1200 / 4] = f[1200 / 4] - tx * k;
    f[1204 / 4] = f[1204 / 4] - ty * k;
    f[1208 / 4] = f[1208 / 4] - tz * k;
    f[2008 / 4] = 0.0f;
    f[2004 / 4] = 0.0f;
}
