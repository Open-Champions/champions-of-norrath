// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena and maps one
// fake function address to a stub. The point test and the matrix product are stubbed and log their arguments.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
typedef int (*Method)(unsigned char*, unsigned char*, unsigned char*, unsigned char*);
static int methodResult, methodCalls;
static unsigned char *mSelf, *mScene, *mMatrix, *mQuery;
static int stubMethod(unsigned char* self, unsigned char* scene, unsigned char* matrix, unsigned char* query) {
    ++methodCalls; mSelf = self; mScene = scene; mMatrix = matrix; mQuery = query; return methodResult;
}
extern "C" void* norrath_host_function(unsigned address) { return address == 0xf00000u ? reinterpret_cast<void*>(&stubMethod) : nullptr; }
#include "query_emitters.cpp"
#include <cstdio>
static int pisResult, pisCalls, mulCalls;
static const unsigned char *pisSphere, *pisPoint, *mulOut, *mulA, *mulB;
extern "C" int PointInSolid__C8VISphereRC7VIVect3(const unsigned char* s, const unsigned char* p) { ++pisCalls; pisSphere = s; pisPoint = p; return pisResult; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* o, const unsigned char* a, const unsigned char* b) { ++mulCalls; mulOut = o; mulA = a; mulB = b; }
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    // scene at 0 (stamp +1348 needs a 2 KiB object), actor at 0x1000, records at 0x1400, object at 0x1500, table at 0x1600
    unsigned char camera[128] = {};
    unsigned char query[32] = {};
    alignas(16) static unsigned char scene[2048];
    // the scene is a plain host array; its table pointer word still has to hold an arena address
    *reinterpret_cast<unsigned*>(scene + 1348) = 7;
    *reinterpret_cast<unsigned*>(scene + 1004) = arena_base + 0x1400;
    unsigned actorAddr = arena_base + 0x1000;
    W(0x1000 + 128) = 3; W(0x1000 + 64) = 0; W(0x1000 + 96) = 2;
    W(0x1400 + 24) = arena_base + 0x1500;                   // record 2: 12 * 2 = 24
    W(0x1500 + 60) = arena_base + 0x1600;
    *reinterpret_cast<short*>(arena + 0x1600 + 56) = 8;
    W(0x1600 + 60) = 0xf00000u;
    pisResult = 1; methodResult = 0;
    int r = QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery(scene, static_cast<int>(actorAddr), camera, query);
    bool ok = r == 0 && W(0x1000 + 128) == 7 && pisCalls == 1 && pisSphere == arena + 0x1000 + 136 && pisPoint == camera + 28 &&
              *reinterpret_cast<unsigned*>(query + 8) == actorAddr && *reinterpret_cast<unsigned*>(query + 12) == 2 &&
              mulCalls == 1 && mulOut == scene + 1232 && mulA == arena + 0x1000 && mulB == camera + 48 &&
              methodCalls == 1 && mSelf == arena + 0x1500 + 8 && mScene == scene && mMatrix == scene + 1232 && mQuery == query;
    // same frame stamp now: nothing happens
    r = QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery(scene, static_cast<int>(actorAddr), camera, query);
    ok = ok && r == 0 && pisCalls == 1 && methodCalls == 1;
    // a masked flag blocks the query and leaves the stamp alone
    *reinterpret_cast<unsigned*>(scene + 1348) = 8; W(0x1000 + 64) = 0x20;
    ok = ok && QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery(scene, static_cast<int>(actorAddr), camera, query) == 0 && W(0x1000 + 128) == 7 && pisCalls == 1;
    // flag bit 0x01 is not in the mask; a point outside the sphere stops before the method
    W(0x1000 + 64) = 1; pisResult = 0;
    ok = ok && QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery(scene, static_cast<int>(actorAddr), camera, query) == 0 && W(0x1000 + 128) == 8 && pisCalls == 2 && methodCalls == 1;
    // a negative method result is returned as -1
    *reinterpret_cast<unsigned*>(scene + 1348) = 9; pisResult = 1; methodResult = -3;
    ok = ok && QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery(scene, static_cast<int>(actorAddr), camera, query) == -1 && methodCalls == 2;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
