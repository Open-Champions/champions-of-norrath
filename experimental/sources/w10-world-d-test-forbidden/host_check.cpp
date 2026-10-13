// Host contract for worldTestForbidden__FP12_worldHeaderG6Point3f. Not an EE plan input.
// Build with -DNORRATH_HOST_ARENA; stored pointers are arena offsets.
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_height_arena = arena;
int collisionFrame = 0;
struct _worldHeader;
struct collisionPart;
struct Point3 { float x, y, z; };
static collisionPart* parts[3];
static float seen[4];
static int seen_limit;
extern "C" void makeCollisionList__FP12_worldHeaderffffPP13collisionParti(_worldHeader*, float a, float b, float c, float d, collisionPart** list, int limit) {
    seen[0] = a; seen[1] = b; seen[2] = c; seen[3] = d; seen_limit = limit;
    for (int i = 0; i < 3; ++i) list[i] = parts[i];
}
extern "C" float worldTestForbidden__FP12_worldHeaderG6Point3f(_worldHeader*, Point3, float);
static void put32(unsigned off, int v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, int v) { short s = short(v); std::memcpy(arena + off, &s, 2); }
static void make_part(unsigned part, unsigned grid) {
    put32(part + 12, int(grid)); put16(part + 16, 0);
    put32(part + 20, 0); put32(part + 24, 0); put32(part + 28, 0);
    put16(part + 32, 32767); put16(part + 34, 0);
    put32(grid + 0, 0); put32(grid + 4, 0); put32(grid + 8, 8); put32(grid + 12, 8);
    put16(grid + 18, 100);
    for (int i = 0; i < 64; ++i) put16(grid + 20 + 2 * i, 0);
}
int main() {
    make_part(0x000, 0x400);
    put16(0x400 + 20 + 2 * (3 * 8 + 2), 1);              // cell (2,3) marked
    parts[0] = reinterpret_cast<collisionPart*>(arena + 0x000); parts[1] = nullptr;
    _worldHeader* w = nullptr;
    if (worldTestForbidden__FP12_worldHeaderG6Point3f(w, {8.5f, 12.5f, 0.0f}, 4.0f) != 1.0f) return 1;  // cell (2,3) is within 1 of (2.1, 3.1)
    if (collisionFrame != 1 || seen[0] != 4.5f || seen[1] != 8.5f || seen[2] != 12.5f || seen[3] != 16.5f || seen_limit != 64) return 2;
    if (worldTestForbidden__FP12_worldHeaderG6Point3f(w, {40.0f, 40.0f, 0.0f}, 4.0f) != 0.0f) return 3;  // marked cell too far
    parts[0] = nullptr;
    if (worldTestForbidden__FP12_worldHeaderG6Point3f(w, {8.5f, 12.5f, 0.0f}, 4.0f) != 0.0f || collisionFrame != 3) return 4;
    put16(0x400 + 18, -30000);                            // part is below the height cut-off: skipped
    put32(0x000 + 28, -975000);
    parts[0] = reinterpret_cast<collisionPart*>(arena + 0x000);
    if (worldTestForbidden__FP12_worldHeaderG6Point3f(w, {8.5f, 12.5f, 0.0f}, 4.0f) != 0.0f) return 5;
    std::printf("world test forbidden contract passed\n");
    return 0;
}
