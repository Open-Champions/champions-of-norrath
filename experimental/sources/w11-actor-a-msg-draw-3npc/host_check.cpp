// Host contract for msg_draw__3NPC; not an EE input. 32-bit words are offsets from a host base; the libc string
// functions are the host's own, every other callee is a stub that logs its arguments (strings by content). Each
// scenario lists the exact call log the rule in msg_draw.cpp gives. The authored function was also compared with
// the original in a private R5900 run (500 scenarios, every word executed). Build with -ffp-contract=off. Returns
// non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cmath>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <strings.h>
#include <string>
unsigned char* w11_actor_a_host_base;
#include "msg_draw.cpp"

// The source names the libc functions through assembler labels; on the host those labels are bound to wrappers
// around the host's own functions.
extern "C" int host_sprintf(char* b, const char* f, ...) __asm__("sprintf");
extern "C" int host_strcmp(const char* a, const char* b) __asm__("strcmp");
extern "C" int host_strcasecmp(const char* a, const char* b) __asm__("strcasecmp");
extern "C" char* host_strstr(const char* a, const char* b) __asm__("strstr");
extern "C" int host_sprintf(char* b, const char* f, ...) { va_list ap; va_start(ap, f); const int n = std::vsprintf(b, f, ap); va_end(ap); return n; }
extern "C" int host_strcmp(const char* a, const char* b) { return std::strcmp(a, b); }
extern "C" int host_strcasecmp(const char* a, const char* b) { return ::strcasecmp(a, b); }
extern "C" char* host_strstr(const char* a, const char* b) { return const_cast<char*>(std::strstr(a, b)); }
float worldCameraPos[3];
float worldViewMatrix[16];
signed char cutMode;
int frameNm;
void* gameCam;
void* haloTexture2;
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int selRunning, selVisible, scriptResult, tagExclamation, tagShort;
static float wResult, noiseResult;
static Point3 matOut, attachOut;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%.9g", v); return b; }
static unsigned off(const void* p) { return p ? static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena) : 0u; }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
extern "C" {
int playerSelectionRunning__Fv() { calls += "selrun;"; return selRunning; }
int playerSelectionNPCVisible__FP3NPC(void* n) { calls += "visible(" + num(off(n)) + ");"; return selVisible; }
unsigned long long playerSelectionGetNPCActiveChunks__FP3NPC(void* n) { calls += "selchunks(" + num(off(n)) + ");"; return 0x500; }
void playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord(void* n, void* m, void* r) { calls += "seldraw(" + num(off(n)) + "," + num(off(m)) + "," + num(off(r)) + ");"; }
char* findTagString__10GameObjectPci(void*, const char* tag, int i) {
    calls += std::string("tag(") + tag + "," + num(i) + ");";
    static char s[] = "script";
    return (!std::strcmp(tag, "exclamation") ? tagExclamation : tagShort) ? s : nullptr;
}
int scriptStart__FPc(char* s) { calls += std::string("script(") + s + ");"; return scriptResult; }
float matTransform__FP6Point3P8Matrix44T0(Point3* out, void* m, Point3* in) {
    calls += std::string("transform(") + (m == worldViewMatrix ? "view" : "?") + "," + pt(*in) + ");"; *out = matOut; return wResult;
}
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return arena + 0x7000 + (std::strlen(a) + std::strlen(b)) * 16; }
void _textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int z, unsigned c) { calls += "icon(" + num(off(t)) + "," + num(x) + "," + num(y) + "," + num(z) + "," + std::to_string(c) + ");"; }
float isin__Fi(int a) { calls += "isin(" + num(a) + ");"; return 0.5f; }
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int bits) { calls += "chunks(" + num(off(m)) + "," + num(static_cast<unsigned>(bits)) + ");"; return 0x700 + static_cast<unsigned>(bits); }
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* x) { calls += "draw(" + num(off(s)) + "," + num(off(m)) + "," + num(off(t)) + "," + num(off(x)) + ");"; return arena + 0x7f00; }
float ef1Noise__Fi(int a) { calls += "noise(" + num(static_cast<unsigned>(a)) + ");"; return noiseResult; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short a, void* s, int b, Point3* first, Point3*, float f) {
    calls += "attach(" + num(off(m)) + "," + pt(p) + "," + num(a) + "," + num(off(s) - 0x1000) + "," + num(b) + "," + num(f) + ");"; *first = attachOut;
}
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float a, float b, int c0, int c1, int c2, int alpha, void* tex, short ang, int mode) {
    calls += "sprite(" + pt(p) + "," + num(a) + "," + num(b) + "," + num(c0) + "," + num(c1) + "," + num(c2) + "," + num(alpha) + "," + num(off(tex)) + "," + num(ang) + "," + num(mode) + ");";
}
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* a, void* b, int c, void* d, void* r, unsigned long long f, int e, int g) {
    calls += "attachment(" + num(off(a)) + "," + num(off(b)) + "," + num(c) + "," + num(off(d)) + "," + num(off(r)) + "," + std::to_string(f) + "," + num(e) + "," + num(g) + ");";
}
}
struct Cfg {
    const char* name = "plain"; float pos[3] = {10, 0, 0}; int cut = 0; short h1066 = 5; const char* alt = "other"; unsigned bits = 77; bool cam = true; unsigned camState = 1;
    int running = 0, visible = 1, tag = 1, tagS = 0, script = 1; int frame = 5;
};
static int bad = 0;
static void run(const char* label, const Cfg& c, const std::string& want) {
    std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear();
    worldCameraPos[0] = worldCameraPos[1] = worldCameraPos[2] = 0;
    for (int i = 0; i < 16; ++i) worldViewMatrix[i] = 0;
    worldViewMatrix[2] = -1; cutMode = static_cast<signed char>(c.cut); frameNm = c.frame; haloTexture2 = arena + 0x7800;
    static unsigned char camObj[64]; std::memset(camObj, 0, sizeof camObj); std::memcpy(camObj + 52, &c.camState, 4); gameCam = c.cam ? camObj : nullptr;
    std::memcpy(obj + 16, c.pos, 12); std::strcpy(reinterpret_cast<char*>(obj + 960), c.name); std::strcpy(reinterpret_cast<char*>(obj + 992), "abc"); std::strcpy(reinterpret_cast<char*>(obj + 1024), "tex1");
    std::strcpy(reinterpret_cast<char*>(obj + 1068), c.alt); std::memcpy(obj + 1066, &c.h1066, 2); unsigned m = 0x2000, t = 0x2100; std::memcpy(obj + 1056, &m, 4); std::memcpy(obj + 1060, &t, 4);
    std::memcpy(obj + 244, &c.bits, 4); short heading = 33; std::memcpy(obj + 56, &heading, 2); float f224 = 0.5f; std::memcpy(obj + 224, &f224, 4);
    selRunning = c.running; selVisible = c.visible; tagExclamation = c.tag; tagShort = c.tagS; scriptResult = c.script; wResult = 2.0f; matOut = Point3{100, 40, 8}; noiseResult = 0.25f; attachOut = Point3{7, 8, 9};
    msg_draw__3NPC(obj);
    if (calls != want) { std::printf("fail: %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); bad = 1; }
}
int main() {
    const std::string draw = "draw(4096,8192,8448,0);";
    const std::string lumps = "lump(plain,lowabc);lump(plain,lowtex1);chunks(28848,77);draw(4096,28848,28864,0);";
    { Cfg c; c.pos[0] = -10; run("behind the camera", c, ""); }
    { Cfg c; c.running = 1; c.visible = 0; run("selection hides it", c, "selrun;visible(4096);"); }
    { Cfg c; c.running = 1; c.visible = 1; run("selection shows it", c, "selrun;visible(4096);chunks(8192,77);" + draw); }
    { Cfg c; run("near", c, "selrun;chunks(8192,77);" + draw); }
    { Cfg c; c.pos[0] = 250; c.cam = false; run("far with the limit 240", c, "selrun;" + draw); }
    { Cfg c; c.pos[0] = 250; c.cut = 1; run("cut mode limit 300", c, "selrun;chunks(8192,77);" + draw); }
    { Cfg c; c.pos[0] = 500; c.h1066 = -1; c.cam = false; run("far, negative halfword", c, "selrun;" + lumps); }
    { Cfg c; c.alt = "FIRLOW"; run("firlow", c, "selrun;chunks(8192,77);" + lumps); }
    { Cfg c; c.alt = "FIRLOW"; c.cut = 2; run("firlow ignored in cut mode", c, "selrun;chunks(8192,77);" + draw); }
    { Cfg c; c.name = "select_npc"; run("select", c, "selrun;chunks(8192,77);selchunks(4096);" + draw + "seldraw(4096,8192,32512);"); }
    // we_king.lmp: the glow sprite.
    { Cfg c; c.name = "we_king.lmp"; c.frame = 5;
      const unsigned seed = 4096u * 0x19660du + (5u << 4) + 0x7efe41u;
      run("we_king", c, "selrun;chunks(8192,77);" + draw + "noise(" + num(seed) + ");attach(8192,10:0:0,33,288,0,0.5);sprite(7:8:9,5,4125,84,101,46,94,30720,0,2);"); }
    { Cfg c; c.name = "superorc.lmp"; run("superorc", c, "selrun;chunks(8192,77);" + draw + "lump(superorc.lmp,cleever.vif);lump(superorc.lmp,cleever.tex);attachment(29040,29040,0,8192,32512,18446744073709551615,-1,0);"); }
    { Cfg c; c.name = "pelyshor.lmp"; run("pelyshor", c, "selrun;chunks(8192,77);" + draw + "lump(pelyshor.lmp,thehorn.vif);lump(pelyshor.lmp,thehorn.tex);attachment(29040,29040,0,8192,32512,18446744073709551615,-1,0);"); }
    // Far with the exclamation marker.
    const std::string marker = "tag(exclamation,0);tag(exclamation,0);script(script);";
    { Cfg c; c.pos[0] = 500; c.frame = 5;
      run("marker", c, "selrun;" + marker + "tag(exclamationShort,0);transform(view,500:0:72);lump(icons.lmp,exclaim.tex);isin(5120);icon(28992,318,186,5,2021687424);" + draw); }
    { Cfg c; c.pos[0] = 500; c.tagS = 1; c.frame = 5;
      run("short marker", c, "selrun;" + marker + "tag(exclamationShort,0);transform(view,500:0:48);lump(icons.lmp,exclaim.tex);isin(5120);icon(28992,318,186,5,2021687424);" + draw); }
    { Cfg c; c.pos[0] = 500; c.script = 0; run("script says no", c, "selrun;" + marker + draw); }
    { Cfg c; c.pos[0] = 500; c.tag = 0; run("no marker tag", c, "selrun;tag(exclamation,0);" + draw); }
    { Cfg c; c.pos[0] = 500; c.camState = 2; run("camera state 2", c, "selrun;" + draw); }
    { Cfg c; c.pos[0] = 500; c.cam = false; run("no camera", c, "selrun;" + draw); }
    return bad;
}
