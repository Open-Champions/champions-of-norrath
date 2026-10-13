// Host contract for worldFindHeight__FP12_worldHeaderG6Point3f. Not an EE plan input.
// Build with -DNORRATH_HOST_HEIGHT; grid pointers stored in parts are arena offsets, and
// makeCollisionList is a recorded stub that hands back two parts.
#include <cmath>
#include <cstdio>
#include <cstring>
alignas(16) unsigned char arena[0x4000];
unsigned char* norrath_height_arena = arena;
int collisionFrame = 0;
static float seen[4];
static void* seen_world = nullptr;
static int seen_limit = 0;
static unsigned char* parts[3];
extern "C" void makeCollisionList__FP12_worldHeaderffffPP13collisionParti(void* w, float a, float b, float c, float d, unsigned char** list, int limit) {
    seen_world = w; seen[0] = a; seen[1] = b; seen[2] = c; seen[3] = d; seen_limit = limit;
    for (int i = 0; i < 3; ++i) list[i] = parts[i];
}
extern "C" float worldFindHeight__FP12_worldHeaderG6Point3f(void*, const float*, float);
static void put32(unsigned off, int v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, int v) { short s = short(v); std::memcpy(arena + off, &s, 2); }
static void make_part(unsigned part, unsigned grid, int z, int h18) {
    put32(part + 12, int(grid)); put16(part + 16, 0);
    put32(part + 20, 0); put32(part + 24, 0); put32(part + 28, z);
    put16(part + 32, 32767); put16(part + 34, 0);                  // cos 1, sin 0
    put32(grid + 0, 0); put32(grid + 4, 0); put32(grid + 8, 8); put32(grid + 12, 8);
    put16(grid + 16, -2000); put16(grid + 18, h18);
    for (int i = 0; i < 64; ++i) put16(grid + 20 + 2 * i, 0);
}
static void cell(unsigned grid, int x, int y, int height) { put16(grid + 20 + 2 * (y * 8 + x), height * 16); }
int main() {
    make_part(0x000, 0x400, 5, 1000);
    cell(0x400, 2, 3, 100);
    cell(0x400, 3, 3, 150);
    cell(0x400, 1, 3, 900);        // too far from the lookup (1.125 > 1.0)
    make_part(0x800, 0xc00, 0, 140);   // skipped once the best height is 155
    cell(0xc00, 2, 3, 999);
    make_part(0x1000, 0x1400, 0, 5000);
    cell(0x1400, 5, 5, 4000);      // too far
    parts[0] = arena + 0x000; parts[1] = arena + 0x800; parts[2] = nullptr;
    int world = 0;
    const float p[3] = {8.5f, 12.5f, 0.0f};
    const float h = worldFindHeight__FP12_worldHeaderG6Point3f(&world, p, 4.0f);
    if (h != 155.0f) { std::fprintf(stderr, "h=%f\n", h); return 1; }
    if (collisionFrame != 1 || seen_world != &world || seen_limit != 64) return 2;
    if (seen[0] != 4.5f || seen[1] != 8.5f || seen[2] != 12.5f || seen[3] != 16.5f) return 3;
    parts[0] = nullptr;
    if (worldFindHeight__FP12_worldHeaderG6Point3f(&world, p, 4.0f) != -1000000.0f || collisionFrame != 2) return 4;
    parts[0] = arena + 0x1000; parts[1] = nullptr;
    if (worldFindHeight__FP12_worldHeaderG6Point3f(&world, p, 4.0f) != 0.0f) return 5;   // in-range cells are zero; (5,5) is out of reach
    std::printf("world find height contract passed\n");
    return 0;
}
