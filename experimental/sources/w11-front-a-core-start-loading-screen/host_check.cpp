// Host check for coreStartLoadingScreen__Fv (source: ../w10-front-a-core-start-loading-screen).
// Build: c++ -std=c++17 -Wall -Wextra -Werror -Wno-deprecated-declarations -fno-builtin host_check.cpp
// Every callee is a stub that appends to a trace; the check compares the whole trace with the
// order given in the source notes (random name, six texture lookups, three frames of six draws,
// slot copies, list clear, unload, thread setup, start). The one EE-only construct, reading
// the global pointer register, is replaced by a fixed value for the host build.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>

#define __asm__
#define volatile(...) (gp = 0x4879f0u)
#include "../w10-front-a-core-start-loading-screen/core_start_loading_screen.cpp"
#undef volatile
#undef __asm__

extern "C" {
signed char coreLoadingScreenRunning, loadingScreenRequestExit, language;
int loadFrames, loadThreadID, drawSizeOverEnable, engineDisplayDisable;
unsigned eRandState;
unsigned loadThreadParam[8];
unsigned char loadStack[16];
}
Texture64 titleloadTexCopy, scrollmapTexCopy[2], blackoutTexCopy, foregrndTexCopy[2];

static std::string trace;
static int lumpToken, createdId = 77;
static Texture64 tex[6];
static const char* names[6] = {nullptr, "scrollmap.tex", "scrollmapb.tex", "blackout.tex", "foregrnd.tex", "foregrndb.tex"};
static int threadStartId = -5, threadStartArg = -5;
static unsigned threadParamSeen[8];
static bool threadSeen;
static std::string firstName;

extern "C" {
int sprintf(char* out, const char* format, ...) {
    std::va_list ap;
    __builtin_va_start(ap, format);
    int n = std::vsnprintf(out, 80, format, ap);
    __builtin_va_end(ap);
    firstName = out;
    return n;
}
void* lumpLoad__FPc(const char* name) { trace += std::string("load ") + name + ";"; return &lumpToken; }
void lumpUnload__FPv(void* lump) { trace += lump == &lumpToken ? "unload;" : "unload?;"; }
void* lumpFindResource__FPcT0(char* lump, const char* name) {
    trace += std::string("find ") + name + (lump == nullptr ? "" : "") + ";";
    if (lump == nullptr) trace += "nolump;";
    for (int i = 0; i < 6; ++i)
        if (i != 0 && std::strcmp(name, names[i]) == 0) return &tex[i];
    return &tex[0];
}
void engineFrameStart__FUib(unsigned color, int flag) {
    trace += "start " + std::to_string(color) + " " + std::to_string(flag) + " d" + std::to_string(drawSizeOverEnable) +
             " e" + std::to_string(engineDisplayDisable) + ";";
}
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color) {
    int idx = -1;
    for (int i = 0; i < 6; ++i) if (texture == &tex[i]) idx = i;
    trace += "draw " + std::to_string(idx) + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(mode) +
             " " + std::to_string(color) + ";";
}
void engineFrameEnd__Fb(bool flag) { trace += flag ? "end 1;" : "end 0;"; }
void drawListClear__Fv() { trace += "clear d" + std::to_string(drawSizeOverEnable) + ";"; }
void loadThreadMain__FPv(void*) {}
int CreateThread(void* param) {
    trace += param == loadThreadParam ? "create;" : "create?;";
    std::memcpy(threadParamSeen, param, sizeof threadParamSeen);
    threadSeen = true;
    return createdId;
}
void StartThread(int id, int argument) { trace += "startthread;"; threadStartId = id; threadStartArg = argument; }
}

static std::string expected(int lumpIndex, const char* title) {
    std::string t = "load load" + std::to_string(lumpIndex) + ".lmp;";
    t += std::string("find ") + title + ";find scrollmap.tex;find scrollmapb.tex;find blackout.tex;find foregrnd.tex;find foregrndb.tex;";
    for (int pass = 0; pass < 3; ++pass) {
        t += "start 2147483648 1 d0 e4;";
        for (int i = 0; i < 6; ++i) t += "draw " + std::to_string(i) + " 0 0 6 2153799776;";
        t += "end 0;";
    }
    t += "clear d0;unload;";
    return t;
}

