// Host contract for msg_draw__5Rondo; not an EE input. Words that hold addresses are offsets from a
// host base. Callees are stubs that log; the gameCam pointer is a host pointer. Covers each early
// exit, both marker heights and the arithmetic of the draw position and color.
#define W10_ACTOR_L_HOST 1
#include "../w10-actor-l-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_l_host_base;
unsigned w10_actor_l_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls; static int tag1, tag2, script_result; static float w_ret = 2.0f, sin_ret = 0.5f; static float screen_ret[3] = {40.0f, 20.0f, 6.0f};
static Point3 seen_position; static int seen_angle; static int tex_x, tex_y, tex_z; static unsigned tex_color;
extern "C" {
unsigned char* gameCam; int frameNm; unsigned char worldViewMatrix[64];
void draw__8CreatureP12_modelHeaderP8_textureT1(void*, void* m, void* t, int e) { calls += "draw(" + std::to_string(static_cast<unsigned char*>(m) - w10_actor_l_host_base) + "," + std::to_string(static_cast<unsigned char*>(t) - w10_actor_l_host_base) + "," + std::to_string(e) + ");"; }
char* findTagString__10GameObjectPci(void*, char* tag, int i) { calls += std::string("tag(") + tag + "," + std::to_string(i) + ");"; return (std::string(tag) == "exclamation" ? tag1 : tag2) ? tag : nullptr; }
int scriptStart__FPc(char*) { calls += "script;"; return script_result; }
float matTransform__FP6Point3P8Matrix44T0(Point3* out, void* m, Point3* in) { calls += "mat" + std::to_string(m == worldViewMatrix) + ";"; seen_position = *in; out->x = screen_ret[0]; out->y = screen_ret[1]; out->z = screen_ret[2]; return w_ret; }
char* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return reinterpret_cast<char*>(0x99); }
float isin__Fi(int a) { seen_angle = a; calls += "sin;"; return sin_ret; }
void textureDrawColor__FP8_textureiiiUi(void*, int x, int y, int z, unsigned c) { calls += "tex;"; tex_x = x; tex_y = y; tex_z = z; tex_color = c; }
}
#include "msg_draw.cpp"
alignas(16) static unsigned char arena[0x2000];
static void putf(int at, float v) { std::memcpy(arena + at, &v, 4); }
int main() {
    w10_actor_l_host_base = arena; unsigned char* const o = arena + 0x100; int bad = 0;
    alignas(16) static unsigned char cam[128];
    auto reset = [&]() {
        std::memset(arena, 0, sizeof arena); calls.clear(); std::memset(cam, 0, sizeof cam);
        unsigned g = 0x800, m = 0xa00, t = 0xb00; std::memcpy(o + 36, &g, 4); std::memcpy(arena + g + 12, &m, 4); std::memcpy(arena + g + 20, &t, 4);
        putf(0x100 + 16, 3.0f); putf(0x100 + 20, 4.0f); putf(0x100 + 24, 10.0f);
        gameCam = cam; const unsigned one = 1; std::memcpy(cam + 52, &one, 4); tag1 = 1; tag2 = 0; script_result = 1; frameNm = 3;
        w_ret = 2.0f; sin_ret = 0.5f; screen_ret[0] = 40.0f; screen_ret[1] = 20.0f; screen_ret[2] = 6.0f;
    };
    const std::string head = "draw(2560,2816,0);";
    reset(); gameCam = nullptr; msg_draw__5Rondo(o); if (calls != head) bad = 1;
    reset(); const unsigned two = 2; std::memcpy(cam + 52, &two, 4); msg_draw__5Rondo(o); if (calls != head) bad = 1;
    reset(); tag1 = 0; msg_draw__5Rondo(o); if (calls != head + "tag(exclamation,0);") bad = 1;
    reset(); script_result = 0; msg_draw__5Rondo(o); if (calls != head + "tag(exclamation,0);tag(exclamation,0);script;") bad = 1;
    reset(); script_result = 2; msg_draw__5Rondo(o); if (calls != head + "tag(exclamation,0);tag(exclamation,0);script;") bad = 1;
    for (int shortTag : {0, 1}) {
        reset(); tag2 = shortTag; msg_draw__5Rondo(o);
        if (calls != head + "tag(exclamation,0);tag(exclamation,0);script;tag(exclamationShort,0);mat1;lump(icons.lmp,exclaim.tex);sin;tex;") bad = 1;
        if (seen_position.x != 3.0f || seen_position.y != 4.0f || seen_position.z != (shortTag ? 58.0f : 82.0f)) bad = 1;
        // scale 0.5: x 20, y 10 -> draw x = (int)(20*0.5) + 293 = 303, y = 10 + 166; angle 3<<10; color top byte (int)(0.5*16+112)=120
        if (tex_x != 303 || tex_y != 176 || tex_z != 5 || seen_angle != (3 << 10) || tex_color != ((120u << 24) | 0x808080u)) bad = 1;
    }
    reset(); w_ret = 4.0f; screen_ret[0] = -33.0f; screen_ret[1] = 7.0f; sin_ret = -1.0f; frameNm = -2; msg_draw__5Rondo(o);
    // x: -33/4 = -8.25 -> *0.5 = -4.125 -> trunc -4; y: 7/4 = 1.75 -> 1; top byte (int)(-16+112)=96
    if (tex_x != 289 || tex_y != 167 || tex_color != ((96u << 24) | 0x808080u) || seen_angle != -2048) bad = 1;
    return bad;
}
