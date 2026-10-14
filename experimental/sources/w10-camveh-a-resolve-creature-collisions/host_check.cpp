// Host contract for resolveCreatureCollisions__7Vehicle. Not an EE plan input.
#include <vector>
// Pointers inside the modelled game structures are 32-bit handles into this table.
static std::vector<void*>& handles() { static std::vector<void*> v{nullptr}; return v; }
static unsigned handleFor(void* p) {
    if (p == nullptr) return 0;
    for (size_t i = 1; i < handles().size(); ++i) if (handles()[i] == p) return static_cast<unsigned>(i);
    handles().push_back(p);
    return static_cast<unsigned>(handles().size() - 1);
}
static void* pointerFor(unsigned h) { return handles()[h]; }
#define LOAD_PTR(p, off) (static_cast<unsigned char*>(pointerFor(*reinterpret_cast<unsigned*>((p) + (off)))))
#define LOAD_CALL(p) (reinterpret_cast<VirtualCall>(pointerFor(*reinterpret_cast<unsigned*>(p))))
#include "resolve_creature_collisions.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

int objectIgnoreFlags;
unsigned frameNm;

static std::vector<unsigned char*> updates;
void objectUpdateInGrid__FP10GameObject(void* o) { updates.push_back(static_cast<unsigned char*>(o)); }

static std::vector<unsigned char*> candidates;
static int findCalls, flagsSeenInFind;
static float findLow[3], findHigh[3];
int objectFindInBox__FG6Point3T0PP10GameObjecti(const float* lo, const float* hi, unsigned char** out, int limit) {
    ++findCalls; flagsSeenInFind = objectIgnoreFlags;
    std::memcpy(findLow, lo, 12); std::memcpy(findHigh, hi, 12);
    if (limit != 64) std::fprintf(stderr, "limit\n");
    size_t k = 0;
    for (; k < candidates.size(); ++k) out[k] = candidates[k];
    out[k] = nullptr;
    return static_cast<int>(k);
}

static unsigned char nodes[8][32];
static int nodeCount;
static std::vector<unsigned char*> linked;
void* listOnewayLink__FP8ListHeadT0Pv(void* head, void* second, void* value) {
    if (second != value) std::fprintf(stderr, "second argument\n");
    unsigned char* node = nodes[nodeCount++];
    std::memcpy(node, head, 4);
    const unsigned h = handleFor(value);
    std::memcpy(node + 12, &h, 4);
    const unsigned n = handleFor(node);
    std::memcpy(head, &n, 4);
    linked.push_back(static_cast<unsigned char*>(value));
    return node;
}

struct Move { unsigned char* pos; float delta[3]; float radius; int mode; float* normal; };
static std::vector<Move> moves;
int objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(float* p, const float* d, float radius, int mode, float* normal) {
    Move m; m.pos = reinterpret_cast<unsigned char*>(p); std::memcpy(m.delta, d, 12); m.radius = radius; m.mode = mode; m.normal = normal;
    moves.push_back(m);
    return 0;
}

static float velocity[3];
static int gpvCalls;
void getParticleVelocity__7VehicleP6Point3(float* out, void*, float*) { ++gpvCalls; std::memcpy(out, velocity, 12); out[2] = 99.0f; }
static float atanArgs[2];
int iatan2__Fff(float a, float b) { atanArgs[0] = a; atanArgs[1] = b; return 70000; }
static int destroyed; static void* destroyedHead;
void listDestroy__FP8ListHead(void* head) { ++destroyed; destroyedHead = head; }

struct VCall { void* self; float amount; int a, b; };
static std::vector<VCall> vcalls;
static void fakeMethod(void* self, float amount, int a, int b) { vcalls.push_back({self, amount, a, b}); }

static int failures = 0;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }

alignas(8) static unsigned char vehicle[2304];
alignas(8) static unsigned char objects[2][1400];
alignas(8) static unsigned char others[3][1400];
alignas(8) static unsigned char vtable[32];
static void setw(unsigned char* p, int off, int v) { std::memcpy(p + off, &v, 4); }
static void setf(unsigned char* p, int off, float v) { std::memcpy(p + off, &v, 4); }
static float getf(const unsigned char* p, int off) { float v; std::memcpy(&v, p + off, 4); return v; }
static int getw(const unsigned char* p, int off) { int v; std::memcpy(&v, p + off, 4); return v; }
static void setp(unsigned char* p, int off, void* q) { unsigned h = handleFor(q); std::memcpy(p + off, &h, 4); }

