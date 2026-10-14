// Host contract for init__Fv. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#define sprintf norrath_host_sprintf
#include "select_init.cpp"
#undef sprintf

float norrath_host_rsqrt(float one, float value) { return one / std::sqrt(value); }

extern "C" {
signed char playerSelectCount, playerSelectStartPlayer, playerSelectEnforce, startExitFade, playerSelectSuccessful;
const char* npcLumpName;
char levelName[0x142];
SelectTarget targets[10];
unsigned char* targetObjs[10];
short yaws[10];
Point3 hpos[10], hvecs[10];
float lightMats[10][9];
SelectInfo playerSelectInfo[4];
char nameTags[4][32];
char pnums[4][8];
SelectDummy dummyList[4];
int nmPlayers, needEnforce, curPlayer, requestToExit, engineDisable30fps;
float hermiteTime, headerAlpha, hermDeltaScale, flareDelta, bodyAlpha;
}

static std::vector<std::string> events;
static int needPlayers;
static unsigned char objects[14][128];
static unsigned char blocks[4][1200];
static int findCalls;
static int blockCalls;
static void note(const std::string& s) { events.push_back(s); }

extern "C" {
int norrath_host_sprintf(char* out, const char* format, ...) {
    va_list ap; va_start(ap, format);
    int n = std::vsnprintf(out, 256, format, ap);
    va_end(ap);
    note(std::string("sprintf:") + out);
    return n;
}
unsigned char* objectFindObject__FPc(char* name) {
    note(std::string("find:") + name);
    return objects[findCalls++];
}
int scriptGetGameState__FPc(const char* key) { note(std::string("state:") + key); return needPlayers; }
void* lumpFindResource__FPcT0(const char* lump, char* name) {
    note(std::string("lump:") + lump + ":" + name);
    return &objects[13];
}
void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char* state, void*, int mode) {
    note("animAdd:" + std::to_string(state - objects[0]) + ":" + std::to_string(mode));
}
float icos__Fi(int) { return 1.0f; }
float isin__Fi(int) { return 0.0f; }
void getHairAndSkin__Fi(int i) { note("hairAndSkin:" + std::to_string(i)); }
void changeHairStyle__Fii(int a, int b) { note("style:" + std::to_string(a) + std::to_string(b)); }
void changeHairColor__Fii(int a, int b) { note("color:" + std::to_string(a) + std::to_string(b)); }
void changeBeard__Fii(int a, int b) { note("beard:" + std::to_string(a) + std::to_string(b)); }
void changeSkin__Fii(int a, int b) { note("skin:" + std::to_string(a) + std::to_string(b)); }
void objectRemoveFromGrid__FP10GameObject(unsigned char*) { note("remove"); }
void* blockAlloc__Fi(int size) { note("alloc:" + std::to_string(size)); return blocks[blockCalls++]; }
void __3NPCG6Point3sPcN33ifPPc(void* self, Point3 position, short yaw, const char* lump, char* vif, char* tex,
                               char* anm, int zero, float scale, SelectDummy* dummy) {
    char text[400];
    std::snprintf(text, sizeof text, "npc:%d:%g,%g,%g:%d:%s:%s:%s:%s:%d:%g:%s:%s", int(static_cast<unsigned char*>(self) - blocks[0]) / 1200,
                  position.x, position.y, position.z, yaw, lump, vif, tex, anm, zero, scale, dummy->tag, dummy->number);
    note(text);
}
void objectAddToRunList__FP10GameObject(void*) { note("run"); }
void initPreSelect__Fv() { note("preSelect"); }
void initNamePad__Fv() { note("namePad"); }
void initLightInfo__Fv() { note("lightInfo"); }
}

static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
static void reset() {
    events.clear(); findCalls = 0; blockCalls = 0;
    std::memset(objects, 0, sizeof objects); std::memset(blocks, 0, sizeof blocks);
    static const char* names[10] = {"nm0", "nm1", "nm2", "nm3", "nm4", "nm5", "nm6", "nm7", "nm8", "nm9"};
    for (int i = 0; i < 10; ++i) {
        targets[i].name = names[i];
        float* p = reinterpret_cast<float*>(objects[i] + 16);
        p[0] = 10.f * i; p[1] = 0; p[2] = 5;                       // positions on the x axis
        *reinterpret_cast<unsigned short*>(objects[i] + 56) = 0x1000 + i;
    }
    for (int i = 10; i < 14; ++i) {
        float* p = reinterpret_cast<float*>(objects[i] + 16);
        p[0] = 1.f * i; p[1] = 2; p[2] = 3;
        *reinterpret_cast<short*>(objects[i] + 56) = static_cast<short>(40 + i);
    }
    for (SelectInfo& info : playerSelectInfo) { std::memset(&info, 0, sizeof info); }
    playerSelectInfo[0].byte32 = 3; playerSelectInfo[0].byte35 = 4;
    playerSelectInfo[1].byte32 = 7; playerSelectInfo[1].byte35 = 0;
    std::memset(levelName, 0, sizeof levelName);
}
static bool has(const std::string& e) { for (const auto& x : events) if (x == e) return true; return false; }

