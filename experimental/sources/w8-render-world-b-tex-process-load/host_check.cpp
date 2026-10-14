#include <cstdio>
#include <cstring>
#include <vector>
#include <string>

extern "C" {
void* world;
float worldViewCenter[3];
signed char loadInProgress;
int loadKillListSize;
int loadSegmentsLoaded;
signed char reallyLoad __asm__("reallyLoad.873");
unsigned char texturesAreLoading;
unsigned texDecodeStarted;
int decompressThreadid;
int decodeQTail;
int decodeQHead;
signed char loadSlot;
signed char loadCompleteFlag;
short loadPage;
short texmapstart;
short texmapend;
unsigned char texmap[2400];
int texturePageBlock[17];
unsigned char texturePageSegLoc[17 * 128];
signed char texturePageTable[4096];
unsigned texturePageLoc[17];
signed char loadKillList[64];
unsigned char segmentUsed[256];
unsigned char uncompMap[0x2800];
char engineCurrentLevelName[80];
void texProcessLoad__Fi(int requested);
}

alignas(8) static unsigned char arena[0x80000];
unsigned char* norrath_tex_arena = arena;

static int busy, streaming_busy;
static int start_calls, end_calls, load_calls, dump_calls, free_calls, init_calls, term_calls, startthread_calls;
static std::string started_name;
static int started_sector;
static char* load_dst; static char* load_done; static int load_size;
static std::vector<void*> freed, inited;
static std::vector<void*> init_segments;
static std::vector<void*> init_staging;
static void* dumped; static int dumped_size;
static int term_arg, startthread_arg;

extern "C" int cdIsBusy__Fv() { return busy; }
extern "C" int cdIsBusyStreamingNonwithstanding__Fv() { return streaming_busy; }
extern "C" void cdStartStream__FPci(char* n, int s) { ++start_calls; started_name = n; started_sector = s; }
extern "C" void cdEndStream__Fv() { ++end_calls; }
extern "C" void cdStreamLoadAsync__FPcT0i(char* d, char* done, int size) { ++load_calls; load_dst = d; load_done = done; load_size = size; }
extern "C" void dumpMem__FPvi(void* p, int n) { ++dump_calls; dumped = p; dumped_size = n; }
extern "C" void textureFreeTexture__FP8_texture(void* t) { ++free_calls; freed.push_back(t); }
extern "C" void textureInitTexture__FP12_worldHeaderP8_texturePUcPc(void* w, void* t, unsigned char* seg, char* staging) {
    ++init_calls; if (w != world) init_calls += 1000; inited.push_back(t); init_segments.push_back(seg); init_staging.push_back(staging);
}
extern "C" int TerminateThread(int t) { ++term_calls; term_arg = t; return 0; }
extern "C" int StartThread(int t, void* a) { ++startthread_calls; startthread_arg = t; if (a) startthread_arg += 1000; return 0; }

static unsigned& W(int off) { return *reinterpret_cast<unsigned*>(arena + off); }
static float& F(int off) { return *reinterpret_cast<float*>(arena + off); }
static const int kTable = 4096;
static const unsigned kStaging = 0x10000;