static int run(unsigned seed, int lang, int threadId) {
    unsigned st = seed * 0x19660du + 0x3c6ef35fu;
    int lumpIndex = static_cast<int>((st >> 16) % 5u);
    const char* titles[] = {"titleload.tex", "titleloadsp.tex", "titleloadit.tex", "titleloadfr.tex", "titleloadgr.tex", "titleload.tex"};
    const char* title = titles[lang >= 1 && lang <= 4 ? lang : 0];
    trace.clear();
    for (int i = 0; i < 6; ++i)
        for (int b = 0; b < 64; ++b) tex[i].bytes[b] = static_cast<unsigned char>(i * 40 + b + 1);
    std::memset(titleloadTexCopy.bytes, 0, 64); std::memset(blackoutTexCopy.bytes, 0, 64);
    std::memset(scrollmapTexCopy, 0, sizeof scrollmapTexCopy); std::memset(foregrndTexCopy, 0, sizeof foregrndTexCopy);
    std::memset(loadThreadParam, 0xab, sizeof loadThreadParam);
    eRandState = seed; language = static_cast<signed char>(lang); loadFrames = 99;
    loadThreadID = threadId; coreLoadingScreenRunning = 0; loadingScreenRequestExit = 1;
    drawSizeOverEnable = 1; engineDisplayDisable = 0; threadSeen = false; threadStartId = threadStartArg = -5;
    coreStartLoadingScreen__Fv();
    std::string want = expected(lumpIndex, title);
    if (threadId == -1) want += "create;";
    want += "startthread;";
    if (trace != want) { std::printf("trace\n%s\nwant\n%s\n", trace.c_str(), want.c_str()); return 1; }
    if (eRandState != st || loadFrames != 0 || drawSizeOverEnable != 1 || engineDisplayDisable != 4) return 2;
    if (!coreLoadingScreenRunning || loadingScreenRequestExit != 0) return 3;
    if (std::memcmp(titleloadTexCopy.bytes, tex[0].bytes, 64) || std::memcmp(&scrollmapTexCopy[0], &tex[1], 64) ||
        std::memcmp(&scrollmapTexCopy[1], &tex[2], 64) || std::memcmp(blackoutTexCopy.bytes, tex[3].bytes, 64) ||
        std::memcmp(&foregrndTexCopy[0], &tex[4], 64) || std::memcmp(&foregrndTexCopy[1], &tex[5], 64)) return 4;
    if (threadId == -1) {
        if (!threadSeen || loadThreadID != createdId) return 5;
        if (threadParamSeen[1] != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(&loadThreadMain__FPv)) ||
            threadParamSeen[2] != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(loadStack)) ||
            threadParamSeen[3] != 8192 || threadParamSeen[4] != 0x4879f0u || threadParamSeen[5] != 5) return 6;
        if (threadParamSeen[0] != 0xabababab || threadParamSeen[6] != 0xabababab) return 7;
        if (threadStartId != createdId) return 8;
    } else {
        if (threadSeen || loadThreadID != threadId || threadStartId != threadId) return 9;
        if (loadThreadParam[1] != 0xabababab) return 10;
    }
    if (threadStartArg != 0) return 11;
    return 0;
}

int main() {
    for (int lang : {0, 1, 2, 3, 4, 5, -1})
        if (int rc = run(12345u + static_cast<unsigned>(lang + 1) * 0x10001u, lang, -1)) return std::printf("lang %d rc %d\n", lang, rc), rc;
    for (unsigned seed : {0u, 1u, 0xffffffffu, 777u, 0x80000000u})
        if (int rc = run(seed, 0, 3)) return std::printf("seed %u rc %d\n", seed, rc), rc;
    // already running: nothing at all happens
    trace.clear();
    eRandState = 5; coreLoadingScreenRunning = 1; loadThreadID = -1;
    coreStartLoadingScreen__Fv();
    if (!trace.empty() || eRandState != 5 || loadThreadID != -1) return 20;
    coreLoadingScreenRunning = -1;  // signed byte: any nonzero value stops it
    coreStartLoadingScreen__Fv();
    if (!trace.empty()) return 21;
    std::puts("core start loading screen passed");
    return 0;
}
