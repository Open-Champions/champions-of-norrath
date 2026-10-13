// Host contract for cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include <cstring>
#include <string>
#include <cstdio>
static unsigned char arena[4096];
unsigned char* norrath_host_base = arena;
#include "cache_equipment.cpp"
static std::string trace;
static CacheFileEntry entry;
static int eqResult = 1, busy = 0;
extern "C" {
int drawFrameNm; unsigned cacheLoadCache; int cacheLoadValid; int cacheLoadDelayTime; signed char cacheCdDone;
char cacheLoadName[64];
int eqFind_calls;
CacheFileEntry* eqFind__FPci(char* name, int flag) {
    trace += std::string("eq:") + name + ":" + std::to_string(flag) + ";";
    return eqResult ? &entry : nullptr;
}
int cdIsBusy__Fv() { return busy; }
void textureFreeTexture__FP8_texture(unsigned t) { trace += "free:" + std::to_string(t) + ";"; }
void textureInitTexture__FP8_texture(unsigned t) { trace += "tex:" + std::to_string(t) + ";"; }
void modelInitModel__FPv(unsigned m) { trace += "model:" + std::to_string(m) + ";"; }
void cdLoadAsync__FPcPvT0ii(const char* f, unsigned b, signed char*, int len, int start) {
    trace += std::string("cd:") + f + ":" + std::to_string(b) + ":" + std::to_string(len) + ":" + std::to_string(start) + ";";
}
}
// sprintf, strcmp and strcpy come from the host C library.
int main() {
    ArmorCache* el = reinterpret_cast<ArmorCache*>(arena + 256);
    ArmorCache* other = reinterpret_cast<ArmorCache*>(arena + 512);
    unsigned model = 7, texture = 7;
    // hit
    el->valid = 1; std::strcpy(el->name, "wolf"); el->buffer = 1000; el->offset = 40;
    char name[] = "wolf";
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(name, el, &model, &texture);
    if (texture != 1000 || model != 1040 || !trace.empty()) return 1;
    // miss with a countdown running
    el->valid = 1; el->delay = 3; char other_name[] = "bear";
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(other_name, el, &model, &texture);
    if (model != 0 || texture != 0 || el->delay != 2 || !trace.empty()) return 2;
    // start a load: found
    el->delay = 0; el->flag84 = 1; el->loaded = 1; el->valid = 1; drawFrameNm = 50;
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(other_name, el, &model, &texture);
    if (trace != "eq:bear:1;" || cacheLoadValid != 1 || cacheLoadDelayTime != 50 || el->loaded != 0 || el->valid != 0 ||
        std::strcmp(cacheLoadName, "bear") != 0 || cacheLoadCache != static_cast<unsigned>(256)) return 3;
    // too soon
    trace.clear(); drawFrameNm = 53;
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(other_name, other, &model, &texture);
    if (!trace.empty()) return 4;
    // issue the read: entry words 4=3, 12=10, half 10=2 -> start 6144, size 14336, length 18432
    drawFrameNm = 54; entry.word4 = 3; entry.word12 = 10; entry.half10 = 2; el->buffer = 5000;
    cacheLoadValid = 1;
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(other_name, other, &model, &texture);
    if (trace != "free:5000;eq:bear:1;cd:wcache.dat:5000:18432:6144;" || el->loaded != 1 || el->offset != 14336) return 5;
    // read still pending
    trace.clear(); cacheCdDone = 0;
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(other_name, other, &model, &texture);
    if (!trace.empty() || cacheLoadCache == 0) return 6;
    // finish
    cacheCdDone = 1;
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(other_name, other, &model, &texture);
    if (trace != "tex:5000;model:19336;" || el->valid != 1 || std::strcmp(el->name, "bear") != 0 || cacheLoadCache != 0) return 7;
    // not found: sets the delay
    trace.clear(); eqResult = 0; other->delay = 0; other->valid = 0;
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(name, other, &model, &texture);
    if (other->delay != 600 || cacheLoadCache != 0) return 8;
    // file choice
    eqResult = 1; el->loaded = 0; el->flag84 = 0; el->flag86 = 1; cacheLoadCache = 256; drawFrameNm = 100; cacheLoadDelayTime = 0;
    trace.clear();
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(name, other, &model, &texture);
    if (trace.find("eqcache2.dat") == std::string::npos) return 9;
    el->loaded = 0; el->flag86 = 0; trace.clear();
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(name, other, &model, &texture);
    if (trace.find("cd:eqcache.dat") == std::string::npos) return 10;
    // busy disc
    el->loaded = 0; trace.clear(); busy = 1;
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(name, other, &model, &texture);
    if (!trace.empty() || el->loaded != 0) return 11;
    return 0;
}
