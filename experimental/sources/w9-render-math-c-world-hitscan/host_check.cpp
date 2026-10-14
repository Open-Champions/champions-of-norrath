// Host contract for worldHitscan__FP12_worldHeaderG6Point3T1fi and norrath_world_hitscan.
// Not an EE plan input. Build with -DNORRATH_HOST_HITSCAN; makeCollisionList and
// worldFindPointHeight are recorded stubs.
#include <cmath>
#include <cstdio>
#include <cstring>
alignas(16) unsigned char arena[0x2000];
unsigned char* norrath_hitscan_arena = arena;
int collisionFrame = 0;
static int lists = 0;
static float last_box[4];
static int last_limit = 0;
static unsigned char* parts[2];
static float point_height = 5.0f;
static float seen_point[3];
extern "C" void makeCollisionList__FP12_worldHeaderffffPP13collisionParti(void*, float a, float b, float c, float d, unsigned char** list, int limit) {
    ++lists; last_box[0] = a; last_box[1] = b; last_box[2] = c; last_box[3] = d; last_limit = limit;
    for (int i = 0; i < 2; ++i) list[i] = parts[i];
}
extern "C" float worldFindPointHeight__FP12_worldHeaderG6Point3(void*, const float* p) { std::memcpy(seen_point, p, 12); return point_height; }
extern "C" float worldHitscan__FP12_worldHeaderG6Point3T1fi(void*, const float*, const float*, float, int);
extern "C" float norrath_world_hitscan(void*, const void*, const void*, float, int);
static void put32(unsigned off, int v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, int v) { short s = short(v); std::memcpy(arena + off, &s, 2); }
static void flat_part(int height, unsigned flags) {
    put32(0x0c, 0x400); put16(0x10, int(flags)); put32(0x14, 0); put32(0x18, 0); put32(0x1c, 0);
    put16(0x20, 32767); put16(0x22, 0);
    put32(0x400, 0); put32(0x404, 0); put32(0x408, 16); put32(0x40c, 16); put16(0x412, 1000);
    for (int i = 0; i < 256; ++i) put16(0x414 + 2 * i, height * 16);
}
int main() {
    int world = 0;
    parts[0] = arena; parts[1] = nullptr;
    flat_part(10, 0);
    const float from[3] = {4.0f, 8.0f, 20.0f};
    const float down[3] = {1.0f, 0.0f, -1.0f};
    float r = worldHitscan__FP12_worldHeaderG6Point3T1fi(&world, from, down, 100.0f, 0);
    if (std::fabs(r - 12.0f) > 0.01f) { std::fprintf(stderr, "r=%f\n", r); return 1; }
    if (collisionFrame != 1 || lists != 1 || last_limit != 256) return 2;
    if (last_box[0] != 4.0f || last_box[1] != 8.0f || std::fabs(last_box[2] - 244.0f) > 0.01f || last_box[3] != 8.0f) return 3;
    if (norrath_world_hitscan(&world, from, down, 100.0f, 0) != r) return 4;   // the placeholder name is the same body
    // a ray that never reaches the terrain inside the allowed length
    const float level[3] = {1.0f, 0.0f, 0.0f};
    const float high[3] = {4.0f, 8.0f, 200.0f};
    if (worldHitscan__FP12_worldHeaderG6Point3T1fi(&world, high, level, 20.0f, 0) != -1.0f) return 5;
    // a cell whose low bits match the mode becomes solid, a ray that starts under it hits at once
    const float low[3] = {4.0f, 8.0f, -5.0f};
    const float flat[3] = {1.0f, 0.0f, 0.0f};
    if (!(worldHitscan__FP12_worldHeaderG6Point3T1fi(&world, low, flat, 50.0f, 0) >= 0.0f)) return 6;
    // a non-positive length is rejected
    if (worldHitscan__FP12_worldHeaderG6Point3T1fi(&world, from, down, 0.0f, 0) != -1.0f) return 7;
    // vertical rays use the point height helper
    const float straight[3] = {0.0f, 0.0f, -1.0f};
    r = worldHitscan__FP12_worldHeaderG6Point3T1fi(&world, from, straight, 100.0f, 0);
    if (r != 15.0f || seen_point[2] != 20.0f) return 8;
    const float up[3] = {0.0f, 0.0f, 1.0f};
    if (worldHitscan__FP12_worldHeaderG6Point3T1fi(&world, from, up, 100.0f, 0) != -1.0f) return 9;
    std::printf("world hitscan contract passed\n");
    return 0;
}
