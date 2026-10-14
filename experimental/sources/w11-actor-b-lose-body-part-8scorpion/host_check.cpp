// Host contract for loseBodyPart__8Scorpioni; not an EE input. Includes the source.
#define W11_ACTOR_B_HOST 1
#include "lose_body_part.cpp"
#include <cstring>
#include <initializer_list>
#include <string>
static unsigned char arena[0x8000];
unsigned char* w11_actor_b_host_base = arena;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls;
static unsigned long long chunksFor[2];
static int chunkCalls;
static unsigned char* allocResult;
static std::string num(long long v) { return std::to_string(v); }
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int sel) {
    calls += "chunks(" + num(static_cast<unsigned char*>(m) - arena) + "," + num(sel) + ");";
    return chunksFor[chunkCalls++ & 1];
}
extern "C" void* blockAlloc__Fi(int size) { calls += "alloc(" + num(size) + ");"; return allocResult; }
extern "C" void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 p, short angle, void* model, void* animation,
                                                                           char* a, char* b, char* c, long long chunks, int d, int e, float scale) {
    calls += "ctor(" + num(static_cast<unsigned char*>(self) - arena) + ",";
    calls += std::to_string(p.x) + "/" + std::to_string(p.y) + "/" + std::to_string(p.z) + ",";
    calls += num(angle) + "," + num(static_cast<unsigned char*>(model) - arena) + "," + num(static_cast<unsigned char*>(animation) - arena) + ",";
    calls += num(reinterpret_cast<unsigned char*>(a) - arena) + "," + num(reinterpret_cast<unsigned char*>(b) - arena) + "," +
             num(reinterpret_cast<unsigned char*>(c) - arena) + "," + std::to_string(static_cast<unsigned long long>(chunks)) + "," + num(d) + "," +
             num(e) + "," + std::to_string(scale) + ");";
}
static void setw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
int main() {
    const unsigned self = 0x400, record = 0x2000, chunk = 0x3000;
    for (int part : {0, 1, 5, 31})
    for (unsigned mask : {0u, 1u, 0x80u, 0xffffffffu})
    for (int allocated : {0, 1})
    for (unsigned long long first : {0ull, 0xf0f0f0f0f0f0f0f0ull, 0xffffffffffffffffull})
    for (unsigned long long second : {0ull, 0x00ff00ff00ff00ffull, 0xffffffffffffffffull}) {
        std::memset(arena, 0x11, sizeof arena);
        setw(self + 36, record); setw(self + 244, mask);
        setw(record + 12, 0x5000); setw(record, 0x6000); setw(record + 8, 0x6100); setw(record + 16, 0x6200);
        float pos[3] = {1.5f, -2.5f, 3.25f}; std::memcpy(arena + self + 16, pos, 12);
        short ang = -1234; std::memcpy(arena + self + 56, &ang, 2);
        float grow = 10.0f; std::memcpy(arena + chunk + 452, &grow, 4);
        chunksFor[0] = first; chunksFor[1] = second; chunkCalls = 0; calls.clear();
        allocResult = allocated ? arena + chunk : nullptr;
        unsigned char before[sizeof arena]; std::memcpy(before, arena, sizeof arena);
        loseBodyPart__8Scorpioni(arena + self, part);
        unsigned newMask = mask | (1u << part);
        std::string want = "chunks(20480," + num(static_cast<int>(mask)) + ");chunks(20480," + num(static_cast<int>(newMask)) + ");alloc(616);";
        if (allocated)
            want += "ctor(" + num(chunk) + ",1.500000/-2.500000/3.250000,-1234,20480," + num(self + 288) + ",24576,24832,25088," +
                    std::to_string(first & ~second) + ",14,2,1.000000);";
        if (calls != want) return 1;
        unsigned got; std::memcpy(&got, arena + self + 244, 4);
        if (got != newMask) return 2;
        unsigned long long d; std::memcpy(&d, arena + self + 552, 8);
        if (d != second) return 3;
        float g; std::memcpy(&g, arena + chunk + 452, 4);
        if (g != (allocated ? 12.0f : 10.0f)) return 4;
        for (unsigned i = 0; i < sizeof arena; ++i) {
            bool ok = (i >= self + 244 && i < self + 248) || (i >= self + 552 && i < self + 560) || (i >= chunk + 452 && i < chunk + 456);
            if (!ok && before[i] != arena[i]) return 5;
        }
    }
    return 0;
}
