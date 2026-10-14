// Host contract for msg_draw__3NPC; not an EE input. 32-bit words are offsets from a host base; callees
// log. The authored function was also compared with the original in a private R5900 run (260 mixed
// scenarios over distance, camera, texts and tag/script results).
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_j_host_base;
#include "msg_draw.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static unsigned char gameCamBlock[128];
static std::string calls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static int running, visible, scriptResult;
static bool hasExclamation, hasShort;
static float matW;
extern "C" {
float worldCameraPos[3];
float worldViewMatrix[16];
signed char cutMode;
unsigned char* gameCam;
int frameNm;
void* haloTexture2;
int playerSelectionRunning__Fv() { calls += "running;"; return running; }
int playerSelectionNPCVisible__FP3NPC(void*) { calls += "visible;"; return visible; }
unsigned long long playerSelectionGetNPCActiveChunks__FP3NPC(void*) { calls += "selchunks;"; return 0x0102030405060708ull; }
void playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord(void*, void* m, void* r) { calls += "seldraw(" + num(off(m)) + "," + num(off(r)) + ");"; }
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int mask) { calls += "chunks(" + num(off(m)) + "," + num(mask) + ");"; return 0x1122334455667788ull; }
void* findTagString__10GameObjectPci(void*, const char* t, int) {
    calls += std::string("tag(") + t + ");";
    return (!std::strcmp(t, "exclamation") ? hasExclamation : hasShort) ? obj + 0x440 : nullptr;
}
int scriptStart__FPc(const char*) { calls += "script;"; return scriptResult; }
float matTransform__FP6Point3P8Matrix44T0(Point3* out, void*, Point3* in) {
    calls += "mat(" + num(in->x) + ":" + num(in->y) + ":" + num(in->z) + ");"; *out = Point3{200, 400, 8}; return matW;
}
void* lumpFindResource__FPcT0(char*, char* n) { calls += std::string("lump(") + n + ");"; return arena + 0x7000 + std::strlen(n); }
float isin__Fi(int a) { calls += "isin" + num(a) + ";"; return 0.5f; }
void _textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int s, unsigned c) { calls += "tdraw(" + num(off(t)) + "," + num(x) + "," + num(y) + "," + num(s) + "," + num(c) + ");"; }
float ef1Noise__Fi(int v) { calls += "noise" + num(static_cast<unsigned>(v)) + ";"; return 0.25f; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short a, void* s, int b, Point3* first, Point3* second, float f) {
    calls += "attach(" + num(off(m)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(off(s) - 0x1000) + "," + num(b) + "," + num(f) + ");";
    *first = Point3{11, 12, 13}; *second = Point3{0, 0, 0};
}
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float a, float b, int c, int d, int e, int f, void* t, short g, int h) {
    calls += "face(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + (t == reinterpret_cast<void*>(0x4321) ? "halo" : "other") + "," + num(g) + "," + num(h) + ");";
}
void* draw__8CreatureP12_modelHeaderP8_textureT1(void*, void* m, void* t, void* t2) { calls += "draw(" + num(off(m)) + "," + num(off(t)) + "," + (t2 ? "t2" : "null") + ");"; return arena + 0x7800; }
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* m2, void* rec, unsigned long b, int c, int d) {
    calls += "dattach(" + num(off(m)) + "," + num(off(t)) + "," + num(a) + "," + num(off(m2)) + "," + num(off(rec)) + "," + num(static_cast<int>(b)) + "," + num(c) + "," + num(d) + ");";
}
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static void reset(const char* lump, const char* name, float x) {
    std::memset(arena, 0, sizeof arena); std::memset(gameCamBlock, 0, sizeof gameCamBlock); w10_actor_j_host_base = arena; calls.clear();
    for (float& f : worldViewMatrix) f = 0;
    worldViewMatrix[2] = -1; worldViewMatrix[6] = 0; worldViewMatrix[10] = 0;
    worldCameraPos[0] = worldCameraPos[1] = worldCameraPos[2] = 0;
    cutMode = 0; gameCam = nullptr; frameNm = 100; haloTexture2 = reinterpret_cast<void*>(0x4321);
    running = 0; visible = 1; scriptResult = 1; hasExclamation = hasShort = false; matW = 2.0f;
    put32(1056, 0x500); put32(1060, 0x600); put32(244, 3);
    std::strcpy(reinterpret_cast<char*>(obj + 960), lump); std::strcpy(reinterpret_cast<char*>(obj + 992), "mdl"); std::strcpy(reinterpret_cast<char*>(obj + 1024), "tex");
    std::strcpy(reinterpret_cast<char*>(obj + 1068), name);
    float pos[3] = {x, 0, 0}; std::memcpy(obj + 16, pos, 12);
    short h = 5; std::memcpy(obj + 56, &h, 2); std::memcpy(obj + 1066, &h, 2);
    float f224 = 1.5f; std::memcpy(obj + 224, &f224, 4);
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
int main() {
    int bad = 0;
    const std::string drawPlain = "draw(1280,1536,null);";
    // Behind the camera (dot product above zero): nothing at all.
    reset("x.lmp", "x", 100); worldViewMatrix[2] = 1; msg_draw__3NPC(obj); bad |= check("behind", "");
    // Selection mode with an invisible NPC returns after the visibility check.
    reset("x.lmp", "x", 100); running = 1; visible = 0; msg_draw__3NPC(obj); bad |= check("hidden", "running;visible;");
    // Near: chunks recomputed every frame.
    reset("x.lmp", "x", 100); msg_draw__3NPC(obj); bad |= check("near", "running;chunks(1280,3);" + drawPlain);
    reset("x.lmp", "x", 240); msg_draw__3NPC(obj); bad |= check("at limit", "running;chunks(1280,3);" + drawPlain);
    // Far, negative halfword at +1066: resources looked up again.
    reset("x.lmp", "x", 241); { short neg = -1; std::memcpy(obj + 1066, &neg, 2); } msg_draw__3NPC(obj);
    bad |= check("far reload", "running;lump(lowmdl);lump(lowtex);chunks(" + num(0x7000 + 6) + ",3);draw(" + num(0x7000 + 6) + "," + num(0x7000 + 6) + ",null);");
    // Far, camera block present and the script returns 1: icon drawn.
    reset("x.lmp", "x", 500); gameCam = gameCamBlock; { unsigned one = 1; std::memcpy(gameCamBlock + 52, &one, 4); } hasExclamation = true;
    msg_draw__3NPC(obj);
    bad |= check("icon", "running;tag(exclamation);tag(exclamation);script;mat(500:0:72);lump(exclaim.tex);isin" + num(100 << 10) + ";tdraw(" + num(0x7000 + 11) + "," + num(static_cast<int>(200 * 0.5f * 0.5f) + 293) + "," + num(static_cast<int>(400 * 0.5f) + 166) + ",5," +
                 num(static_cast<unsigned>(static_cast<int>(0.5f * 16.0f + 112.0f)) << 24 | 0x808080u) + ");" + drawPlain);
    reset("x.lmp", "x", 500); gameCam = gameCamBlock; { unsigned one = 1; std::memcpy(gameCamBlock + 52, &one, 4); } hasExclamation = hasShort = true;
    msg_draw__3NPC(obj);
    bad |= check("icon short", "running;tag(exclamation);tag(exclamation);script;tag(exclamationShort);mat(500:0:48);lump(exclaim.tex);isin" + num(100 << 10) + ";tdraw(" + num(0x7000 + 11) + "," + num(static_cast<int>(200 * 0.5f * 0.5f) + 293) + "," + num(static_cast<int>(400 * 0.5f) + 166) + ",5," +
                 num(static_cast<unsigned>(static_cast<int>(0.5f * 16.0f + 112.0f)) << 24 | 0x808080u) + ");" + drawPlain);
    reset("x.lmp", "x", 500); gameCam = gameCamBlock; { unsigned one = 1; std::memcpy(gameCamBlock + 52, &one, 4); } hasExclamation = true; scriptResult = 0;
    msg_draw__3NPC(obj); bad |= check("script no", "running;tag(exclamation);tag(exclamation);script;" + drawPlain);
    // cutMode raises the limit to 300.
    reset("x.lmp", "x", 280); cutMode = 1; msg_draw__3NPC(obj); bad |= check("cut near", "running;chunks(1280,3);draw(1280,1536,null);");
    // The "firlow" name reloads resources when cutMode is zero.
    reset("x.lmp", "FirLow", 100); msg_draw__3NPC(obj);
    bad |= check("firlow", "running;chunks(1280,3);lump(lowmdl);lump(lowtex);chunks(" + num(0x7000 + 6) + ",3);draw(" + num(0x7000 + 6) + "," + num(0x7000 + 6) + ",null);");
    // Selection lump.
    reset("select_one.lmp", "x", 100); msg_draw__3NPC(obj);
    bad |= check("select", "running;chunks(1280,3);selchunks;draw(1280,1536,null);seldraw(1280,30720);");
    // King lump: noise, attachment points, sprite.
    reset("we_king.lmp", "x", 100); msg_draw__3NPC(obj);
    {
        const unsigned arg = 0x7efe41u + 0x1000u * 1664525u + (100u << 4);
        bad |= check("king", "running;chunks(1280,3);draw(1280,1536,null);noise" + num(arg) + ";attach(1280,100:0:0,5,288,0,1.5);face(11:12:13,5,4125,84,101,46,94,halo,0,2);");
    }
    // Attachments.
    reset("superorc.lmp", "x", 100); msg_draw__3NPC(obj);
    bad |= check("superorc", "running;chunks(1280,3);draw(1280,1536,null);lump(cleever.vif);lump(cleever.tex);dattach(" + num(0x7000 + 11) + "," + num(0x7000 + 11) + ",0,1280,30720,-1,-1,0);");
    reset("pelyshor.lmp", "x", 100); msg_draw__3NPC(obj);
    bad |= check("pelyshor", "running;chunks(1280,3);draw(1280,1536,null);lump(thehorn.vif);lump(thehorn.tex);dattach(" + num(0x7000 + 11) + "," + num(0x7000 + 11) + ",0,1280,30720,-1,-1,0);");
    return bad;
}
