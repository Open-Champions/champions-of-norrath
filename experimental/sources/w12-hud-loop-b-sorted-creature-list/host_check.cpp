// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// Objects, list nodes and the filter creature are byte arrays with the observed field offsets; the
// pointer-sized slots (node next pointer, node creature at +12, creature word at +72) hold native
// pointers on the host. distanceToPlane2 is stubbed as a plain dot product so that cone and heading
// tests have a checkable meaning; icos/isin use 65536 units per turn.
#include "sorted_creature_list.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct Blob { alignas(8) unsigned char b[256]; };
static unsigned char* nextOf(unsigned char* n) { unsigned char* p; std::memcpy(&p, n, sizeof p); return p; }
static std::vector<Blob*> world;
static Blob* filterCreature;
static std::string trace;
static int filterRejectIndex = -1;
static int boxCalls;

static void putFloat(Blob& o, int off, float v) { std::memcpy(o.b + off, &v, 4); }
static void putPtr(Blob& o, int off, const void* p) { std::memcpy(o.b + off, &p, sizeof p); }
template <class T> static T getPtr(const Blob& o, int off) { T p; std::memcpy(&p, o.b + off, sizeof p); return p; }
static Blob* makeObject(unsigned flags, float x, float y, float z) {
    Blob* o = new Blob();
    std::memset(o->b, 0, sizeof o->b);
    std::memcpy(o->b + 12, &flags, 4);
    putFloat(*o, 16, x); putFloat(*o, 20, y); putFloat(*o, 24, z);
    return o;
}

extern "C" {
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int max) {
    ++boxCalls;
    trace += "box(" + std::to_string((int)low.x) + "," + std::to_string((int)low.y) + "," + std::to_string((int)low.z) + ";" +
             std::to_string((int)high.x) + "," + std::to_string((int)high.y) + "," + std::to_string((int)high.z) + ";" + std::to_string(max) + ")";
    for (size_t i = 0; i < world.size(); ++i) out[i] = world[i]->b;
    out[world.size()] = nullptr;
    return static_cast<int>(world.size());
}
float icos__Fi(int a) { trace += "c" + std::to_string(a) + ";"; return std::cos(a * 6.283185307179586 / 65536.0); }
float isin__Fi(int a) { trace += "s" + std::to_string(a) + ";"; return std::sin(a * 6.283185307179586 / 65536.0); }
float distanceToPlane2__FP6Point3P6Point2T0(Point3* origin, Point2* dir, Point3* target) {
    return dir->x * (target->x - origin->x) + dir->y * (target->y - origin->y);
}
void listInsert__FP8ListHeadP10__listNodePv(void* list, void* node, void* item) {
    Blob* head = static_cast<Blob*>(list);
    Blob* fresh = new Blob();
    std::memset(fresh->b, 0, sizeof fresh->b);
    putPtr(*fresh, 12, item);
    putPtr(*fresh, 0, node);
    unsigned char* cur = getPtr<unsigned char*>(*head, 0);
    unsigned char* prev = nullptr;
    while (cur != nullptr && cur != node) { prev = cur; cur = nextOf(cur); }
    if (prev == nullptr) putPtr(*head, 0, fresh->b);
    else { unsigned char* f = fresh->b; std::memcpy(prev, &f, sizeof f); }
}
}
static int filterFn(void* self, void* object) {
    trace += "F" + std::to_string(static_cast<unsigned char*>(self) - filterCreature->b) + ";";
    return object != (filterRejectIndex >= 0 ? static_cast<void*>(world[filterRejectIndex]->b) : nullptr);
}
struct VT { alignas(8) unsigned char b[256]; };

static std::string order(Blob& head) {
    std::string s;
    unsigned char* n = getPtr<unsigned char*>(head, 0);
    while (n != nullptr) {
        unsigned char* c;
        std::memcpy(&c, n + 12, sizeof c);
        int idx = -1;
        for (size_t i = 0; i < world.size(); ++i) if (world[i]->b == c) idx = static_cast<int>(i);
        s += idx >= 0 ? std::string(1, static_cast<char>('A' + idx)) : "?";
        n = nextOf(n);
    }
    return s;
}
static int bad;
static void expect(const char* what, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, got.c_str(), want.c_str()); bad = 1; }
}

