// Host contract for msg_draw__6Player; not an EE input. Build with the host compiler and -fno-builtin
// (strcmp and memset are called by name). Words are offsets from w11_actor_b_host_base.
#define W11_ACTOR_B_HOST 1
#include "msg_draw.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char* w11_actor_b_host_base = nullptr;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
signed char cutMode, thereIsACutPlayer, hudNetWaiting, gameHidePlayers, netNetworkMode;
char engineCurrentLevelName[80];
int gamePause, gameControlPaused, frameNm, shopActiveFrame, gamePlayersPresent, shipOn;
int spellAlpha[4];
unsigned char haloColor[64];
unsigned short healColor[4], healPulse[4];
void* haloTexture1;
unsigned char worldCameraPos[12], worldCameraMatrix[48], worldViewMatrix[64], player[4 * 23984];
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static std::string calls;
static int dlg, camControl, localPlayer;
static float matW = 2.0f;
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string off(const void* p) { return p ? std::to_string(static_cast<const unsigned char*>(p) - arena) : std::string("null"); }
static std::string pt(const Point3& p) { return "(" + f(p.x) + "," + f(p.y) + "," + f(p.z) + ")"; }
extern "C" {
int compare_text(const char* a, const char* b) __asm__("strcmp");
int compare_text(const char* a, const char* b) { calls += std::string("cmp(") + b + ");"; return std::strcmp(a, b); }
void* clear_bytes(void* d, int v, unsigned n) __asm__("memset");
void* clear_bytes(void* d, int v, unsigned n) { calls += "clear" + std::to_string(n) + ";"; std::memset(d, v, n); return d; }
int DLG_Active__Fv() { calls += "dlg;"; return dlg; }
int netGetCameraCustomControl__Fi(int i) { calls += "cam" + std::to_string(i) + ";"; return camControl; }
int netPlayerIsLocal__Fi(int i) { calls += "local" + std::to_string(i) + ";"; return localPlayer; }
void playerItemScan__6PlayerP3padi(void* s, void* p, int a) { calls += "scan" + off(s) + "," + off(p) + "," + std::to_string(a) + ";"; }
void maybeDrawMultiPlayerMarkers__6Player(void*) { calls += "markers;"; }
void lightSetAmbientOffset__FPs(short* a) { calls += "amb" + std::to_string(a[0]) + "," + std::to_string(a[1]) + "," + std::to_string(a[2]) + "," + std::to_string(a[3]) + ";"; }
void lightResetOptionalParams__Fv() { calls += "lreset;"; }
void modelResetOptionalParams__Fv() { calls += "mreset;"; }
void matMakeTransYaw__FP8Matrix34G6Point3s(void*, Point3 p, short yaw) { calls += "make" + pt(p) + "," + std::to_string(yaw) + ";"; }
void matRot__FP8Matrix34is(void*, int a, short b) { calls += "rot" + std::to_string(a) + "," + std::to_string(b) + ";"; }
char* lumpFindResource__FPcT0(char* l, char* n) { calls += std::string("lump(") + l + "," + n + ");"; return reinterpret_cast<char*>(std::strlen(n) * 16); }
int modelDraw(void* m, void* t, int mode, Point3 p, void*, void* a, unsigned long long fl, void* r, int last) {
    calls += "mdraw" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(m)) + "," + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(t)) + "," + std::to_string(mode) + "," + pt(p) + "," +
             (a ? "A" : "") + std::to_string(fl) + (r ? "R" : "") + "," + std::to_string(last) + ";";
    return 0;
}
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* e) { calls += "cdraw" + off(s) + "," + off(m) + "," + off(t) + (e ? "E" : "") + ";"; return arena + 0x3000; }
void drawAttachments__6PlayerP11_drawRecordf(void* s, void* r, float z) { calls += "attach" + off(s) + "," + off(r) + "," + f(z) + ";"; }
float isin__Fi(int a) { calls += "isin" + std::to_string(a) + ";"; return 0.5f; }
void modelGetCharAttachmentPos(void* m, Point3 p, short angle, void* a, int which, Point3* out, Point3* out2, float s) {
    calls += "gcap" + off(m) + "," + pt(p) + "," + std::to_string(angle) + "," + off(a) + "," + std::to_string(which) + (out2 ? "O" : "") + "," + f(s) + ";";
    out->x = 10.0f * (which + 1); out->y = 20.0f; out->z = 30.0f;
}
void P_FaceSprite(const Point3* p, float size, float depth, int r, int g, int b, int a, void* t, short fl, int mode) {
    calls += "face" + pt(*p) + "," + f(size) + "," + f(depth) + "," + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(a) + "," +
             std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(t)) + "," + std::to_string(fl) + "," + std::to_string(mode) + ";";
}
float matTransform__FP6Point3P8Matrix44T0(Point3* out, void*, Point3* in) {
    calls += "mtr" + pt(*in) + ";"; out->x = 100.0f; out->y = 50.0f; out->z = 8.0f; return matW;
}
void textureDrawColor(void* t, int x, int y, int z, unsigned c) {
    calls += "tex" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(t)) + "," + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(c) + ";";
}
}
static void set_real(unsigned o, float v) { std::memcpy(arena + o, &v, 4); }
static void set_word(unsigned o, unsigned v) { std::memcpy(arena + o, &v, 4); }
static void set_half(unsigned o, short v) { std::memcpy(arena + o, &v, 2); }
static void set_global(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
static void set_player_real(int i, unsigned o, float v) { std::memcpy(player + i * 23984 + o, &v, 4); }

static void reset() {
    std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player);
    std::memset(healColor, 0, sizeof healColor); std::memset(healPulse, 0, sizeof healPulse);
    w11_actor_b_host_base = arena;
    cutMode = 0; thereIsACutPlayer = 0; hudNetWaiting = 0; gameHidePlayers = 0; netNetworkMode = 0;
    std::strcpy(engineCurrentLevelName, "world");
    gamePause = 0; gameControlPaused = 0; frameNm = 100; shopActiveFrame = 0; gamePlayersPresent = 1; shipOn = 0;
    std::memset(spellAlpha, 0, sizeof spellAlpha); haloTexture1 = reinterpret_cast<void*>(0x777);
    for (int i = 0; i < 16; ++i) haloColor[i * 4] = static_cast<unsigned char>(i + 1);
    std::memset(worldCameraPos, 0, sizeof worldCameraPos); std::memset(worldCameraMatrix, 0, sizeof worldCameraMatrix);
    dlg = 0; camControl = 0; localPlayer = 0; matW = 2.0f;
    set_word(960, 1); set_real(16, 1.0f); set_real(20, 2.0f); set_real(24, 3.0f); set_half(56, 9); set_word(1332, 0x1000); set_word(1344, 0x1100);
    calls.clear();
}
#define CHECK(cond, msg) do { if (!(cond)) { std::puts(calls.c_str()); std::puts("fail: " msg); return 1; } } while (0)
static const std::string PLAIN = "cam1;markers;clear8;";

