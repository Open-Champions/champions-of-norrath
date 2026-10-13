// Host contract for convertToLord__11VampireLord; not an EE input. Callees are stubs that log. The pointer
// words at +36 and +28 are offsets from a host base.
#define W11_ACTOR_C_HOST 1
#include "convert_to_lord.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
static std::string calls;
static unsigned char* the_object;
static int header_token;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" void makeExplosion__FG6Point3f(Point3 position, float scale) {
    calls += "boom(" + num(position.x) + "," + num(position.y) + "," + num(position.z) + "," + num(scale) + ");";
}
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    calls += std::string("lump(") + lump + "," + name + ");";
    return &header_token;
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume) {
    calls += std::string("sound(") + (header == &header_token ? "H" : "?") + "," + (object == the_object ? "self" : "?") + "," + num(volume) + ");";
}
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags) {
    calls += "anim(" + std::to_string(static_cast<unsigned char*>(state) - the_object) + "," +
             std::to_string(static_cast<unsigned char*>(header) - w11_actor_c_host_base) + "," + std::to_string(flags) + ");";
}
int main() {
    int failures = 0;
    for (unsigned long long flags : {0ull, 0xffffffffffffffffull, 0x0123456789abcdefull})
        for (unsigned w12 : {0u, 0xffffffffu, 0x80000001u}) {
            static unsigned char base[4096];
            w11_actor_c_host_base = base;
            std::memset(base, 0x77, sizeof base);
            the_object = base + 64;
            unsigned char* block = base + 2048;
            *reinterpret_cast<unsigned*>(the_object + 36) = 2048;
            *reinterpret_cast<unsigned*>(block + 28) = 3000;
            float p[3] = {1.5f, -2.0f, 30.25f};
            std::memcpy(the_object + 16, p, 12);
            std::memcpy(the_object + 160, &flags, 8);
            std::memcpy(the_object + 12, &w12, 4);
            *reinterpret_cast<unsigned*>(the_object + 964) = 1;
            unsigned char before[4096];
            std::memcpy(before, base, sizeof base);
            calls.clear();
            convertToLord__11VampireLord(the_object);
            unsigned long long wantFlags = flags & ~0x100000ull;
            unsigned want12 = w12 & ~1u;
            std::memcpy(before + 64 + 160, &wantFlags, 8);
            std::memcpy(before + 64 + 12, &want12, 4);
            std::memset(before + 64 + 964, 0, 4);
            before[64 + 292] = 0;
            if (std::memcmp(before, base, sizeof base) != 0) { std::fprintf(stderr, "memory differs\n"); ++failures; }
            if (calls != "boom(1.5,-2,30.25,4);lump(fx.lmp,spel_we_explarrow_hit.anm);sound(H,self,1);anim(288,3000,1);") {
                std::fprintf(stderr, "calls: %s\n", calls.c_str()); ++failures;
            }
        }
    if (!failures) std::puts("convertToLord ok");
    return failures != 0;
}
