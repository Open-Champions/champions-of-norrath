// Host check only; not a build input. The routine's memset and setters are replaced by recording stubs; the trace holds the
// call order, the float bits or colour lanes passed, and whether the object was already zero when memset ran.
#define memset norrath_record_memset
#include "ctor.cpp"
#undef memset
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[1024];
static std::string trace;
extern "C" void* norrath_record_memset(void* destination, int value, unsigned long count) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "memset:%ld,%d,%lu;", static_cast<unsigned char*>(destination) - object, value, count);
    trace += buf;
    std::memset(destination, value, count);
    return destination;
}
static unsigned bits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
extern "C" void SetFriction__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetFriction:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetBirthrate__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetBirthrate:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetBirthrateVar__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetBirthrateVar:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetLifespan__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetLifespan:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetLifespanVar__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetLifespanVar:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetVelocity__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetVelocity:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetVelocityVar__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetVelocityVar:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetStartSize__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetStartSize:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetStartSizeVar__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetStartSizeVar:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetEndSize__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetEndSize:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetEndSizeVar__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetEndSizeVar:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetStartColorVar__20VIParticleAttributesRC10VIColor32F(void* self, const float* c) { char b[160]; std::snprintf(b, sizeof b, "SetStartColorVar:%ld,%08x,%08x,%08x,%08x;", static_cast<unsigned char*>(self) - object, bits(c[0]), bits(c[1]), bits(c[2]), bits(c[3])); trace += b; }
extern "C" void SetEndColorVar__20VIParticleAttributesRC10VIColor32F(void* self, const float* c) { char b[160]; std::snprintf(b, sizeof b, "SetEndColorVar:%ld,%08x,%08x,%08x,%08x;", static_cast<unsigned char*>(self) - object, bits(c[0]), bits(c[1]), bits(c[2]), bits(c[3])); trace += b; }
extern "C" void SetInheritVelocity__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetInheritVelocity:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetDeltaSpawn__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetDeltaSpawn:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetNozzleAxis__20VIParticleAttributesRC7VIVect3(void* self, const float* c) { char b[160]; std::snprintf(b, sizeof b, "SetNozzleAxis:%ld,%08x,%08x,%08x;", static_cast<unsigned char*>(self) - object, bits(c[0]), bits(c[1]), bits(c[2])); trace += b; }
extern "C" void SetNozzleHprVar__20VIParticleAttributesRC7VIVect3(void* self, const float* c) { char b[160]; std::snprintf(b, sizeof b, "SetNozzleHprVar:%ld,%08x,%08x,%08x;", static_cast<unsigned char*>(self) - object, bits(c[0]), bits(c[1]), bits(c[2])); trace += b; }
extern "C" void SetGradientRepeat__20VIParticleAttributesf(void* self, float value) { char b[96]; std::snprintf(b, sizeof b, "SetGradientRepeat:%ld,%08x;", static_cast<unsigned char*>(self) - object, bits(value)); trace += b; }
extern "C" void SetGravityOn__20VIParticleAttributesi(void* self, int on) { char b[96]; std::snprintf(b, sizeof b, "SetGravityOn:%ld,%d;", static_cast<unsigned char*>(self) - object, on); trace += b; }
extern "C" void SetGradientColor__20VIParticleAttributesiRC10VIColor32F(void* self, int index, const float* c) { char b[160]; std::snprintf(b, sizeof b, "SetGradientColor:%ld,%d,%08x,%08x,%08x,%08x;", static_cast<unsigned char*>(self) - object, index, bits(c[0]), bits(c[1]), bits(c[2]), bits(c[3])); trace += b; }

static std::string f(const char* name, unsigned v) { char b[64]; std::snprintf(b, sizeof b, "%s:0,%08x;", name, v); return b; }

int main() {
    std::memset(object, 0x5a, sizeof object);
    __20VIParticleAttributes(object);
    std::string want = "memset:0,0,724;";
    want += f("SetFriction", 0) + f("SetBirthrate", 0x42c80000u) + f("SetBirthrateVar", 0) + f("SetLifespan", 0x3f000000u) +
            f("SetLifespanVar", 0) + f("SetVelocity", 0x40000000u) + f("SetVelocityVar", 0) + f("SetStartSize", 0x3e4cccccu) +
            f("SetStartSizeVar", 0) + f("SetEndSize", 0x3e4cccccu) + f("SetEndSizeVar", 0);
    want += "SetStartColorVar:0,00000000,00000000,00000000,00000000;SetEndColorVar:0,00000000,00000000,00000000,00000000;";
    want += f("SetInheritVelocity", 0) + f("SetDeltaSpawn", 0);
    want += "SetNozzleAxis:0,00000000,3f800000,00000000;SetNozzleHprVar:0,3f800000,3f800000,3f800000;";
    want += f("SetGradientRepeat", 0x3f800000u);
    want += "SetGravityOn:0,0;";
    for (int i = 0; i < 32; ++i) {
        float v = 1.0f - static_cast<float>(i) * __builtin_bit_cast(float, 0x3d042108);
        char b[160];
        std::snprintf(b, sizeof b, "SetGradientColor:0,%d,3f800000,%08x,00000000,%08x;", i, bits(v), bits(v));
        want += b;
    }
    bool ok = trace == want;
    // memset clears the first 724 bytes only, and the setters are stubs, so the object ends zero there and untouched after.
    for (int i = 0; i < 724; ++i) if (object[i] != 0) ok = false;
    for (int i = 724; i < 1024; ++i) if (object[i] != 0x5a) ok = false;
    if (!ok) std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str());
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