static void reset() {
    std::memset(vehicle, 0, sizeof vehicle);
    std::memset(objects, 0, sizeof objects);
    std::memset(others, 0, sizeof others);
    std::memset(nodes, 0, sizeof nodes);
    updates.clear(); candidates.clear(); moves.clear(); vcalls.clear(); linked.clear();
    nodeCount = 0; findCalls = 0; gpvCalls = 0; destroyed = 0; objectIgnoreFlags = 7; frameNm = 0;
    velocity[0] = velocity[1] = 0; velocity[2] = 0;
    setf(vehicle, 16, 0.5f); setf(vehicle, 20, 0.25f); setf(vehicle, 24, 2.0f);
    setf(vehicle, 1128, 1.0f); setf(vehicle, 1132, 2.0f); setf(vehicle, 1136, 3.0f);
    setw(vehicle, 2140, 1);
    setp(vehicle, 2104, objects[0]);
    setf(objects[0], 16, 10.0f); setf(objects[0], 20, 10.0f); setf(objects[0], 24, 10.0f);
    setf(objects[0], 28, 2.0f); setf(objects[0], 80, 4.0f);
    std::memset(vtable, 0, sizeof vtable);
    short adj = 8; std::memcpy(vtable + 16, &adj, 2);
    const unsigned fn = handleFor(reinterpret_cast<void*>(fakeMethod));
    std::memcpy(vtable + 20, &fn, 4);
    for (int k = 0; k < 3; ++k) {
        setf(others[k], 16, 1.0f + 0.5f * k); setf(others[k], 20, 0.25f + 0.5f * k); setf(others[k], 28, 1.5f);
        others[k][40] = 4;
        setp(others[k], 72, vtable);
    }
}

