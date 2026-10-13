// Host contract for the Skeleton constructor; not an EE input. Address-holding words are offsets from a host base. Stubs
// record every call as text; the expected text and fields are rebuilt below from a table of name variants and a
// straight-line model of the roll consumption.
#define W10_ACTOR_M_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "ctor.cpp"
unsigned char* w10_actor_m_host_base;
unsigned w10_actor_m_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
alignas(16) static unsigned char arena[0x4000];
unsigned char skeletonResourceMap[0x100];
unsigned skeletonResMap[0xc8];
unsigned resourceFrame;
int frameNm;
unsigned char vt_Skeleton[8];
static std::string trace;
static std::vector<unsigned> rolls;
static size_t roll_index;
static unsigned char* obj;
static const unsigned MODEL_OFF = 0x1000, HDR_OFF = 0x1100;
static std::string rel(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - obj); }
extern "C" unsigned eRand__10GameObject(void* o) { trace += "R"; (void)o; return rolls[roll_index++ % rolls.size()]; }
extern "C" void creature_construct(void* self, Point3 p, short type, void* map, float scale, char** args) {
    char b[160];
    std::snprintf(b, sizeof b, "creature(%.1f,%.1f,%.1f,%d,%d,%.1f,%d);", p.x, p.y, p.z, type, static_cast<int>(static_cast<unsigned char*>(map) - skeletonResourceMap), scale, args == nullptr);
    trace += b;
    std::memset(self, 0x21, 0x500);
    const unsigned model = MODEL_OFF;
    std::memcpy(static_cast<unsigned char*>(self) + 36, &model, 4);
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) { trace += std::string("lump(") + lump + "," + name + ");"; return arena + 0x2000 + 16 * (std::strlen(name) % 5); }
extern "C" int strticmp__FPCcT0(const char* a, const char* b) {
    trace += std::string("cmp(") + b + ");";
    for (;; ++a, ++b) {
        const int x = (*a >= 'A' && *a <= 'Z') ? *a + 32 : *a, y = (*b >= 'A' && *b <= 'Z') ? *b + 32 : *b;
        if (x != y) return x < y ? -1 : 1;
        if (x == 0) return 0;
    }
}
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* h, int mask) {
    trace += "chunks(" + std::to_string(static_cast<unsigned char*>(h) - arena) + "," + std::to_string(static_cast<unsigned>(mask)) + ");";
    return 0x1122334455667788ull;
}
extern "C" void initType__8Skeletonb(void*, bool f) { trace += std::string("initType(") + (f ? "1" : "0") + ");"; }
extern "C" void animInitState__FP14AnimationState(void* s) { trace += "animInit(" + rel(s) + ");"; }
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int f) {
    trace += "animAdd(" + rel(s) + "," + std::to_string(static_cast<unsigned char*>(h) - arena) + "," + std::to_string(f) + ");";
}
extern "C" void setState__8Skeletoni(void*, int s) { trace += "setState(" + std::to_string(s) + ");"; }
extern "C" void objectAddToSlowRunList__FP10GameObject(void*) { trace += "slow;"; }
extern "C" void setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(void* self, void* map, int a, float fa, int b, float fb, int c, float fc, int d, float fd, int e) {
    char t[160];
    std::snprintf(t, sizeof t, "flags(%d,%d,%d,%g,%d,%g,%d,%g,%d,%g,%d);", self == obj, map == skeletonResMap, a, fa, b, fb, c, fc, d, fd, e);
    trace += t;
}
extern "C" void finishConstruction__8Creature(void*) { trace += "finish;"; }

struct Variant { const char* text; int b972; int w968; };
static const Variant variants[] = {{"skeletonred", 2, -1}, {"skeletonblue", 1, -1}, {"skeletonswordohwred", 2, 0}, {"skeletonswordohwblue", 1, 0}, {"skeletonsworddhwred", 2, 1},
                                   {"skeletonsworddhwblue", 1, 1}, {"skeletonaxered", 2, 2}, {"skeletonaxeblue", 1, 2}, {"skeletonspearred", 2, 3}, {"skeletonspearblue", 1, 3},
                                   {"skeletonbowred", 2, 4}, {"skeletonbowblue", 2, 4}, {"skeletonswordohw", -1, 0}, {"skeletonsworddhw", -1, 1}, {"skeletonaxe", -1, 2},
                                   {"skeletonspear", -1, 3}, {"skeletonbow", -1, 4}};
static float fraction(unsigned r) { return static_cast<float>(static_cast<int>(r)) * 0.0000152587890625f; }