int main() {
    Blob head; std::memset(head.b, 0, sizeof head.b);
    // A..F: A(5,0) B(8,0) C(flag 1) D(no 0x20) E(-3,0) F(2,0).
    world = {makeObject(0x20, 5, 0, 0), makeObject(0x20, 8, 0, 0), makeObject(0x21, 4, 0, 0), makeObject(0, 4, 0, 0),
             makeObject(0x20, -3, 0, 0), makeObject(0x20, 2, 0, 0)};
    Point3 center = {0, 0, 0};
    trace.clear();
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, 0, 10.0f, -1.0f, &head, nullptr);
    expect("order", order(head), "BAF");
    expect("trace no cone", trace, "box(-10,-10,-10;10,10,10;512)c0;s0;");
    // Bits 0x4 and 0x8 also exclude; bit 0x2 and 0x10 do not.
    world = {makeObject(0x24, 5, 0, 0), makeObject(0x28, 6, 0, 0), makeObject(0x22, 7, 0, 0), makeObject(0x30, 3, 0, 0), makeObject(0x2c, 9, 0, 0)};
    std::memset(head.b, 0, sizeof head.b);
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, 0, 10.0f, -1.0f, &head, nullptr);
    expect("flag bits", order(head), "CD");
    // Cone 90: left/right headings -8192 / 8192; the point (5,6) is outside, (5,0) and (5,4) inside.
    world = {makeObject(0x20, 5, 0, 0), makeObject(0x20, 5, 6, 0), makeObject(0x20, 5, 4, 0), makeObject(0x20, 5, -6, 0)};
    std::memset(head.b, 0, sizeof head.b);
    trace.clear();
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, 0, 10.0f, 90.0f, &head, nullptr);
    expect("cone order", order(head), "CA");
    expect("cone trace", trace, "box(-10,-10,-10;10,10,10;512)c0;s0;c-8192;s-8192;c8192;s8192;");
    // The same objects with cone -1.0 keep all four that are in front.
    std::memset(head.b, 0, sizeof head.b);
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, 0, 10.0f, -1.0f, &head, nullptr);
    expect("no cone order", order(head), "DBCA");
    // Rounding of the half-cone step: cone 1.0 gives 91, -0.5 gives -46, 0 gives 0; the heading wraps as a short.
    world.clear();
    trace.clear();
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, 100, 1.0f, 1.0f, &head, nullptr);
    expect("step 91", trace, "box(-1,-1,-1;1,1,1;512)c100;s100;c-16193;s-16193;c16393;s16393;");
    trace.clear();
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, 100, 1.0f, -0.5f, &head, nullptr);
    expect("step -46", trace, "box(-1,-1,-1;1,1,1;512)c100;s100;c-16330;s-16330;c16530;s16530;");
    trace.clear();
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, 32767, 1.0f, 0.0f, &head, nullptr);
    expect("wrap", trace, "box(-1,-1,-1;1,1,1;512)c32767;s32767;c16383;s16383;c-16385;s-16385;");
    // Filter creature: entry at vtable + 160 holds delta 8 and the function; B is rejected, the call gets this + 8.
    world = {makeObject(0x20, 5, 0, 0), makeObject(0x20, 8, 0, 0), makeObject(0x20, 2, 0, 0)};
    filterCreature = new Blob(); std::memset(filterCreature->b, 0, sizeof filterCreature->b);
    VT* vt = new VT(); std::memset(vt->b, 0, sizeof vt->b);
    // The entry is the source's CallEntry: on the EE the function pointer sits at +4, on a 64-bit host the
    // struct is padded, so build it from the same type.
    { const CallEntry entry = {8, 0, filterFn}; std::memcpy(vt->b + 160, &entry, sizeof entry); }
    putPtr(*filterCreature, 72, vt->b);
    filterRejectIndex = 1;
    std::memset(head.b, 0, sizeof head.b);
    trace.clear();
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, 0, 10.0f, -1.0f, &head, filterCreature);
    expect("filter order", order(head), "AC");
    expect("filter trace", trace, "box(-10,-10,-10;10,10,10;512)c0;s0;F8;F8;F8;");
    // Insertion into an existing list: distances 100, 25, 9 (x = 10, 5, 3); candidate at 5 goes before the
    // equal node; candidate at 4 goes between 25 and 9; candidate at 11 goes first; candidate at 1 goes last.
    for (float x : {5.0f, 4.0f, 11.0f, 1.0f}) {
        Blob n[3]; Blob* creatures[3];
        const float xs[3] = {10, 5, 3};
        for (int i = 0; i < 3; ++i) {
            creatures[i] = makeObject(0x20, xs[i], 0, 0);
            std::memset(n[i].b, 0, sizeof n[i].b);
            putPtr(n[i], 12, creatures[i]->b);
            if (i < 2) putPtr(n[i], 0, n[i + 1].b);
        }
        std::memset(head.b, 0, sizeof head.b); putPtr(head, 0, n[0].b);
        world = {makeObject(0x20, x, 0, 0)};
        objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(center, 0, 20.0f, -1.0f, &head, nullptr);
        // Read back the x of each creature in list order.
        std::string seq;
        unsigned char* cur = getPtr<unsigned char*>(head, 0);
        while (cur != nullptr) {
            unsigned char* c; std::memcpy(&c, cur + 12, sizeof c);
            float fx; std::memcpy(&fx, c + 16, 4);
            seq += std::to_string(static_cast<int>(fx)) + ",";
            cur = nextOf(cur);
        }
        const std::string want = x == 5.0f ? "10,5,5,3," : x == 4.0f ? "10,5,4,3," : x == 11.0f ? "11,10,5,3," : "10,5,3,1,";
        expect("insert", seq, want);
    }
    std::printf(bad ? "FAIL\n" : "ok: sorted creature list\n");
    return bad;
}