int main() {
    // Stage 1 moves the attached objects relative to the vehicle.
    reset();
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(getf(objects[0], 16) == 0.5f + 1.0f * 4.0f && getf(objects[0], 20) == 0.25f + 2.0f * 4.0f && getf(objects[0], 24) == 2.0f + 3.0f * 4.0f, "attached object position");
    expect(updates.size() == 1 && updates[0] == objects[0], "grid update");
    expect(findCalls == 1 && flagsSeenInFind == 1 && objectIgnoreFlags == 7, "ignore flags set during the search then restored");
    expect(findLow[0] == 4.5f - 2.0f && findHigh[2] == 14.0f + 2.0f && findHigh[1] == 8.25f + 2.0f, "search box");
    expect(destroyed == 1 && destroyedHead == vehicle + 2136, "list destroyed");

    // Null entries are skipped.
    reset(); setw(vehicle, 2140, 2); setp(vehicle, 2104, nullptr); setp(vehicle, 2108, objects[1]); setf(objects[1], 28, 1.0f);
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(updates.size() == 1 && updates[0] == objects[1] && findCalls == 1, "null entry skipped");

    // A candidate overlapping object 0 is pushed and linked.
    reset(); setf(objects[0], 28, 2.0f);
    // object 0 ends at (4.5, 8.25); candidate at (4.5 - 1, 8.25) distance 1 < 2 + 1.5.
    setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f);
    candidates = {&others[0][0]};
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(linked.size() == 1 && linked[0] == others[0], "candidate linked");
    expect(moves.size() == 1 && moves[0].pos == others[0] + 16 && moves[0].radius == 2.0f && moves[0].mode == 1 && moves[0].normal == nullptr, "move arguments");
    // dx = -1, dy = 0, dist = 1, ratio = (2 + 1.5 - 1) / 1 = 2.5
    expect(moves[0].delta[0] == -2.5f && std::fabs(moves[0].delta[1]) < 1e-6f && moves[0].delta[2] == 0.0f, "push vector");
    expect(updates.size() == 2 && updates[1] == others[0], "pushed object grid update");

    // Skips: too far, own object, flag 0x40 with a 1328 pointer, flag 0x200, same as the attached object.
    reset(); setf(others[0], 16, 40.0f); candidates = {others[0]};
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(linked.empty() && moves.empty(), "too far");
    reset(); setf(others[0], 16, 4.5f); setf(others[0], 20, 8.25f);   // dist 0, linked but not moved
    candidates = {others[0]};
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(linked.size() == 1 && moves.empty(), "exactly on top is linked but not pushed");
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); setw(others[0], 12, 0x40); setp(others[0], 1328, others[1]); candidates = {others[0]};
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(linked.empty(), "0x40 with a 1328 pointer skipped");
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); setw(others[0], 12, 0x40); candidates = {others[0]};
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(linked.size() == 1, "0x40 without a 1328 pointer is considered");
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); setw(others[0], 12, 0x200); candidates = {others[0]};
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(linked.size() == 1 && moves.empty(), "0x200 is linked but not pushed");
    reset(); candidates = {objects[0]};
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(linked.empty(), "the object itself is skipped");
    reset(); setw(vehicle, 2140, 2); setp(vehicle, 2108, objects[1]); setf(objects[1], 16, 100.0f); setf(objects[1], 28, 1.0f);
    setf(objects[0], 28, 2.0f);
    candidates = {objects[1]};
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(linked.empty(), "other attached objects are skipped");
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); setf(others[1], 16, 4.0f); setf(others[1], 20, 8.25f);
    candidates = {others[0], others[0], others[1]};
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(linked.size() == 2 && linked[0] == others[0] && linked[1] == others[1], "a candidate already in the list is not linked twice");

    // Stage 3: reactions of linked creatures.
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); candidates = {others[0]};
    velocity[0] = 10.0f; velocity[1] = 0.0f;       // outward from the vehicle? creature is at (3.5, 8.25), vehicle (0.5, 0.25)
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(gpvCalls == 1, "velocity queried once for the linked creature");
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); candidates = {others[0]}; setw(others[0], 12, 0);
    velocity[0] = 6.0f; velocity[1] = 8.0f;
    resolveCreatureCollisions__7Vehicle(vehicle);
    {
        const float nx = 3.0f, ny = 8.0f, nl = std::sqrt(nx * nx + ny * ny), dot = (nx / nl) * 0.6f + (ny / nl) * 0.8f;
        const float speed = 10.0f * dot;
        expect(vcalls.size() == 1 && vcalls[0].self == others[0] + 8 && vcalls[0].a == 0 && vcalls[0].b == 0, "virtual call target and arguments");
        expect(std::fabs(vcalls[0].amount - static_cast<float>(static_cast<int>((speed - 4.0f) * 256.0f) / 4)) < 1.0f, "virtual call amount");
    }
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); candidates = {others[0]}; setw(others[0], 12, 0x20);
    velocity[0] = 6.0f; velocity[1] = 8.0f;
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(getf(others[0], 756) > 0.0f && vcalls.size() == 1, "0x20 stores the strength");
    {
        short a; std::memcpy(&a, others[0] + 748, 2);
        expect(a == static_cast<short>(70000) && atanArgs[0] > 0.9f, "0x20 stores iatan2 of (ny, nx)");
    }
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); candidates = {others[0]}; setw(others[0], 12, 0x20);
    velocity[0] = -6.0f; velocity[1] = -8.0f;
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(vcalls.empty() && getf(others[0], 756) == 0.0f, "velocity away from the vehicle: dot <= 0 does nothing");
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); candidates = {others[0]};
    velocity[0] = 0.3f; velocity[1] = 0.4f;
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(vcalls.empty(), "speed below 4.0 does nothing");
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); candidates = {others[0]}; frameNm = 3;
    velocity[0] = 6.0f; velocity[1] = 8.0f;
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(vcalls.empty(), "(frameNm & 7) != 0 skips the virtual call");
    reset(); setf(others[0], 16, 3.5f); setf(others[0], 20, 8.25f); candidates = {others[0]}; setw(others[0], 12, 0x40); setp(others[0], 1328, nullptr);
    velocity[0] = 6.0f; velocity[1] = 8.0f;
    resolveCreatureCollisions__7Vehicle(vehicle);
    expect(vcalls.empty(), "0x40 skips the virtual call");
    return failures ? 1 : 0;
}
