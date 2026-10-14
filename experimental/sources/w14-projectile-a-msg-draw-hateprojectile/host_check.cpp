// Host contract for msg_draw__14HateProjectile; not an EE input. isin__Fi logs its argument and returns a fixed single; P_FaceSprite
// logs its arguments. The authored function was also compared with the original in a private R5900 run (150 scenarios, every original
// word covered; mutants of the fade offset, the pulse multiplier and modulus, the constants (except one-ulp changes of the pool
// singles), the colour integers, the shift and the final truncation are caught there).
#define W12_SPELL_C_HOST 1
#include "../w12-actor-spell-c-common/access.hpp"
#include <cstdio>
#include <cstring>
unsigned char* w12_spell_c_host_base;
unsigned w12_spell_c_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static unsigned char arena[0x200];
static int sin_arg, sin_calls, face_calls;
static float sin_value = 0.5f;
static Point3 face_p; static float face_a, face_b; static int face_i[4]; static void* face_texture; static short face_g; static int face_h;
extern "C" {
int frameNm;
void* haloTexture5 = reinterpret_cast<void*>(0x2468);
float isin__Fi(int angle) { ++sin_calls; sin_arg = angle; return sin_value; }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float a, float b, int c, int d, int e, int f, void* texture, short g, int h) {
    ++face_calls; face_p = p; face_a = a; face_b = b; face_i[0] = c; face_i[1] = d; face_i[2] = e; face_i[3] = f; face_texture = texture; face_g = g; face_h = h;
}
}
#include "msg_draw.cpp"

static int run(int frame, int stamp, float s) {
    using namespace w12c;
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    unsigned char* o = arena + 0x40;
    real(o, 16) = 1.5f; real(o, 20) = -2.0f; real(o, 24) = 3.25f;
    sword(o, 88) = stamp;
    frameNm = frame; sin_value = s; sin_calls = face_calls = 0;
    msg_draw__14HateProjectile(o);
    if (sin_calls != 1 || face_calls != 1) return 1;
    if (sin_arg != (frame % 32768) * 2701) return 2;
    float u = static_cast<float>(stamp - frame + 180) * bits(0x3d088888u);
    if (1.0f < u) u = 1.0f;
    const float a = u * bits(0x461c4000u);
    const float b = static_cast<float>(static_cast<int>(a * 0.25f));
    const float size = static_cast<float>(static_cast<int>(a + s * b));
    if (face_p.x != 1.5f || face_p.y != -2.0f || face_p.z != 3.25f || face_a != 0.0f || face_b != size) return 3;
    if (face_i[0] != 80 || face_i[1] != 0 || face_i[2] != 0 || face_i[3] != static_cast<int>(u * 128.0f)) return 4;
    if (face_texture != haloTexture5 || face_g != static_cast<short>(frame << 10) || face_h != 1) return 5;
    return 0;
}
int main() {
    int r;
    if ((r = run(1000, 1100, 0.5f))) return r;          // u = 280 / 30 limited to 1.0 -> no: (1100 - 1000 + 180) * k > 1, limited
    if ((r = run(1000, 850, 0.25f))) return 10 + r;      // u below 1.0
    if ((r = run(-5, 20, -0.75f))) return 20 + r;        // negative frame counter: C remainder
    if ((r = run(40000, 40000, 1.0f))) return 30 + r;
    return 0;
}
