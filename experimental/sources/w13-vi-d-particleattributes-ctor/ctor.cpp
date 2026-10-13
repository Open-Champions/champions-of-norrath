// Observed: __20VIParticleAttributes at 0x010b3030 is 728 bytes in the pinned SLUS-20565 image and returns nothing. With the object
// as this (s) it first stores float 0.0 into: three floats at +28, thirty-two groups of four floats from +56 (+56..+567), four floats
// at +568, four at +584, and three-float groups at +612, +624, +636, +648, +660 and +672. It then calls memset(this, 0, 724), which
// clears all of those stores again, and then calls, in this order, with this as the first argument:
//   SetFriction(0.0), SetBirthrate(100.0), SetBirthrateVar(0.0), SetLifespan(0.5), SetLifespanVar(0.0), SetVelocity(2.0),
//   SetVelocityVar(0.0), SetStartSize(a), SetStartSizeVar(0.0), SetEndSize(a), SetEndSizeVar(0.0) where a is the float with bit
//   pattern 0x3e4ccccc,
//   SetStartColorVar(&c0), SetEndColorVar(&c0) where c0 is four floats of 0.0 (two separate locals),
//   SetInheritVelocity(0.0), SetDeltaSpawn(0.0), SetNozzleAxis(&(0.0, 1.0, 0.0)), SetNozzleHprVar(&(1.0, 1.0, 1.0)),
//   SetGradientRepeat(1.0), SetGravityOn(0),
// and then for i = 0 to 31: SetGradientColor(i, &c) where c is the four floats {1.0, v, 0.0, v} and v = 1.0 - (float)i * k, with k the
// float with bit pattern 0x3d042108 (close to 1/31).
// Inferred: the object is 724 bytes or more; the three-float and four-float groups are vectors and colours that the setters
// also write. The two bit patterns are the constants the image holds; their decimal source forms are not recorded.
// Unknown: the setters' bodies and the meaning of each field (all setters are imports). Nothing here ran on the EE.
extern "C" {
void* memset(void* destination, int value, unsigned long count);
void SetFriction__20VIParticleAttributesf(void* self, float value);
void SetBirthrate__20VIParticleAttributesf(void* self, float value);
void SetBirthrateVar__20VIParticleAttributesf(void* self, float value);
void SetLifespan__20VIParticleAttributesf(void* self, float value);
void SetLifespanVar__20VIParticleAttributesf(void* self, float value);
void SetVelocity__20VIParticleAttributesf(void* self, float value);
void SetVelocityVar__20VIParticleAttributesf(void* self, float value);
void SetStartSize__20VIParticleAttributesf(void* self, float value);
void SetStartSizeVar__20VIParticleAttributesf(void* self, float value);
void SetEndSize__20VIParticleAttributesf(void* self, float value);
void SetEndSizeVar__20VIParticleAttributesf(void* self, float value);
void SetStartColorVar__20VIParticleAttributesRC10VIColor32F(void* self, const float* color);
void SetEndColorVar__20VIParticleAttributesRC10VIColor32F(void* self, const float* color);
void SetInheritVelocity__20VIParticleAttributesf(void* self, float value);
void SetDeltaSpawn__20VIParticleAttributesf(void* self, float value);
void SetNozzleAxis__20VIParticleAttributesRC7VIVect3(void* self, const float* axis);
void SetNozzleHprVar__20VIParticleAttributesRC7VIVect3(void* self, const float* hpr);
void SetGradientRepeat__20VIParticleAttributesf(void* self, float value);
void SetGravityOn__20VIParticleAttributesi(void* self, int on);
void SetGradientColor__20VIParticleAttributesiRC10VIColor32F(void* self, int index, const float* color);

void __20VIParticleAttributes(void* self) {
    float* f = static_cast<float*>(self);
    for (int i = 7; i <= 9; ++i) f[i] = 0.0f;
    for (int i = 14; i < 14 + 32 * 4; ++i) f[i] = 0.0f;
    for (int i = 142; i <= 145; ++i) f[i] = 0.0f;
    for (int i = 146; i <= 149; ++i) f[i] = 0.0f;
    static const int groups[6] = {153, 156, 159, 162, 165, 168};
    for (int g = 0; g < 6; ++g)
        for (int i = 0; i < 3; ++i) f[groups[g] + i] = 0.0f;
    memset(self, 0, 724);
    const float a = __builtin_bit_cast(float, 0x3e4ccccc);
    const float k = __builtin_bit_cast(float, 0x3d042108);
    SetFriction__20VIParticleAttributesf(self, 0.0f);
    SetBirthrate__20VIParticleAttributesf(self, 100.0f);
    SetBirthrateVar__20VIParticleAttributesf(self, 0.0f);
    SetLifespan__20VIParticleAttributesf(self, 0.5f);
    SetLifespanVar__20VIParticleAttributesf(self, 0.0f);
    SetVelocity__20VIParticleAttributesf(self, 2.0f);
    SetVelocityVar__20VIParticleAttributesf(self, 0.0f);
    SetStartSize__20VIParticleAttributesf(self, a);
    SetStartSizeVar__20VIParticleAttributesf(self, 0.0f);
    SetEndSize__20VIParticleAttributesf(self, a);
    SetEndSizeVar__20VIParticleAttributesf(self, 0.0f);
    float startVar[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    SetStartColorVar__20VIParticleAttributesRC10VIColor32F(self, startVar);
    float endVar[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    SetEndColorVar__20VIParticleAttributesRC10VIColor32F(self, endVar);
    SetInheritVelocity__20VIParticleAttributesf(self, 0.0f);
    SetDeltaSpawn__20VIParticleAttributesf(self, 0.0f);
    float axis[3] = {0.0f, 1.0f, 0.0f};
    SetNozzleAxis__20VIParticleAttributesRC7VIVect3(self, axis);
    float hpr[3] = {1.0f, 1.0f, 1.0f};
    SetNozzleHprVar__20VIParticleAttributesRC7VIVect3(self, hpr);
    SetGradientRepeat__20VIParticleAttributesf(self, 1.0f);
    SetGravityOn__20VIParticleAttributesi(self, 0);
    for (int i = 0; i < 32; ++i) {
        float v = 1.0f - static_cast<float>(i) * k;
        float color[4] = {1.0f, v, 0.0f, v};
        SetGradientColor__20VIParticleAttributesiRC10VIColor32F(self, i, color);
    }
}
}
