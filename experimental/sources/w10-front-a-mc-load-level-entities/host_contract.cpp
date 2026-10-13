#include "mc_load_level_entities.cpp"

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>

unsigned char ramSlot[800000];
unsigned char loadBuffer[4096];
signed char keepPlayerPositions, gotPlayerStartThisLevel, mcDisableStartupScript;
unsigned char* world;
static std::string events;
static unsigned char worldData[256];
static unsigned char scriptEntry[32], objectsEntry[32];
static bool haveLump, haveScript;
static int lumpToken;
static unsigned char objectsToken, scriptToken;

extern "C" {
void* lumpLoad__FPc(const char* name) {
    events += std::string("load ") + name + ";";
    return haveLump ? &lumpToken : nullptr;
}
unsigned char* lumpFind__FPvPc(void*, const char* name) {
    events += std::string("find ") + name + ";";
    if (std::strcmp(name, "objects.ob") == 0) return objectsEntry;
    return haveScript ? scriptEntry : nullptr;
}
void scriptSetScript__FPv(void* s) { events += s == &scriptToken ? "script tok;" : s == nullptr ? "script 0;" : "script ?;"; }
void scriptStart__FPc(const char* n) { events += std::string("start ") + n + ";"; }
void readMap__3mapPUc(unsigned char* d) { events += "map " + std::to_string(d - ramSlot) + ";"; }
void objectPutSaveData__FPci(void* d, int f) {
    events += std::string("put ") + (d == loadBuffer ? "buf" : d == &objectsToken ? "obj" : "?") + " " + std::to_string(f) + ";";
}
void floorPutSaveInfo__FPc(void* d) { events += "floor " + std::to_string(static_cast<unsigned char*>(d) - loadBuffer) + ";"; }
void megatileEntitize__FPci(void* d, int f) { events += std::string("mega ") + (d == &objectsToken ? "obj" : "?") + " " + std::to_string(f) + ";"; }
int sprintf(char* out, const char* format, ...) {
    std::va_list ap;
    __builtin_va_start(ap, format);
    int n = std::vsnprintf(out, 80, format, ap);
    __builtin_va_end(ap);
    return n;
}
}

static void setEntry(int i, const char* name, int size) {
    unsigned char* e = ramSlot + 0x1b610 + 24 * i;
    std::strcpy(reinterpret_cast<char*>(e), name);
    std::memcpy(e + 20, &size, 4);
}

int main() {
    world = worldData;
    void* o = &objectsToken;
    std::memcpy(objectsEntry + 4, &o, sizeof o);
    void* s = &scriptToken;
    std::memcpy(scriptEntry + 4, &s, sizeof s);
    int eight = 1;
    std::memcpy(scriptEntry + 8, &eight, 4);
    // table: "alpha" (size 5), "beta" (size 7), then empty. Blocks follow.
    setEntry(0, "alpha", 5);
    setEntry(1, "beta", 7);
    unsigned char* b = ramSlot + 0x1b898 + 5;  // block of "beta"
    int s1 = 3, s2 = 4;
    std::memcpy(b, &s1, 4);
    for (int i = 0; i < 3; ++i) b[8 + i] = static_cast<unsigned char>(10 + i);
    std::memcpy(b + 8 + 3, &s2, 4);
    for (int i = 0; i < 4; ++i) b[16 + 3 + i] = static_cast<unsigned char>(20 + i);
    haveLump = true; haveScript = true;
    keepPlayerPositions = 1;
    char name[] = "lev", full[] = "beta";
    MC_LoadLevelEntities__FPcT0(name, full);
    std::string expect = "load lev.lmp;find script.scr;find script.scr;find script.scr;script tok;start preStart;map " +
                         std::to_string(0x1b898 + 5 + 16 + 3 + 4) + ";put buf 0;floor 3;find objects.ob;put obj 1;start start;";
    if (events != expect) { std::puts(events.c_str()); return 1; }
    if (loadBuffer[0] != 10 || loadBuffer[2] != 12 || loadBuffer[3] != 20 || loadBuffer[6] != 23) return 2;
    if (keepPlayerPositions != 0 || gotPlayerStartThisLevel != 3) return 3;
    // not found: no restore, no lump script, megatile route, disabled startup scripts
    events.clear();
    haveScript = false; mcDisableStartupScript = 1;
    int one = 1;
    std::memcpy(worldData + 112, &one, 4);
    char other[] = "zeta";
    MC_LoadLevelEntities__FPcT0(name, other);
    if (events != "load lev.lmp;find script.scr;script 0;find objects.ob;mega obj 0;") { std::puts(events.c_str()); return 4; }
    if (gotPlayerStartThisLevel != 0) return 5;
    // no lump
    events.clear();
    haveLump = false; mcDisableStartupScript = 0;
    MC_LoadLevelEntities__FPcT0(name, full);
    if (events.find("script 0;") == std::string::npos || events.find("find objects.ob") != std::string::npos) return 6;
    // script entry present but its word at 8 is zero
    events.clear();
    haveLump = true; haveScript = true; eight = 0;
    std::memcpy(scriptEntry + 8, &eight, 4);
    MC_LoadLevelEntities__FPcT0(name, other);
    if (events.find("find script.scr;find script.scr;script 0;") == std::string::npos) return 7;
    std::puts("mc load level entities passed");
    return 0;
}
