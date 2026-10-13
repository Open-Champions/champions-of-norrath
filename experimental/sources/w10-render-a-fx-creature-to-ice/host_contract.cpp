// Host contract for fxCreatureToIce. Not an EE plan input.
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> events;
static int shadowCount, allocCalls;
static unsigned fakeChunk[200];
extern "C" {
unsigned syncRandState;
unsigned char _vt$13LooseIceChunk[4];
}
struct Point3;
#include "fx_creature_to_ice.cpp"
extern "C" {
int modelGetNmShadowPoints__FP12_modelHeader(void*) { return shadowCount; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void*, void*, Point3 point, short value, int index, Point3* position, float* height, void* matrix, float scale) {
    events.push_back("pos " + std::to_string(point.x) + " " + std::to_string(value) + " " + std::to_string(index) + " " +
                     (matrix ? "m" : "null") + " " + std::to_string(scale));
    position->x = 10.0f * index; position->y = 1.0f; position->z = -0.0f;
    *height = 3.0f;
}
int sprintf(char* buffer, const char* format, ...) {
    va_list args; va_start(args, format);
    int n = std::vsnprintf(buffer, 64, format, args);
    va_end(args);
    return n;
}
void* lumpFindResource__FPcT0(char* lump, char* name) { events.push_back(std::string("lump ") + lump + " " + name); return fakeChunk; }
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void*, Point3 p, float v) { events.push_back("snd " + std::to_string(p.x) + " " + std::to_string(v)); }
float icos__Fi(int a) { return a == 0 ? 1.0f : 0.5f; }
float isin__Fi(int a) { return a == 0 ? 0.0f : 0.25f; }
void* blockAlloc__Fi(int bytes) {
    events.push_back("alloc " + std::to_string(bytes));
    return ++allocCalls == 2 ? nullptr : fakeChunk;
}
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void*, Point3 p, char* a, char* b, char* c, long kind, int flags, void* d, float s) {
    events.push_back("ctor " + std::to_string(p.x) + " " + std::to_string(p.y) + " " + std::to_string(1.0f / p.z < 0 ? -1 : 1) + " " + a + " " + b + " " + c + " " +
                     std::to_string(kind) + " " + std::to_string(flags) + " " + (d ? "d" : "null") + " " + std::to_string(s));
}
}
int main() {
    Point3 here = {7.0f, 8.0f, 9.0f};
    syncRandState = 1;
    // Zero or negative piece count: only the sound plays.
    shadowCount = 4;
    fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff(nullptr, nullptr, here, 0, nullptr, 0.9f, 1.0f, 0.0f);
    if (events.size() != 2 || events[0].rfind("lump fx.lmp expl_ice", 0) != 0 || events[1] != "snd 7.000000 1.000000") { std::printf("%zu\n", events.size()); return 1; }
    if (syncRandState != 1664525u + 1013904223u) return 2;
    events.clear();
    // Step zero (more pieces than shadow points) also returns early.
    shadowCount = 1;
    fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff(nullptr, nullptr, here, 0, nullptr, 3.0f, 1.0f, 0.0f);
    if (events.size() != 2) return 3;
    events.clear();
    // Three pieces over six points: indices 2, 4, 0. The second allocation fails.
    shadowCount = 6;
    fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff(nullptr, nullptr, here, 0, nullptr, 3.0f, 0.5f, 2.0f);
    int positions = 0, ctors = 0;
    for (const auto& e : events) { positions += e.rfind("pos", 0) == 0; ctors += e.rfind("ctor", 0) == 0; }
    if (positions != 3 || ctors != 2) return 4;
    if (events[2] != "pos 7.000000 0 2 null 0.500000") { std::printf("%s\n", events[2].c_str()); return 5; }
    if (events[3].rfind("alloc 584", 0) != 0) return 6;
    bool sawIndex4 = false, sawIndex0 = false;
    for (const auto& e : events) { sawIndex4 |= e == "pos 7.000000 0 4 null 0.500000"; sawIndex0 |= e == "pos 7.000000 0 0 null 0.500000"; }
    if (!sawIndex4 || !sawIndex0) return 7;
    // The push is applied: x = 20 + 1 * 2, y = 1 + 0 * 2, z = -0.0 + 0.0 = +0.0 (first piece).
    if (events[4].rfind("ctor 22.000000 1.000000 1 fx.lmp icechunk.vif icechunk.tex ", 0) != 0) { std::printf("%s\n", events[4].c_str()); return 8; }
    if (events[4].find(" 778 null 1.000000") == std::string::npos) return 10;
    if (fakeChunk[18] != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13LooseIceChunk))) return 9;
    std::puts("fxCreatureToIce contract passed");
    return 0;
}
