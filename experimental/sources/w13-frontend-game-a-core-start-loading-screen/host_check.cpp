// Host check for coreStartLoadingScreen__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -DNORRATH_HOST_CONTRACT -fno-builtin host_check.cpp
// Fails on a wrong random step or lump name, wrong texture name or order, wrong language choice,
// wrong draw arguments or pass count, a wrong texture copy slot, wrong thread-block words, or a
// wrong guard (running flag, existing thread).
#include "core_start_loading_screen.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
signed char coreLoadingScreenRunning;
signed char loadingScreenRequestExit;
signed char language;
int loadFrames;
int loadThreadID;
unsigned eRandState;
int drawSizeOverEnable;
int engineDisplayDisable;
unsigned loadThreadParam[8];
unsigned char loadStack[16];
unsigned norrath_host_gp = 0x4879f0;
}
Texture64 titleloadTexCopy;
Texture64 scrollmapTexCopy[2];
Texture64 blackoutTexCopy;
Texture64 foregrndTexCopy[2];

static std::string trace;
static char lumpBlock[8];
static Texture64 textures[6];
static const char* const names[6] = {nullptr, "scrollmap.tex", "scrollmapb.tex", "blackout.tex", "foregrnd.tex", "foregrndb.tex"};
static const char* seenTitle;
static int created, started_id, started_arg, thread_state_at_create_gp;
static int nextThread = 41;