int main() {
    // plain draw: camera-side distance 0 -> |dot| < 360 so the size shrinks; no attachments, no sprites
    reset(); frameNm = 100;
    msg_draw__6Player(arena);
    CHECK(calls.rfind("dlg;cam1;local1;cam1;", 0) == 0 || calls.rfind("dlg;cam1;cam1;", 0) == 0, "start");
    CHECK(calls.find("cdraw0,4096,4352;attach0,12288,") != std::string::npos, "creature draw");
    CHECK(calls.find("attach0,12288,3.5;") == std::string::npos, "size shrinks");
    // size: |dot| = 0 -> 5 - 360 * 0.018055... = -1.5 -> clamped to 1
    CHECK(calls.find("attach0,12288,1;") != std::string::npos, "clamped size");
    CHECK(arena[1108] == 0, "byte 1108 cleared");
    // cut mode: the byte at +1106 must be zero and a cut player must exist
    reset(); cutMode = 1; thereIsACutPlayer = 1; arena[1108] = 0;
    msg_draw__6Player(arena);
    CHECK(calls == "cmp(bronzeht);cmp(gnomehub);cmp(nas);" && arena[1108] == 1, "cut mode on an unlisted level");
    reset(); cutMode = 1; thereIsACutPlayer = 1; std::strcpy(engineCurrentLevelName, "gnomehub");
    msg_draw__6Player(arena);
    CHECK(calls.find("cmp(nas);") == std::string::npos && calls.find("cdraw") != std::string::npos && arena[1108] == 0, "listed level draws");
    reset(); cutMode = 1; thereIsACutPlayer = 1; arena[1106] = 1;
    msg_draw__6Player(arena);
    CHECK(calls.find("cmp(") == std::string::npos && calls.find("cdraw") != std::string::npos, "cut flag byte skips the name test");
    // item scan gating
    reset(); localPlayer = 1;
    msg_draw__6Player(arena);
    CHECK(calls.find("scan0,null,1;") != std::string::npos, "item scan");
    reset(); localPlayer = 1; gamePause = 1;
    msg_draw__6Player(arena);
    CHECK(calls.find("scan") == std::string::npos, "paused: no item scan");
    reset(); localPlayer = 1; hudNetWaiting = 1;
    msg_draw__6Player(arena);
    CHECK(calls.find("scan") == std::string::npos, "hud waiting: no item scan");
    // camera control: dot < 84 returns with the byte set
    reset(); camControl = 1; set_global(worldCameraMatrix + 8, 1.0f); set_real(16, 83.0f);
    msg_draw__6Player(arena);
    CHECK(arena[1108] == 1 && calls.find("markers") == std::string::npos, "camera too close");
    reset(); camControl = 1; set_global(worldCameraMatrix + 8, 1.0f); set_real(16, 84.0f);
    msg_draw__6Player(arena);
    CHECK(arena[1108] == 0 && calls.find("markers") != std::string::npos, "camera far enough");
    // shop window
    reset(); frameNm = 100; shopActiveFrame = 97;   // frame - 4 < 97 and not 104 < 97 -> return
    msg_draw__6Player(arena);
    CHECK(calls.find("markers") == std::string::npos, "shop window returns");
    reset(); frameNm = 100; shopActiveFrame = 96;
    msg_draw__6Player(arena);
    CHECK(calls.find("markers") != std::string::npos, "shop window edge");
    reset(); frameNm = 100; shopActiveFrame = 105;
    msg_draw__6Player(arena);
    CHECK(calls.find("markers") != std::string::npos, "shop window far side");
    reset(); gameHidePlayers = 1; msg_draw__6Player(arena);
    CHECK(calls.find("markers") == std::string::npos, "hidden players");
    reset(); gamePlayersPresent = 0; msg_draw__6Player(arena);
    CHECK(calls.find("markers") == std::string::npos, "no players");
    // ambient offsets: bit 3 of +12
    reset(); set_word(12, 8);
    msg_draw__6Player(arena);
    CHECK(calls.find("markers;clear8;amb0,0,0,0;") != std::string::npos, "bit 3");
    // heal colour: raise, lower, clamp
    reset(); set_player_real(1, 180, 1.0f); healColor[1] = 10; healPulse[1] = 0;
    msg_draw__6Player(arena);
    CHECK(healColor[1] == 13 && healPulse[1] == 960 && calls.find("isin960;amb25,0,0,0;") != std::string::npos, "heal colour rises");
    reset(); set_player_real(1, 180, 1.0f); healColor[1] = 60;
    msg_draw__6Player(arena);
    CHECK(healColor[1] == 60, "heal colour stops at 60");
    reset(); set_player_real(1, 180, 0.5f); healColor[1] = 10; healPulse[1] = 5;
    msg_draw__6Player(arena);
    CHECK(healColor[1] == 7 && healPulse[1] == 965, "heal colour falls");
    reset(); set_player_real(1, 180, 0.5f); healColor[1] = 2; healPulse[1] = 5;
    msg_draw__6Player(arena);
    CHECK(healColor[1] == 0 && healPulse[1] == 960 && calls.find("amb12,0,0,0;") != std::string::npos, "heal colour clamps");
    reset(); set_player_real(1, 180, 0.09f); healColor[1] = 2;
    msg_draw__6Player(arena);
    CHECK(healColor[1] == 2 && calls.find("isin960") == std::string::npos && calls.find("amb") == std::string::npos, "at 0.1 nothing");
    // effect words: floats become halfwords
    reset(); set_word(896 + 8, 1); set_real(108, 1.9f); set_real(112, -2.9f); set_real(116, 3.0f); set_real(120, 40000.0f);
    msg_draw__6Player(arena);
    CHECK(calls.find("amb1,-2,3,") != std::string::npos, "effect offsets");
    // halfword at +1188
    reset(); set_half(1188, 100);
    msg_draw__6Player(arena);
    CHECK(calls.find("amb750,750,1500,1500;") != std::string::npos, "scaled offsets");
    reset(); set_half(1188, -3);
    msg_draw__6Player(arena);
    CHECK(calls.find("amb-23,-23,-45,-45;") != std::string::npos, "negative scaled offsets");
    // ship
    reset(); shipOn = 1;
    msg_draw__6Player(arena);
    CHECK(calls.find("make(1,2,8.8),9;rot0,0;lump(ship.lmp,ship.vif);lump(ship.lmp,ship.tex);mdraw128,128,1,(1,2,3),18446744073709551615,0;mreset;lreset;") != std::string::npos, "ship");
    CHECK(calls.find("cdraw") == std::string::npos, "ship replaces the creature draw");
    // size of the attachment draw when far from the camera plane
    reset(); set_global(worldCameraMatrix + 8, 1.0f); set_real(16, 300.0f);
    msg_draw__6Player(arena);
    CHECK(calls.find("attach0,12288,") != std::string::npos, "far attachment");
    reset(); set_global(worldCameraMatrix + 8, 1.0f); set_real(16, 360.0f);
    msg_draw__6Player(arena);
    CHECK(calls.find("attach0,12288,5;") != std::string::npos, "at 360 the size is 5");
    // spell halos
    reset(); spellAlpha[1] = 7; frameNm = 100;
    msg_draw__6Player(arena);
    CHECK(calls.find("gcap4096,(1,2,3),9,288,0,1;gcap4096,(1,2,3),9,288,1,1;isin102400;face(10,20,30),5,8125,5,6,7,7,1911,0,2;isin102400;face(20,20,30),5,8125,5,6,7,7,1911,0,2;") != std::string::npos, "spell halos");
    // name tag
    reset(); netNetworkMode = 1; set_word(12, 0x400000); localPlayer = 0; matW = 2.0f;
    msg_draw__6Player(arena);
    CHECK(calls.find("mtr(1,2,87);lump(icons.lmp,busy.tex);isin") != std::string::npos, "name tag projection");
    CHECK(calls.find("tex128,") != std::string::npos, "name tag drawn");
    { unsigned x = static_cast<unsigned>(static_cast<int>(50.0f * 0.5f) + 293), y = static_cast<unsigned>(static_cast<int>(25.0f) + 166);
      unsigned color = (static_cast<unsigned>(static_cast<int>(0.5f * 16.0f + 112.0f)) << 24) | 0x808080u;
      CHECK(calls.find("tex" + std::to_string(8 * 16) + "," + std::to_string(x) + "," + std::to_string(y) + ",5," + std::to_string(color) + ";") != std::string::npos, "name tag position"); }
    reset(); netNetworkMode = 1; set_word(12, 0x400000); localPlayer = 1;
    msg_draw__6Player(arena);
    CHECK(calls.find("busy") == std::string::npos, "local player has no tag");
    reset(); netNetworkMode = 1; set_word(12, 0);
    msg_draw__6Player(arena);
    CHECK(calls.find("busy") == std::string::npos, "needs the flag bit");
    reset(); netNetworkMode = 1; set_word(12, 0x400000); dlg = 1;
    msg_draw__6Player(arena);
    CHECK(calls.find("busy") == std::string::npos, "dialog hides the tag");
    return 0;
}
