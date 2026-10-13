// Host check only; not an EE build input. Build and run:
//   c++ -std=c++17 -Wall -Wextra -Werror -DNORRATH_HOST_ARENA host_check.cpp && ./a.out
// Table words in the arena are 32-bit offsets; offsets 0xF000.. stand for the stub functions. Every call the
// function makes is recorded as a trace line; an independently written model builds the expected trace.
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include "get_rank_description.cpp"

static unsigned char arena[0x1000];
static std::string trace;
static int variant, curRank, textCount;
static int delta;
static unsigned short formats[64];
static unsigned char* const objectPtr = arena + 0x100;
static unsigned short* const outPtr = reinterpret_cast<unsigned short*>(arena + 0x800);

static int rampValue(int index, int rank) {
    static const int A[] = {3, -2, 5, 1, 0, 7, -4, 2}, B[] = {1, 2, -1, 0, 3, 1, 2, -3}, C[] = {0, 1, -5, 2, 9, -1, 4, 6};
    int v = variant;
    if (v >= 64) return (((v - 64) >> (2 * (index & 3))) & 3) - 1;
    return (index + 1) * A[v % 8] + rank * B[(v / 8 + index) % 8] + C[(v / 3 + index * 2) % 8] - (v % 5 == 0 ? index : 0) +
           (v % 7 == 0 ? 1 : 0) * (index % 2);
}
static void ev(const char* fmt, long a = 0, long b = 0, long c = 0, long d = 0, long e = 0) {
    char buf[200];
    std::snprintf(buf, sizeof buf, fmt, a, b, c, d, e);
    trace += buf;
    trace += ';';
}
static unsigned bitsOf(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static long objOffset(unsigned char* o) { return static_cast<long>(o - objectPtr); }
static int rampStub(unsigned char* o, int index, int rank) {
    ev("R%ld,%ld,%ld", objOffset(o), index, rank);
    return rampValue(index, rank);
}
static void damageStub(unsigned char* o, int index, int rank, float scale, int flag, int* lo, int* hi) {
    ev("D%ld,%ld,%ld,%lx,%ld", objOffset(o), index, rank, (long)bitsOf(scale), flag);
    *lo = rampValue(index + 20, rank);
    *hi = rampValue(index + 30, rank) * 3;
}
static int tweekStub(unsigned char* o, int rank, int value) {
    ev("K%ld,%ld,%ld", objOffset(o), rank, value);
    return rank * 2 - value + variant % 3;
}
extern "C" unsigned char* norrath_host_pointer(unsigned address) {
    if (address == 0xF001) return reinterpret_cast<unsigned char*>(rampStub);
    if (address == 0xF002) return reinterpret_cast<unsigned char*>(damageStub);
    if (address == 0xF003) return reinterpret_cast<unsigned char*>(tweekStub);
    return arena + address;
}
extern "C" unsigned short* textGetString__FPc(const char* key) {
    trace += 'T'; trace += key; trace += ';';
    return formats + textCount++;
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* f, float a, float b, float c, float e,
                                         float g, float h, float i, float j) {
    float v[8] = {a, b, c, e, g, h, i, j};
    ev("U%ld,%ld", d == outPtr ? 1 : 0, f - formats);
    for (float x : v) ev("%lx", (long)bitsOf(x));
}
extern "C" int getEnemyWord__Fi(int value) {
    ev("E%ld", value);
    return value * 5 + 2 + variant;
}
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }

// Independent expectation: the same calls in the same order, written against a recording model.
struct Model {
    std::string t;
    int rank, texts = 0;
    void ev(const char* fmt, long a = 0, long b = 0, long c = 0, long d = 0, long e = 0) {
        char buf[200];
        std::snprintf(buf, sizeof buf, fmt, a, b, c, d, e);
        t += buf; t += ';';
    }
    int rampAt(int index, int r) { ev("R%ld,%ld,%ld", delta, index, r); return rampValue(index, r); }
    int ramp(int index) { return rampAt(index, rank); }
    void dmg(int index, float scale, int flag, int* lo, int* hi) {
        ev("D%ld,%ld,%ld,%lx,%ld", delta, index, rank, (long)bitsOf(scale), flag);
        *lo = rampValue(index + 20, rank);
        *hi = rampValue(index + 30, rank) * 3;
    }
    int tweek(int r, int v) { ev("K%ld,%ld,%ld", delta, r, v); return r * 2 - v + variant % 3; }
    int enemy(int v) { ev("E%ld", v); return v * 5 + 2 + variant; }
    int rnd(int v) {
        float x = (float)v, y = x + x * 0.2f;
        return (int)(y >= 0.0f ? y + 0.5f : y - 0.5f);
    }
    void textOnly(const char* key) { t += 'T'; t += key; t += ';'; ++texts; }
    void usOnly(std::initializer_list<float> fl) {
        float v[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int k = 0;
        for (float x : fl) v[k++] = x;
        ev("U%ld,%ld", 1, texts - 1);
        for (float x : v) ev("%lx", (long)bitsOf(x));
    }
    void emit(const char* key, std::initializer_list<float> fl) { textOnly(key); usOnly(fl); }
};

int main() {
    int bad = 0, runs = 0;
    const short deltas[] = {0, 24, -8};
    for (short dl : deltas)
        for (int v = 0; v < 320; ++v)
            for (int rank = -2; rank <= 12; ++rank) {
                std::memset(arena, 0, sizeof arena);
                delta = dl; variant = v; curRank = rank; textCount = 0; trace.clear();
                put32(0x100 + 96, 0x400);
                for (int slot : {9, 11, 15}) {
                    std::memcpy(arena + 0x400 + slot * 8, &dl, 2);
                    put32(0x400 + slot * 8 + 4, slot == 9 ? 0xF002 : slot == 11 ? 0xF001 : 0xF003);
                }
                getRankDescription__14SkillColdArrowPUsi(objectPtr, outPtr, rank);
                Model M; M.rank = rank;
                    int c = M.ramp(2), b = M.ramp(1), d = M.ramp(3);
                    M.emit("coldarrow_rankdesc", {(float)c, (float)(b / 12), (float)d});
                ++runs;
                if (M.t != trace) {
                    if (bad < 3) std::printf("fail delta=%d variant=%d rank=%d\n got  %s\n want %s\n", dl, v, rank, trace.c_str(), M.t.c_str());
                    bad = 1;
                }
            }
    std::printf("%d runs, %s\n", runs, bad ? "FAIL" : "ok");
    return bad;
}