static void tr(const char* s) { trace += s; trace += ";"; }
extern "C" void* lumpLoad__FPc(const char* name) { tr((std::string("load ") + name).c_str()); return lumpBlock; }
extern "C" void lumpUnload__FPv(void* lump) { tr(lump == lumpBlock ? "unload" : "unload?"); }
extern "C" void* lumpFindResource__FPcT0(char* lump, const char* name) {
    tr((std::string("find ") + lump + " " + name).c_str());  // first argument is the lump file name
    for (int i = 0; i < 6; ++i) {
        const char* want = i == 0 ? seenTitle : names[i];
        if (std::strcmp(want, name) == 0) return &textures[i];
    }
    return nullptr;
}
extern "C" void engineFrameStart__FUib(unsigned color, int flag) {
    char b[48]; std::snprintf(b, sizeof b, "start %x %d", color, flag); tr(b);
}
extern "C" void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color) {
    int idx = -1;
    for (int i = 0; i < 6; ++i) if (texture == &textures[i]) idx = i;
    char b[64]; std::snprintf(b, sizeof b, "draw %d %d %d %d %x", idx, x, y, mode, color); tr(b);
}
extern "C" void engineFrameEnd__Fb(bool flag) { tr(flag ? "end 1" : "end 0"); }
extern "C" void drawListClear__Fv() { tr("clear"); }
extern "C" void loadThreadMain__FPv(void*) {}
extern "C" int CreateThread(void* param) {
    ++created;
    thread_state_at_create_gp = param == loadThreadParam ? 1 : 0;
    tr("create");
    return nextThread;
}
extern "C" void StartThread(int id, int argument) {
    started_id = id; started_arg = argument; tr("startthread");
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

static unsigned lcg(unsigned s) { return s * 0x19660d + 0x3c6ef35f; }

static int run(signed char lang, unsigned seed, int thread) {
    static const char* const titles[5] = {"titleload.tex", "titleloadsp.tex", "titleloadit.tex", "titleloadfr.tex", "titleloadgr.tex"};
    const char* title = lang >= 1 && lang <= 4 ? titles[lang] : titles[0];
    seenTitle = title;
    trace.clear(); created = 0; started_id = -99; started_arg = -99;
    coreLoadingScreenRunning = 0; loadingScreenRequestExit = 7; language = lang;
    eRandState = seed; loadFrames = 55; loadThreadID = thread;
    drawSizeOverEnable = 5; engineDisplayDisable = 0;
    for (int i = 0; i < 6; ++i) for (int j = 0; j < 64; ++j) textures[i].bytes[j] = static_cast<unsigned char>(i * 40 + j + 1);
    std::memset(&titleloadTexCopy, 0, sizeof titleloadTexCopy);
    std::memset(scrollmapTexCopy, 0, sizeof scrollmapTexCopy);
    std::memset(&blackoutTexCopy, 0, sizeof blackoutTexCopy);
    std::memset(foregrndTexCopy, 0, sizeof foregrndTexCopy);
    std::memset(loadThreadParam, 0xaa, sizeof loadThreadParam);

    coreStartLoadingScreen__Fv();

    unsigned next = lcg(seed);
    CHECK(eRandState == next);
    CHECK(loadFrames == 0);
    char expected[2048];
    int n = std::snprintf(expected, sizeof expected, "load load%u.lmp;", (next >> 16) % 5);
    n += std::snprintf(expected + n, sizeof expected - n, "find load%u.lmp %s;find load%u.lmp scrollmap.tex;find load%u.lmp scrollmapb.tex;find load%u.lmp blackout.tex;find load%u.lmp foregrnd.tex;find load%u.lmp foregrndb.tex;", (next >> 16) % 5, title, (next >> 16) % 5, (next >> 16) % 5, (next >> 16) % 5, (next >> 16) % 5, (next >> 16) % 5);
    for (int pass = 0; pass < 3; ++pass) {
        n += std::snprintf(expected + n, sizeof expected - n, "start 80000000 1;");
        for (int i = 0; i < 6; ++i) n += std::snprintf(expected + n, sizeof expected - n, "draw %d 0 0 6 80606060;", i);
        n += std::snprintf(expected + n, sizeof expected - n, "end 0;");
    }
    n += std::snprintf(expected + n, sizeof expected - n, "clear;unload;");
    if (thread == -1) n += std::snprintf(expected + n, sizeof expected - n, "create;");
    n += std::snprintf(expected + n, sizeof expected - n, "startthread;");
    if (trace != expected) std::printf("trace:    %s\nexpected: %s\n", trace.c_str(), expected);
    CHECK(trace == expected);
    CHECK(drawSizeOverEnable == 1 && engineDisplayDisable == 4);
    CHECK(std::memcmp(&titleloadTexCopy, &textures[0], 64) == 0);
    CHECK(std::memcmp(&scrollmapTexCopy[0], &textures[1], 64) == 0);
    CHECK(std::memcmp(&scrollmapTexCopy[1], &textures[2], 64) == 0);
    CHECK(std::memcmp(&blackoutTexCopy, &textures[3], 64) == 0);
    CHECK(std::memcmp(&foregrndTexCopy[0], &textures[4], 64) == 0);
    CHECK(std::memcmp(&foregrndTexCopy[1], &textures[5], 64) == 0);
    CHECK(coreLoadingScreenRunning == 1 && loadingScreenRequestExit == 0);
    if (thread == -1) {
        CHECK(created == 1 && thread_state_at_create_gp == 1);
        CHECK(loadThreadID == nextThread && started_id == nextThread);
        CHECK(loadThreadParam[1] == static_cast<unsigned>(reinterpret_cast<unsigned long>(&loadThreadMain__FPv)));
        CHECK(loadThreadParam[2] == static_cast<unsigned>(reinterpret_cast<unsigned long>(loadStack)));
        CHECK(loadThreadParam[3] == 8192 && loadThreadParam[4] == 0x4879f0 && loadThreadParam[5] == 5);
        CHECK(loadThreadParam[0] == 0xaaaaaaaau && loadThreadParam[6] == 0xaaaaaaaau);
    } else {
        CHECK(created == 0 && loadThreadID == thread && started_id == thread);
        CHECK(loadThreadParam[1] == 0xaaaaaaaau && loadThreadParam[5] == 0xaaaaaaaau);
    }
    CHECK(started_arg == 0);
    return 0;
}

int main() {
    unsigned seeds[] = {0, 1, 0x12345678, 0xffffffffu, 0x80000000u, 0xdeadbeefu, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // add seeds whose random step lands on every lump number, with the top bit both clear and set
    int filled = 6;
    for (unsigned s = 2; filled < 16 && s < 100000; ++s) {
        unsigned r = lcg(s);
        int want = (filled - 6) % 5;
        bool high = (filled - 6) >= 5;
        if ((r >> 16) % 5 == static_cast<unsigned>(want) && ((r >> 31) != 0) == high) seeds[filled++] = s;
    }
    CHECK(filled == 16);
    for (signed char lang : {-1, 0, 1, 2, 3, 4, 5})
        for (unsigned s : seeds)
            for (int thread : {-1, 0, 5}) CHECK(run(lang, s, thread) == 0);
    // all five lump numbers are reachable
    bool seen[5] = {};
    for (unsigned s = 0; s < 200; ++s) seen[(lcg(s) >> 16) % 5] = true;
    for (bool v : seen) CHECK(v);
    // already running: nothing happens
    coreLoadingScreenRunning = 1; trace.clear(); eRandState = 9; loadFrames = 3;
    coreStartLoadingScreen__Fv();
    CHECK(trace.empty() && eRandState == 9 && loadFrames == 3 && coreLoadingScreenRunning == 1);
    coreLoadingScreenRunning = -1;
    coreStartLoadingScreen__Fv();
    CHECK(trace.empty());
    std::puts("core start loading screen passed");
    return 0;
}
