// Host contract for __9LooseFireG6Point3P10GameObject; not an EE input. Stored 32-bit addresses are offsets into an arena
// (NORRATH_HOST_ARENA). Checks the base constructor arguments, the call order, the optional owner link, the stored words
// and floats, and the read-modify-write of the flag word after the run-list call.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(16) static unsigned char arena[0x1000];
static const unsigned base = 0x100000u;
unsigned char vt_LooseFire[8];
unsigned frameNm;
static const unsigned fireAddress = 0x7f0010u;
static std::string calls;
static unsigned char* baseSelf; static Point3 basePoint; static void* baseMap; static char** baseArgs;
static unsigned char* linkList; static void* linkA; static void* linkB; static unsigned listWordAtLink;
static int effectKind; static Point3 effectPoint;
static unsigned char* runSelf; static unsigned flagAtRun, wordAt80AtRun;

extern "C" void* __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* object, Point3 point, void* map, char** args) {
    calls += "base;"; baseSelf = static_cast<unsigned char*>(object); basePoint = point; baseMap = map; baseArgs = args;
    std::memcpy(static_cast<unsigned char*>(object) + 16, &point, 12);   // the base stores the position
    return object;
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(ListHead* list, void* a, void* b) {
    calls += "link;"; linkList = reinterpret_cast<unsigned char*>(list); linkA = a; linkB = b;
    std::memcpy(&listWordAtLink, linkList, 4);
}
extern "C" void addEffect__FiG6Point3(int kind, Point3 point) { calls += "effect;"; effectKind = kind; effectPoint = point; }
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    calls += "run;"; runSelf = static_cast<unsigned char*>(object);
    std::memcpy(&flagAtRun, runSelf + 12, 4); std::memcpy(&wordAt80AtRun, runSelf + 80, 4);
}
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + (a - base); }
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_LooseFire) return fireAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static float flt(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;
    unsigned char* const owner = arena + 0x800;
    const Point3 points[] = {{1.0f, 2.0f, 3.0f}, {-100.5f, 0.0f, 1.0e5f}};
    const unsigned frames[] = {0u, 100u, 0xffffff00u};
    const unsigned flags[] = {0u, 0x420cu, 0xffffffffu, 0x00010000u, 0x1u};
    for (const Point3& point : points) for (unsigned frame : frames) for (unsigned flag : flags) for (int withOwner = 0; withOwner < 2; ++withOwner) {
        std::memset(arena, 0x5a, sizeof arena);
        std::memcpy(object + 12, &flag, 4);
        calls.clear(); frameNm = frame; listWordAtLink = 0xffffffffu; linkList = nullptr;
        __9LooseFireG6Point3P10GameObject(object, point, withOwner ? owner : nullptr);

        if (baseSelf != object || std::memcmp(&basePoint, &point, 12) != 0 || baseMap != nullptr || baseArgs != nullptr) { std::puts("fail: base args"); bad = 1; }
        if (word(object + 72) != fireAddress || word(object + 76) != 0u) { std::puts("fail: table/list"); bad = 1; }
        const std::string want = withOwner ? "base;link;effect;run;" : "base;effect;run;";
        if (calls != want) { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
        if (withOwner && (linkList != object + 76 || linkA != owner || linkB != owner || listWordAtLink != 0u)) { std::puts("fail: link"); bad = 1; }
        if (word(object + 80) != frame + 120u || wordAt80AtRun != frame + 120u) { std::puts("fail: frame word"); bad = 1; }
        if (effectKind != 5 || std::memcmp(&effectPoint, &point, 12) != 0) { std::puts("fail: effect"); bad = 1; }
        if (runSelf != object || flagAtRun != flag) { std::puts("fail: run list"); bad = 1; }
        if (flt(object + 28) != 10.0f || word(object + 12) != (flag | 0x420cu)) { std::puts("fail: final stores"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
