// Host contract for msg_draw__10StaticFire; not an EE input. 32-bit words are offsets from a host
// base; callees log. The authored function was also compared with the original in a private R5900
// run (250 random-state scenarios; 294 of 295 original words reached, the missing one padding).
#define W10_ACTOR_N_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "msg_draw.cpp"

char levelName[512];
int frameNm;
void* haloTexture2 = reinterpret_cast<void*>(0x22);
void* haloTexture7 = reinterpret_cast<void*>(0x77);
void* haloTexture8 = reinterpret_cast<void*>(0x88);
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int visibleFirst, visibleRing;
static int visits;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
char* strstr(const char* h, const char* n) {
    for (; *h; ++h) { const char *a = h, *b = n; while (*a && *b && *a == *b) { ++a; ++b; } if (!*b) return const_cast<char*>(h); }
    return nullptr;
}
int gamePositionVisible__FP6Point3f(Point3* p, float r) {
    const bool first = visits++ == 0;
    calls += "vis(" + num(p->x) + ":" + num(p->y) + ":" + num(p->z) + "," + num(r) + ");";
    return first ? visibleFirst : visibleRing;
}
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float s, float s2, int a, int b, int c, int d, void* t, short spin, int flag) {
    calls += "sprite(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(s) + "," + num(s2) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," +
             num(reinterpret_cast<unsigned long>(t)) + "," + num(spin) + "," + num(flag) + ");";
}
}
static void put32(unsigned a, unsigned v) { std::memcpy(obj + a, &v, 4); }
static void putf(unsigned a, float v) { std::memcpy(obj + a, &v, 4); }
static void put16(unsigned char* b, unsigned a, int v) { short s = static_cast<short>(v); std::memcpy(b + a, &s, 2); }
static unsigned lo() { return static_cast<unsigned>(obj - arena); }
static void reset() {
    std::memset(arena, 0, sizeof arena); w10_actor_n_host_base = arena; calls.clear(); visits = 0;
    visibleFirst = 1; visibleRing = 1; frameNm = 3;
    std::memset(levelName, 0, sizeof levelName); std::strcpy(levelName + 81, "faydark");
    putf(16, 100); putf(20, 200); putf(24, 300);
    putf(92, 0.001f); putf(96, 80.0f); putf(100, 0.5f); putf(104, 5000.0f);
    put16(obj, 118, 4); put16(obj, 120, 0); put16(obj, 122, 0);
    put32(1188, 0x4000);
}
static int bad;
static void check(const char* label, const std::string& want) {
    if (calls != want) { std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); bad = 1; }
}
int main() {
    // No ring entries and nothing lit: only the visibility test.
    reset(); msg_draw__10StaticFire(obj); check("empty", "vis(100:200:300,941);");
    // The halo sprite needs every condition: visible, byte +110, word +124, byte +109 clear.
    const std::string halo = "vis(100:200:300,941);sprite(100:200:312,26,";
    reset(); obj[110] = 1; put32(124, 5); msg_draw__10StaticFire(obj);
    check("halo", halo + num((80.0f * 59.0f + 12000.0f) * 0.5f) + ",185,104,81,110,34,0,6);");
    reset(); obj[110] = 1; put32(124, 5); obj[109] = 1; msg_draw__10StaticFire(obj); check("halo cut", "vis(100:200:300,941);");
    reset(); obj[110] = 1; msg_draw__10StaticFire(obj); check("halo no counter", "vis(100:200:300,941);");
    reset(); put32(124, 5); msg_draw__10StaticFire(obj); check("halo no flame", "vis(100:200:300,941);");
    reset(); obj[110] = 1; put32(124, 5); visibleFirst = 0; msg_draw__10StaticFire(obj); check("halo hidden", "vis(100:200:300,941);");
    // One ring record (index 0 to 1): position is the object point plus (a, b, c) / 16.
    reset(); put16(obj, 120, 1); put16(arena + 0x4000, 0, 32); put16(arena + 0x4000, 2, -16); put16(arena + 0x4000, 4, 800);
    frameNm = 0; msg_draw__10StaticFire(obj);
    // v = min(800 * 0.001, 1) = 0.8, w = v; colours ((0.8 * -46 + 56) * 0.5, ...)
    {
        const float w = 800.0f * 0.001f;
        char want[512];
        std::snprintf(want, sizeof want, "vis(100:200:300,941);vis(102:199:350,941);sprite(102:199:350,5,%s,%d,%d,%d,%d,%d,%d,1);", num((5000.0f - 4800.0f) * w + 4800.0f).c_str(),
                      static_cast<int>((w * -46.0f + 56.0f) * 0.5f), static_cast<int>((w * -43.0f + 56.0f) * 0.5f), static_cast<int>((w * -42.0f + 56.0f) * 0.5f),
                      static_cast<int>(w * -171.0f + 171.0f), 0x77, (static_cast<int>(lo()) * 179) % 65536);
        check("ring", want);
    }
    // Byte +111 doubles the intensity; the odd index negates the spin; the ring wraps at +118.
    reset(); put16(obj, 118, 3); put16(obj, 122, 1); put16(obj, 120, 0); obj[111] = 1;
    put16(arena + 0x4000 + 10, 4, 100); put16(arena + 0x4000 + 20, 4, 100); put16(arena + 0x4000, 4, 5000); frameNm = 7; msg_draw__10StaticFire(obj);
    {
        // two records (1 and 2), then the index wraps to 0 which is the end bound: v = 100 * 0.001 = 0.1, w = 0.2
        const int k = (static_cast<int>(lo()) + 7) * 179;
        const std::string want = "vis(100:200:300,941);vis(100:200:306.25,941);sprite(100:200:306.25,5," + num((5000.0f - 4800.0f) * (0.1f + 0.1f) + 4800.0f) + "," +
            std::to_string(static_cast<int>((0.2f * -46.0f + 56.0f) * 0.5f)) + "," + std::to_string(static_cast<int>((0.2f * -43.0f + 56.0f) * 0.5f)) + "," +
            std::to_string(static_cast<int>((0.2f * -42.0f + 56.0f) * 0.5f)) + "," + std::to_string(static_cast<int>(0.1f * -171.0f + 171.0f)) + ",119," + std::to_string(-(k % 65536)) +
            ",1);vis(100:200:306.25,941);sprite(100:200:306.25,5," + num((5000.0f - 4800.0f) * (0.1f + 0.1f) + 4800.0f) + "," +
            std::to_string(static_cast<int>((0.2f * -46.0f + 56.0f) * 0.5f)) + "," + std::to_string(static_cast<int>((0.2f * -43.0f + 56.0f) * 0.5f)) + "," +
            std::to_string(static_cast<int>((0.2f * -42.0f + 56.0f) * 0.5f)) + "," + std::to_string(static_cast<int>(0.1f * -171.0f + 171.0f)) + ",119," + std::to_string(k % 65536) + ",1);";
        check("wrap", want);
    }
    // Hidden ring entries draw nothing.
    reset(); put16(obj, 120, 1); visibleRing = 0; msg_draw__10StaticFire(obj); check("ring hidden", "vis(100:200:300,941);vis(100:200:300,941);");
    // The colder level name (contains "kel") pulls the colour towards (70, 102, 107) and picks haloTexture8 for u < 0.997.
    reset(); std::strcpy(levelName + 81, "xkelx"); put16(obj, 120, 1); put16(arena + 0x4000, 4, 800); frameNm = 0; msg_draw__10StaticFire(obj);
    reset(); std::strcpy(levelName + 81, "xkelx"); put16(obj, 120, 1); put16(arena + 0x4000, 4, 800); putf(24, 3000); frameNm = 0; msg_draw__10StaticFire(obj);
    if (calls.find(",119,") == std::string::npos) { std::printf("fail warm texture\n  %s\n", calls.c_str()); bad = 1; }
    reset(); std::strcpy(levelName + 81, "xkelx"); put16(obj, 120, 1); put16(arena + 0x4000, 4, 800); putf(24, -300); frameNm = 0; msg_draw__10StaticFire(obj);
    if (calls.find(",136,") == std::string::npos) { std::printf("fail cool texture 2\n  %s\n", calls.c_str()); bad = 1; }
    return bad;
}