static void reset_all() {
    std::memset(arena, 0, sizeof arena);
    std::memset(texmap, 0, sizeof texmap);
    std::memset(texturePageSegLoc, 0xff, sizeof texturePageSegLoc);
    std::memset(texturePageTable, 0xff, sizeof texturePageTable);
    std::memset(segmentUsed, 0, sizeof segmentUsed);
    std::memset(uncompMap, 0, sizeof uncompMap);
    std::memset(loadKillList, 0, sizeof loadKillList);
    std::memset(texturePageLoc, 0, sizeof texturePageLoc);
    for (int& b : texturePageBlock) b = -1;
    world = arena;
    W(88) = 1000; W(92) = 1011; W(96) = 6; W(100) = kTable; F(104) = 10.0f; W(124) = kStaging;
    for (int id = 1000; id <= 1011; ++id) { W(kTable + (id - 1000) * 8) = 5000 + id; W(kTable + (id - 1000) * 8 + 4) = 0x30000; }
    texmapstart = 5000; texmapend = 5599;
    // cell (cy=53,cx=52): neighbours 5251..5253, 5351..5353, 5451..5453 -> blocks 1000..1008.
    const int bases[3] = {5251, 5351, 5451};
    for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) {
        const int cell = bases[r] + c;
        reinterpret_cast<short*>(texmap)[cell - 5000] = static_cast<short>(1000 + r * 3 + c);
    }
    worldViewCenter[0] = 25.0f; worldViewCenter[1] = 35.0f; worldViewCenter[2] = 0.0f;
    loadInProgress = 0; loadKillListSize = 0; loadSegmentsLoaded = 0; reallyLoad = 0; texturesAreLoading = 7;
    texDecodeStarted = 0; decompressThreadid = 77; decodeQTail = 9; decodeQHead = 9;
    loadSlot = 55; loadCompleteFlag = 0; loadPage = 0;
    std::strcpy(engineCurrentLevelName, "level1");
    busy = 0; streaming_busy = 0;
    start_calls = end_calls = load_calls = dump_calls = free_calls = init_calls = term_calls = startthread_calls = 0;
    freed.clear(); inited.clear(); init_segments.clear(); init_staging.clear();
}