int main() {
    // Offline path on the second select level: players are built and the screen state is reset.
    reset(); needPlayers = 0; nmPlayers = 2; playerSelectCount = 0;
    std::strcpy(levelName + 0x51, "kelsel2");
    playerSelectStartPlayer = 2; playerSelectEnforce = 5; playerSelectInfo[2].byte32 = 9;
    hermiteTime = 3; flareDelta = 3; bodyAlpha = 3; requestToExit = 1; startExitFade = 1; playerSelectSuccessful = 1;
    init__Fv();
    if (playerSelectCount != 1) return 1;
    if (std::strcmp(npcLumpName, "select.lmp") != 0) return 2;               // not "kelsel"
    for (int i = 0; i < 10; ++i) {
        if (targetObjs[i] != objects[i]) return 3;
        if (yaws[i] != static_cast<short>(-16384 - (0x1000 + i))) return 4;
        if (!near(hpos[i].x, 10.f * i + 90.f) || !near(hpos[i].y, 0) || !near(hpos[i].z, 5)) return 5;
        // b = (-1,0,0), c = cross(b, up) = (0,1,0)
        const float expect[9] = {0, 1, 0, -1, 0, 0, 0, 0, 1};
        for (int k = 0; k < 9; ++k) if (!near(lightMats[i][k], expect[k])) return 6;
    }
    for (int i = 0; i < 9; ++i) if (!near(hvecs[i].x, 1) || !near(hvecs[i].y, 0) || !near(hvecs[i].z, 0)) return 7;
    if (!near(hvecs[9].x, -1) || !near(hvecs[9].y, 0)) return 8;              // wraps from the last to the first
    if (events[0] != "find:nm0" || events[1] != "state:needPlayers" || events[2] != "hairAndSkin:0") return 9;
    if (has("lump:select.lmp:nm0_intro1.anm")) return 10;                      // only when players are needed
    // Players: names and the constructor arguments, in order, with the 10 removals before them.
    int removes = 0;
    for (const auto& e : events) if (e == "remove") ++removes;
    if (removes != 10) return 11;
    if (!has("sprintf:finalpos1") || !has("sprintf:finalpos2") || has("sprintf:finalpos3")) return 12;
    if (!has("sprintf:nm3_bodyv1.vif") || !has("sprintf:nm3_bodyv5.tex") || !has("sprintf:nm3_intro1.anm")) return 13;
    if (!has("sprintf:nm7_bodyv1.vif") || !has("sprintf:nm7_bodyv1.tex")) return 14;
    if (std::strcmp(nameTags[0], "name=nm3") != 0 || std::strcmp(pnums[0], "pnum=0") != 0 ||
        std::strcmp(nameTags[1], "name=nm7") != 0 || std::strcmp(pnums[1], "pnum=1") != 0) return 15;
    if (dummyList[1].tag != nameTags[1] || dummyList[1].number != pnums[1] || dummyList[1].zero != 0) return 16;
    if (!has("npc:0:10,2,3:50:select.lmp:nm3_bodyv1.vif:nm3_bodyv5.tex:nm3_intro1.anm:0:60:name=nm3:pnum=0")) return 17;
    if (!has("npc:1:11,2,3:51:select.lmp:nm7_bodyv1.vif:nm7_bodyv1.tex:nm7_intro1.anm:0:60:name=nm7:pnum=1")) return 18;
    if (*reinterpret_cast<unsigned long long*>(blocks[0] + 160) != 0x10000000ull ||
        *reinterpret_cast<unsigned long long*>(blocks[1] + 160) != 0x10000000ull) return 19;
    // Reset block of the second select level.
    if (playerSelectInfo[2].byte32 != 0 || headerAlpha != 0 || hermDeltaScale != 1.0f || needEnforce != 5 ||
        curPlayer != 2 || flareDelta != 0 || bodyAlpha != 0 || requestToExit != 0 || startExitFade != 0 ||
        playerSelectSuccessful != 0 || engineDisable30fps != 1 || hermiteTime != 0) return 20;
    if (events[events.size() - 3] != "preSelect" || events[events.size() - 2] != "namePad" ||
        events[events.size() - 1] != "lightInfo") return 21;

    // Other level, players needed: hair/skin resets, intro animations, no removals, no reset block.
    reset(); needPlayers = 1; nmPlayers = 3; playerSelectCount = 2;
    std::strcpy(levelName + 0x51, "kelsel");
    engineDisable30fps = 0; curPlayer = 77;
    init__Fv();
    if (playerSelectCount != 2 || std::strcmp(npcLumpName, "select2.lmp") != 0) return 22;
    if (!has("lump:select2.lmp:nm0_intro1.anm") || !has("animAdd:288:3")) return 23;
    removes = 0;
    int styles = 0;
    for (const auto& e : events) { if (e == "remove") ++removes; if (e.rfind("style:", 0) == 0) ++styles; }
    if (removes != 0 || styles != 10 || !has("style:90") || !has("color:90") || !has("beard:90") || !has("skin:90")) return 24;
    if (has("alloc:1144") || engineDisable30fps != 0 || curPlayer != 77) return 25;

    // No players configured and not the second level: nothing past the vectors.
    reset(); needPlayers = 0; nmPlayers = 0;
    std::strcpy(levelName + 0x51, "zzz");
    init__Fv();
    if (has("alloc:1144") || std::strcmp(npcLumpName, "select.lmp") != 0) return 26;
    return 0;
}
