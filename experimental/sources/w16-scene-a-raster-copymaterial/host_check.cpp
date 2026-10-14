// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 17];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int addResult, addCalls, matrixCalls;
extern "C" int Add__t6VIList1Z10VIMaterial(unsigned char* list) { ++addCalls; return list == arena + 0x400 + 19416 ? addResult : -77; }
extern "C" void* __as__10VIMatrix33RC10VIMatrix33(unsigned char* self, const unsigned char* other) { ++matrixCalls; std::memcpy(self, other, 36); return self; }
#include "copy_material.cpp"
#include <cstdio>
static int& W(unsigned off) { return *reinterpret_cast<int*>(arena + off); }
int main() {
    const unsigned raster = 0x400, table = 0x8000, objects = 0x9000, src = 0x10000, dst = 0x12000;
    W(raster + 19388) = arena_base + table;
    for (int i = 0; i < 4; ++i) { W(table + 12 * i) = arena_base + objects + 0x20 * i; W(objects + 0x20 * i + 4) = 10 * i; }
    for (unsigned i = 0; i < 0x1000; ++i) { arena[src + i] = static_cast<unsigned char>(i * 7 + 1); arena[dst + i] = 0xcd; }
    W(src + 8) = 2; W(src + 48) = 2; W(src + 48 + 144) = -1;
    addResult = arena_base + dst;
    bool ok = CopyMaterial__8VIRasterii(arena + raster, arena_base + src, 1) == static_cast<int>(arena_base + dst);
    ok = ok && addCalls == 1 && matrixCalls == 2 && W(dst + 4) == static_cast<int>(0xcdcdcdcd) + 1 && W(dst + 8) == 2;
    ok = ok && std::memcmp(arena + dst + 12, arena + src + 12, 4) == 0 && std::memcmp(arena + dst + 24, arena + src + 24, 8) == 0 && std::memcmp(arena + dst + 32, arena + src + 32, 4) == 0;
    ok = ok && std::memcmp(arena + dst + 16, arena + src + 16, 8) != 0;   // +16..+23 is not copied
    for (int layer = 0; layer < 2; ++layer) {
        unsigned s = src + 144 * layer, d = dst + 144 * layer;
        ok = ok && std::memcmp(arena + d + 48, arena + s + 48, 16) == 0 && std::memcmp(arena + d + 64, arena + s + 64, 36) == 0;
        ok = ok && std::memcmp(arena + d + 100, arena + s + 100, 4) == 0 && std::memcmp(arena + d + 104, arena + s + 104, 16) == 0 && std::memcmp(arena + d + 120, arena + s + 120, 4) == 0;
        ok = ok && arena[d + 124] == 0xcd && arena[d + 127] == 0xcd;   // +124 is not copied
    }
    ok = ok && std::memcmp(arena + dst + 128 + 144, arena + src + 128 + 144, 64) == 0 && std::memcmp(arena + dst + 128 + 144 + 64, arena + src + 128 + 144 + 64, 4) != 0;  // last layer reaches +191 only
    ok = ok && W(objects + 0x40 + 4) == 20 + 1 && W(objects + 0x20 + 4) == 10 && W(objects + 4) == 0;   // surface id 2 referenced once; -1 skipped
    // flag zero leaves the reference count alone, negative Add result stops everything
    W(dst + 4) = 5; ok = ok && CopyMaterial__8VIRasterii(arena + raster, arena_base + src, 0) == static_cast<int>(arena_base + dst) && W(dst + 4) == 5;
    addCalls = 0; matrixCalls = 0; addResult = -1;
    ok = ok && CopyMaterial__8VIRasterii(arena + raster, arena_base + src, 1) == -1 && matrixCalls == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