int main() {
    // A. null world.
    reset_all(); world = nullptr; loadInProgress = 3;
    texProcessLoad__Fi(-1);
    if (loadInProgress != 3 || texturesAreLoading != 7) return 1;
    // B. state 3 becomes 2; other values do nothing.
    reset_all(); loadInProgress = 3;
    texProcessLoad__Fi(-1);
    if (loadInProgress != 2) return 2;
    loadInProgress = 5; texProcessLoad__Fi(-1);
    if (loadInProgress != 5 || start_calls) return 3;
    // C. busy disc.
    reset_all(); busy = 1;
    texProcessLoad__Fi(-1);
    if (loadInProgress != 0 || texturesAreLoading != 7) return 4;

    // D. empty cache, closest block wins (the distance uses the x cell for y).
    reset_all();
    texProcessLoad__Fi(-1);
    if (loadPage != 1007 || loadInProgress != 3 || loadSlot != 0) return 5;
    if (reallyLoad != 1 || start_calls != 1 || started_name != "level1.tex" || started_sector != 5000 + 1007) return 6;
    if (texturesAreLoading != 1 || decodeQTail != 0 || decodeQHead != 0 || loadKillListSize != 0) return 7;
    // explicit request for a block
    reset_all();
    texProcessLoad__Fi(1010);
    if (loadPage != 1010 || loadInProgress != 3) return 8;
    // zero start word: not a real load
    reset_all();
    W(kTable + (1007 - 1000) * 8) = 0;
    texProcessLoad__Fi(-1);
    if (loadPage != 1007 || reallyLoad != 0 || start_calls != 0 || loadInProgress != 3 || texturesAreLoading != 1) return 9;
    // zero size is not eligible
    reset_all();
    W(kTable + (1007 - 1000) * 8 + 4) = 0;
    texProcessLoad__Fi(-1);
    if (loadPage != 1004) return 10;  // ties keep the earlier cell

    // E. resident pages, eviction by free segments.
    reset_all();
    texturePageBlock[0] = 1007; texturePageBlock[1] = 1004; texturePageBlock[2] = 3000; texturePageBlock[3] = 3001;
    texturePageTable[1007] = 0; texturePageTable[1004] = 1; texturePageTable[3000] = 2; texturePageTable[3001] = 3;
    W(kTable + 2000 * 8 + 4) = 0x20000; W(kTable + 2001 * 8 + 4) = 0x20000;
    for (int i = 0; i < 6; ++i) segmentUsed[i] = 1;
    texDecodeStarted = 0x1234;
    reinterpret_cast<unsigned*>(uncompMap)[5 * 2] = 0x1234;
    reinterpret_cast<unsigned*>(uncompMap)[700 * 2] = 0x1234;
    reinterpret_cast<unsigned*>(uncompMap)[701 * 2] = 0x4321;
    texProcessLoad__Fi(-1);
    if (loadPage != 1006) return 11;
    if (loadKillListSize != 2 || loadKillList[0] != 2 || loadKillList[1] != 3) return 12;
    if (texturePageTable[3000] != -1 || texturePageTable[3001] != -1) return 13;
    if (texturePageTable[1007] != 0 || texturePageTable[1004] != 1) return 14;
    if (loadSlot != 3) return 15;
    if (texDecodeStarted != 0 || reinterpret_cast<unsigned*>(uncompMap)[5 * 2] != 0 ||
        reinterpret_cast<unsigned*>(uncompMap)[700 * 2] != 0 || reinterpret_cast<unsigned*>(uncompMap)[701 * 2] != 0x4321) return 16;
    if (term_calls != 1 || term_arg != 77 || startthread_calls != 1 || startthread_arg != 77) return 17;
    if (loadInProgress != 3) return 18;
    // enough free segments: only one slot is evicted/chosen (the first unflagged: slot 2).
    reset_all();
    texturePageBlock[0] = 1007; texturePageBlock[1] = 1004; texturePageBlock[2] = 3000; texturePageBlock[3] = 3001;
    texturePageTable[3000] = 2; texturePageTable[3001] = 3; texturePageTable[1007] = 0; texturePageTable[1004] = 1;
    W(kTable + 2000 * 8 + 4) = 0x20000;
    texProcessLoad__Fi(-1);
    if (loadSlot != 2 || loadKillListSize != 1 || loadKillList[0] != 2 || texturePageTable[3000] != -1 || texturePageTable[3001] != 3) return 19;
    if (term_calls != 0) return 20;

    // F. everything resident: nothing to do but clear texturesAreLoading.
    reset_all();
    for (int k = 0; k < 9; ++k) texturePageBlock[k] = 1000 + k;
    texProcessLoad__Fi(-1);
    if (texturesAreLoading != 0 || loadInProgress != 0 || start_calls) return 21;

    // G. no slot left: loadSlot gets the previous candidate and the state stays.
    reset_all();
    for (int k = 0; k < 9; ++k) { texturePageBlock[k] = 3000 + k; texturePageTable[3000 + k] = static_cast<signed char>(k); }
    for (int i = 0; i < 6; ++i) segmentUsed[i] = 1;
    texProcessLoad__Fi(-1);
    if (loadInProgress != 0 || loadSlot != 8 || loadKillListSize != 9 || start_calls) return 22;

    // H. state 2 releases evicted pages.
    reset_all();
    loadInProgress = 2; loadKillListSize = 2; loadSegmentsLoaded = 5;
    loadKillList[0] = 2; loadKillList[1] = 4;
    texturePageBlock[2] = 1001; texturePageBlock[4] = 1002;
    texturePageLoc[2] = 0x20000; texturePageLoc[4] = 0x30000;
    *reinterpret_cast<int*>(arena + 0x20000) = 2;
    *reinterpret_cast<int*>(arena + 0x30000) = 0;
    texturePageSegLoc[2 * 128 + 0] = 4; texturePageSegLoc[2 * 128 + 1] = 5;
    texturePageSegLoc[4 * 128 + 0] = 0xff;
    segmentUsed[4] = segmentUsed[5] = segmentUsed[1] = 1;
    texProcessLoad__Fi(-1);
    if (free_calls != 2 || freed[0] != arena + 0x20000 + 64 || freed[1] != arena + 0x20000 + 128) return 23;
    if (segmentUsed[4] != 0 || segmentUsed[5] != 0 || segmentUsed[1] != 1) return 24;
    if (texturePageBlock[2] != -1 || texturePageBlock[4] != -1 || texturePageLoc[2] != 0 || texturePageLoc[4] != 0) return 25;
    if (loadCompleteFlag != 1 || loadInProgress != 1 || loadKillListSize != 0 || loadSegmentsLoaded != 0) return 26;

    // I. state 1.
    reset_all();
    loadInProgress = 1; loadCompleteFlag = 0; reallyLoad = 1; loadPage = 1002; loadSlot = 3; loadSegmentsLoaded = 0;
    texProcessLoad__Fi(-1);
    if (load_calls || loadSegmentsLoaded != 0) return 27;
    loadCompleteFlag = 1; streaming_busy = 1;
    texProcessLoad__Fi(-1);
    if (load_calls || loadSegmentsLoaded != 0) return 28;
    streaming_busy = 0;
    segmentUsed[0] = 1;
    texProcessLoad__Fi(-1);   // size 0x30000, nothing loaded: chunk 0x20000 into segment 1
    if (load_calls != 1 || load_size != 0x20000 || load_dst != reinterpret_cast<char*>(arena + kStaging + (1 << 17))) return 29;
    if (load_done != reinterpret_cast<char*>(&loadCompleteFlag)) return 30;
    if (texturePageSegLoc[3 * 128 + 0] != 1 || segmentUsed[1] != 1 || loadSegmentsLoaded != 1) return 31;
    texProcessLoad__Fi(-1);   // remaining 0x10000 into segment 2
    if (load_calls != 2 || load_size != 0x10000 || texturePageSegLoc[3 * 128 + 1] != 2 || loadSegmentsLoaded != 2) return 32;
    if (end_calls != 0) return 33;
    // finished: page address from the first segment, textures initialised.
    *reinterpret_cast<int*>(arena + kStaging + (1 << 17)) = 2;
    texProcessLoad__Fi(-1);
    if (end_calls != 1) return 34;
    if (texturePageLoc[3] != kStaging + (1u << 17)) return 35;
    if (texturePageSegLoc[3 * 128 + 1] != 2 || texturePageSegLoc[3 * 128 + 2] != 0xff || texturePageSegLoc[3 * 128 + 127] != 0xff) return 36;
    if (loadInProgress != 0 || loadCompleteFlag != 0) return 37;
    if (init_calls != 2 || inited[0] != arena + kStaging + (1 << 17) + 64 || inited[1] != arena + kStaging + (1 << 17) + 128) return 38;
    if (init_segments[0] != texturePageSegLoc + 3 * 128 || init_staging[0] != reinterpret_cast<void*>(arena + kStaging)) return 39;
    if (texturePageBlock[3] != 1002 || texturePageTable[1002] != 3) return 40;
    if (dump_calls != 0) return 41;
    // oversized count triggers a dump
    reset_all();
    loadInProgress = 1; loadCompleteFlag = 1; reallyLoad = 1; loadPage = 1002; loadSlot = 3; loadSegmentsLoaded = 3;
    texturePageSegLoc[3 * 128 + 0] = 0;
    *reinterpret_cast<int*>(arena + kStaging) = 16001;
    texProcessLoad__Fi(-1);
    if (dump_calls != 1 || dumped != arena + kStaging || dumped_size != 1024) return 42;
    // not a real load: count word is cleared instead, nothing initialised
    reset_all();
    loadInProgress = 1; loadCompleteFlag = 1; reallyLoad = 0; loadPage = 1002; loadSlot = 3; loadSegmentsLoaded = 3;
    texturePageSegLoc[3 * 128 + 0] = 0;
    *reinterpret_cast<int*>(arena + kStaging) = 9;
    texProcessLoad__Fi(-1);
    if (*reinterpret_cast<int*>(arena + kStaging) != 0 || init_calls != 0 || end_calls != 0) return 43;
    if (texturePageBlock[3] != 1002 || texturePageTable[1002] != 3 || loadInProgress != 0) return 44;
    return 0;
}