int main() {
    w10_actor_m_host_base = arena;
    obj = arena + 0x200;
    int bad = 0;
    const unsigned thr[5] = {0x3dccca2du, 0x3dccca2du, 0x3dcccab3u, 0x3dcccccu << 4 | 0xc, 0x3dcccbc0u};
    for (int k = 0; k < 60; ++k) {
        std::memset(arena, 0x5a, sizeof arena);
        std::memset(skeletonResMap, 0, sizeof skeletonResMap);
        const unsigned lump_off = 0x3000, n1_off = 0x3040, n2_off = 0x3060;
        std::strcpy(reinterpret_cast<char*>(arena + lump_off), "lumpx");
        std::strcpy(reinterpret_cast<char*>(arena + n1_off), "res1");
        std::strcpy(reinterpret_cast<char*>(arena + n2_off), "resource2");
        skeletonResMap[0] = lump_off;
        resourceFrame = 7;
        skeletonResMap[1] = (k % 3 == 0) ? 7 : 6;
        skeletonResMap[2] = (k % 11 == 0) ? 0 : n1_off; skeletonResMap[4] = (k % 13 == 0) ? 0 : n2_off;
        for (int v = 0; v < 5; ++v) skeletonResMap[(104 * v + 68) / 4] = HDR_OFF + 16 * v;
        frameNm = 900 + k;
        // roll values: mixture around the thresholds
        rolls.clear();
        unsigned seed = 12345u + 77u * k;
        for (int i = 0; i < 24; ++i) { seed = seed * 1664525u + 1013904223u; rolls.push_back(i % 5 == 0 ? (6000u + (seed >> 28) * 200u) : (seed >> 16)); }
        roll_index = 0;
        const bool known = k % 20 < 17;
        const char* name = known ? variants[k % 17].text : "other";
        char buffer[40];
        std::strcpy(buffer, name);
        if (k % 4 == 1) for (char* p = buffer; *p; ++p) if (*p >= 'a' && *p <= 'z' && (p - buffer) % 2 == 0) *p -= 32;
        trace.clear();
        __8SkeletonG6Point3sPPcPci(obj, Point3{1.0f, 2.0f, 3.0f}, static_cast<short>(k - 30), nullptr, buffer, k % 4);
        // model
        size_t ri = 0;
        auto R = [&]() { return rolls[ri++ % rolls.size()]; };
        std::string want = "creature(1.0,2.0,3.0," + std::to_string(k - 30) + "," + std::to_string((k % 4) << 6) + ",10.0,1);";
        if (skeletonResMap[1] != 7 || true) {}
        // the table work happens before the first roll; derive from the initial state used above
        const bool stamp_differs = !(k % 3 == 0);
        std::vector<unsigned> expect_res;
        if (stamp_differs) {
            if (k % 11 != 0) { want += "lump(lumpx,res1);"; }
            if (k % 11 != 0 && k % 13 != 0) { want += "lump(lumpx,resource2);"; }
        }
        want += "R";
        unsigned w968 = R() % 5u;
        int b972 = 0;
        int matched = -1;
        for (int i = 0; i < 17; ++i) {
            want += std::string("cmp(") + variants[i].text + ");";
            bool equal = true;
            for (const char *x = buffer, *y = variants[i].text;; ++x, ++y) {
                char cx = (*x >= 'A' && *x <= 'Z') ? *x + 32 : *x;
                if (cx != *y) { equal = false; break; }
                if (!*x) break;
            }
            if (equal) { matched = i; break; }
            if (i == 10) continue;
        }
        // bowred and bowblue share one result: the matcher stops at the first equal one; indices 10 and 11 both map to (2, 4)
        if (matched >= 0) {
            if (variants[matched].b972 >= 0) b972 = variants[matched].b972;
            if (variants[matched].w968 >= 0) w968 = static_cast<unsigned>(variants[matched].w968);
        }
        unsigned w208 = 0, w976 = 0, w1120 = 0xdead, w1124 = 0xdead;
        bool have1120 = false, have1124 = false;
        if (w968 < 5) {
            want += "R";
            const bool below = fraction(R()) < [&] { unsigned t = thr[w968]; float f; std::memcpy(&f, &t, 4); return f; }();
            if (w968 == 0) { if (below) w208 = 1; want += "R"; w976 = (R() & 1) ? 77 : 79; want += "R"; w1120 = 91 + 2 * (R() & 1); have1120 = true; }
            else if (w968 == 1) { if (below) w208 = 1; w976 = 89; }
            else if (w968 == 2) { if (below) w208 = 1; want += "R"; w976 = (R() & 1) ? 85 : 87; }
            else if (w968 == 3) { if (below) { want += "R"; w208 = (R() & 1) + 1; } want += "R"; w976 = (R() & 1) ? 95 : 97; }
            else { if (below) { want += "R"; w208 = (R() & 3) + 1; } w1124 = 0; have1124 = true; w976 = 83; }
        }
        unsigned w244 = 0;
        want += "R";
        const unsigned s = R();
        if (s & 1) { want += "R"; w244 |= 1u << (R() % 3); }
        if (w968 == 4) w244 |= 0x80; else if (s & 2) { want += "R"; w244 |= 1u << (R() % 12 + 9); }
        if (!(w244 & 0x4000)) { want += "R"; w244 |= 1u << (R() % 5 + 21); }
        want += "R";
        w244 |= 1u << ((R() & 1) + 5);
        char b[160];
        std::snprintf(b, sizeof b, "chunks(%u,%u);initType(0);animInit(288);animAdd(288,%u,1);setState(0);slow;", 0u, w244, HDR_OFF + 16 * w968);
        want += b;
        // header word at model+12 is the arena-relative offset of that word: here it is 0x5a5a5a5a, see below
        if (trace.find("chunks(") == std::string::npos) { std::fprintf(stderr, "case %d no chunks\n", k); bad = 1; continue; }
        std::string got = trace;
        // normalise the chunks header offset (it is the word read from model+12, i.e. 0x5a5a5a5a - arena offset noise)
        size_t c0 = got.find("chunks("), c1 = got.find(',', c0);
        got.replace(c0 + 7, c1 - c0 - 7, "0");
        // flags tail
        const int idx[15] = {1, 2, 3, 14, 15, 16, 27, 28, 29, 40, 41, 42, 53, 54, 55};
        for (int i = 0; i < 15; ++i) {
            const float f21 = 0.3f, f24 = 0.9f, f23 = 0.2f, f22 = 1.3f;
            if (i < 6 || i >= 8 && i < 12) std::snprintf(b, sizeof b, "flags(1,1,%d,%g,1,%g,0,%g,0,%g,0);", idx[i], f21, 0.0, 0.0, 0.0);
            else if (i < 8) std::snprintf(b, sizeof b, "flags(1,1,%d,%g,1,%g,1,%g,0,%g,0);", idx[i], f21, f24, 0.0, 0.0);
            else std::snprintf(b, sizeof b, "flags(1,1,%d,%g,2,%g,1,%g,0,%g,0);", idx[i], f23, f22, 0.0, 0.0);
            want += b;
        }
        want += "finish;";
        // compress the 'R' markers: the stubs record R for every roll, the model records R at the same places
        if (got != want) { std::fprintf(stderr, "case %d\n%s\n%s\n", k, got.c_str(), want.c_str()); bad = 1; }
        const bool fields_ok = *reinterpret_cast<unsigned*>(obj + 968) == w968 && obj[972] == b972 && *reinterpret_cast<unsigned*>(obj + 208) == w208 &&
                               (w968 >= 5 || *reinterpret_cast<unsigned*>(obj + 976) == w976) && (!have1120 || *reinterpret_cast<unsigned*>(obj + 1120) == w1120) &&
                               (!have1124 || *reinterpret_cast<unsigned*>(obj + 1124) == w1124) && *reinterpret_cast<unsigned*>(obj + 244) == w244 &&
                               *reinterpret_cast<unsigned*>(obj + 248) == 900u + k && obj[40] == 20 && obj[172] == 16 && *reinterpret_cast<unsigned*>(obj + 72) == w10m::to_word(vt_Skeleton) &&
                               *reinterpret_cast<unsigned*>(obj + 960) == 0 && *reinterpret_cast<unsigned*>(obj + 964) == 0 && *reinterpret_cast<unsigned long long*>(obj + 552) == 0x1122334455667788ull &&
                               *reinterpret_cast<unsigned long long*>(obj + 160) == (0x2121212121212121ull | 0x10000ull) && skeletonResMap[1] == 7 &&
                               *reinterpret_cast<short*>(obj + 1084) == -1 && *reinterpret_cast<short*>(obj + 1108) == -1 && obj[1080] == 0xff && obj[1081] == 0xff &&
                               *reinterpret_cast<short*>(obj + 1064) == 0 && *reinterpret_cast<short*>(obj + 1078) == 0;
        if (!fields_ok) { std::fprintf(stderr, "case %d fields (968=%u 972=%d)\n", k, w968, b972); bad = 1; }
    }
    if (!bad) std::puts("skeleton ctor ok");
    return bad;
}
