// Host contract for worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3. Not an EE plan
// input. Build with -DNORRATH_HOST_CLIP; makeCollisionList is a recorded stub.
#include <cmath>
#include <cstdio>
#include <cstring>
alignas(16) unsigned char arena[0x2000];
unsigned char* norrath_clip_arena = arena;
int collisionFrame = 0;
static unsigned char* parts[2];
static float box[4];
static int calls = 0;
extern "C" void makeCollisionList__FP12_worldHeaderffffPP13collisionParti(void*, float a, float b, float c, float d, unsigned char** list, int limit) {
    ++calls; box[0] = a; box[1] = b; box[2] = c; box[3] = d;
    if (limit != 256) list[0] = nullptr;
    for (int i = 0; i < 2; ++i) list[i] = parts[i];
}
extern "C" float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(void*, const float*, const float*, float, int, float*);
static void put32(unsigned off, int v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, int v) { short s = short(v); std::memcpy(arena + off, &s, 2); }
static void part(int wall_value) {
    put32(0x0c, 0x400); put16(0x10, 0); put32(0x14, 0); put32(0x18, 0); put32(0x1c, 0);
    put16(0x20, 32767); put16(0x22, 0);
    put32(0x400, 0); put32(0x404, 0); put32(0x408, 16); put32(0x40c, 16); put16(0x412, 1000);
    for (int i = 0; i < 256; ++i) put16(0x414 + 2 * i, 0);
    put16(0x414 + 2 * (2 * 16 + 8), wall_value);   // cell x = 8, y = 2
}
int main() {
    int world = 0;
    parts[0] = arena; parts[1] = nullptr;
    const float pos[3] = {8.0f, 8.0f, 0.0f};
    const float move[3] = {40.0f, 0.0f, 0.0f};
    float out[3] = {7.0f, 7.0f, 7.0f};
    part(100 * 16);
    float r = worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(&world, pos, move, 4.0f, 0, out);
    if (std::fabs(r - 0.5f) > 1e-3f) { std::fprintf(stderr, "r=%f\n", r); return 1; }
    if (std::fabs(out[0] + 1.0f) > 1e-3f || std::fabs(out[1]) > 1e-3f || out[2] != 0.0f) { std::fprintf(stderr, "out %f %f %f\n", out[0], out[1], out[2]); return 2; }
    if (collisionFrame != 1 || calls != 1) return 3;
    if (box[0] != 4.0f || box[1] != 4.0f || box[2] != 52.0f || box[3] != 12.0f) return 4;
    // no output pointer is needed, and the answer is the same
    if (std::fabs(worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(&world, pos, move, 4.0f, 0, nullptr) - 0.5f) > 1e-3f) return 5;
    // a wall that is too low (below z + 12) is ignored
    part(5 * 16);
    out[0] = out[1] = out[2] = 7.0f;
    if (worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(&world, pos, move, 4.0f, 0, out) != 1.0f || out[0] != 7.0f) return 6;
    // a cell whose flag bit matches the mask is solid at height 32000
    part(1);
    if (worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(&world, pos, move, 4.0f, 0, out) != 1.0f) return 7;
    if (std::fabs(worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(&world, pos, move, 4.0f, 1, out) - 0.5f) > 1e-3f) return 8;
    // nothing listed: free motion
    parts[0] = nullptr;
    if (worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(&world, pos, move, 4.0f, 0, out) != 1.0f) return 9;
    std::printf("world clip motion contract passed\n");
    return 0;
}
