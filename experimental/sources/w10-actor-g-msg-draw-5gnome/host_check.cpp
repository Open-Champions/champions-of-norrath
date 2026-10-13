// Host contract for msg_draw__5Gnome; not an EE input. Words are offsets from a host base.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
void* w10g_host_fn(unsigned) { return nullptr; }
#include "msg_draw.cpp"
unsigned gameCam; int frameNm; unsigned char worldViewMatrix[64];
static std::string calls; static int finds[3], fi; static int scriptResult; static float matW, sinResult; static float outv[3];
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static Point3 matIn; static void* matMatrix; static int sinArg; static unsigned lastColor; static int lastX, lastY, lastZ; static void* lastTexture;
static long off(const void* p) { return static_cast<long>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int draw__8CreatureP12_modelHeaderP8_textureT1(void*, void* m, void* t, void* x) { calls += "draw(" + num(off(m)) + "," + num(off(t)) + "," + (x ? "x" : "null") + ");"; return 0; }
char* findTagString__10GameObjectPci(void*, char* tag, int n) { calls += std::string("find(") + tag + "," + num(n) + ");"; return finds[fi++ % 3] ? reinterpret_cast<char*>(&calls) : nullptr; }
int scriptStart__FPc(char*) { calls += "script;"; return scriptResult; }
float matTransform__FP6Point3P8Matrix44T0(Point3* out, void* m, Point3* in) { calls += "mat;"; matIn = *in; matMatrix = m; out->x = outv[0]; out->y = outv[1]; out->z = outv[2]; return matW; }
void* lumpFindResource__FPcT0(char* l, char* r) { calls += std::string("lump(") + l + "," + r + ");"; return arena + 0x3000; }
float isin__Fi(int a) { sinArg = a; calls += "isin;"; return sinResult; }
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int z, unsigned c) { calls += "tdc;"; lastTexture = t; lastX = x; lastY = y; lastZ = z; lastColor = c; }
}
static int bad;
static void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s (calls %s)\n", what, calls.c_str()); bad = 1; } }
int main() {
    w10g_host_base = arena;
    unsigned char* o = arena + 0x1000;
    auto reset = [&]() {
        std::memset(arena, 0, sizeof arena); calls.clear(); fi = 0;
        unsigned t = 0x200, model = 0x400, tex = 0x500, cam = 0x600, one = 1; std::memcpy(o + 36, &t, 4);
        std::memcpy(arena + t + 12, &model, 4); std::memcpy(arena + t + 20, &tex, 4);
        gameCam = cam; std::memcpy(arena + cam + 52, &one, 4);
        float p[3] = {5, 6, 7}; std::memcpy(o + 16, p, 12);
        finds[0] = finds[1] = finds[2] = 1; scriptResult = 1; matW = 2.0f; sinResult = 0.5f; frameNm = 3;
        outv[0] = 100; outv[1] = 50; outv[2] = 8;
    };
    reset(); msg_draw__5Gnome(o);
    check(calls == "draw(1024,1280,null);find(exclamation,0);find(exclamation,0);script;find(exclamationShort,0);mat;lump(icons.lmp,exclaim.tex);isin;tdc;", "full path call order");
    check(matIn.x == 5 && matIn.y == 6 && matIn.z == 7 + 48.0f, "short tag found: z + 48");
    check(sinArg == 3 << 10, "angle is the frame number shifted by 10");
    check(lastX == 25 + 293 && lastY == 25 + 166 && lastZ == 5, "screen position: x/2 and y divided by w, plus 293 and 166");
    check(lastColor == ((static_cast<unsigned>(0.5f * 16 + 112) << 24) | 0x808080u), "color from the sine");
    check(lastTexture == arena + 0x3000, "texture from the lump lookup");
    reset(); finds[2] = 0; msg_draw__5Gnome(o); check(matIn.z == 7 + 72.0f, "short tag missing: z + 72");
    reset(); gameCam = 0; msg_draw__5Gnome(o); check(calls == "draw(1024,1280,null);", "no camera: draw only");
    reset(); { unsigned two = 2; std::memcpy(arena + 0x600 + 52, &two, 4); } msg_draw__5Gnome(o); check(calls == "draw(1024,1280,null);", "camera word not 1: draw only");
    reset(); finds[0] = 0; msg_draw__5Gnome(o); check(calls == "draw(1024,1280,null);find(exclamation,0);", "tag missing: stop");
    reset(); scriptResult = 0; msg_draw__5Gnome(o); check(calls == "draw(1024,1280,null);find(exclamation,0);find(exclamation,0);script;", "script result not 1: stop");
    reset(); scriptResult = 2; msg_draw__5Gnome(o); check(calls == "draw(1024,1280,null);find(exclamation,0);find(exclamation,0);script;", "script result 2: stop");
    reset(); matW = 4.0f; sinResult = -1.0f; msg_draw__5Gnome(o);
    check(lastX == 12 + 293 && lastY == 12 + 166 && lastColor == ((static_cast<unsigned>(-16.0f + 112) << 24) | 0x808080u), "other w and sine");
    if (!bad) std::puts("gnome msg_draw ok");
    return bad;
}
