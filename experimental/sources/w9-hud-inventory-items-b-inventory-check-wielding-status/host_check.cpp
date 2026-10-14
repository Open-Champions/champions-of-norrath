#include <cstring>
#include <cstddef>
#include <vector>
extern "C" {
unsigned char player[23984 * 4 + 512];
int inventoryCheckWieldingStatus__FiPQ24item11description(int, const void*);
}

static int rank60, rank61, rankCalls, strstrCalls;
extern "C" int playerGetSkillRank__Fii(int p, int s) { ++rankCalls; if (p != 2) return -1000; return s == 60 ? rank60 : rank61; }
extern "C" char* strstr(const char* h, const char* n) {
    ++strstrCalls;
    for (; *h; ++h) { const char *a = h, *b = n; while (*a && *b && *a == *b) ++a, ++b; if (!*b) return (char*)h; }
    return nullptr;
}

// host mirror of the item and description layouts (pointer-width independent offsets)
struct Desc { const char* name; unsigned char gap[29 - sizeof(void*)]; signed char kind, sub; unsigned char g2; int shape; };
struct Entry { short tag, value; unsigned char u[4]; };
struct Item { const Desc* d; unsigned char gap[8 - sizeof(void*)]; long long flags; unsigned char g2[20]; Entry e[4]; };
static_assert(offsetof(Item, e) == 36 && offsetof(Item, flags) == 8, "layout");

static int& w(int off) { return *reinterpret_cast<int*>(player + 2 * 23984 + off); }
static int run(int kind, int sub, int shape, const char* name, long long flags, int mode, std::vector<Entry> es = {}) {
    Desc d = {name, {}, (signed char)kind, (signed char)sub, 0, shape};
    Item it = {&d, {}, flags, {}, {}};
    for (size_t i = 0; i < es.size() && i < 4; ++i) it.e[i] = es[i];
    for (size_t i = es.size(); i < 4; ++i) it.e[i].tag = -1;
    w(80) = mode;
    return inventoryCheckWieldingStatus__FiPQ24item11description(2, &it);
}

int main() {
    std::memset(player, 0, sizeof player);
    w(144) = w(148) = w(152) = w(156) = 100; player[2 * 23984 + 88] = 50;
    rank60 = rank61 = 10;
    // kind 1
    if (run(1, 0, 0, "helm", 0x80000, 4) != 0) return 1;
    if (run(1, 0, 0, "helm", 0x80000, 2) != 1) return 2;
    if (run(1, 0, 0, "helm", 0, 4) != 1) return 3;
    if (run(1, 0, 0, "helm", 0, 2) != 0) return 4;
    // "circlet" suppresses the 1
    if (run(1, 0, 0, "circlet", 0, 4) != 0 || strstrCalls == 0) return 5;
    // mode 1
    if (run(2, 3, 0, "x", 0, 1) != 1 || run(2, 4, 0, "x", 0, 1) != 1) return 6;
    if (run(2, 2, 0, "x", 0, 1) != 0 || run(2, 5, 0, "x", 0, 1) != 0) return 7;
    if (run(2, 2, 8, "x", 0, 1) != 1 || run(2, 2, 4, "x", 0, 1) != 1 || run(2, 2, 3, "x", 0, 1) != 0) return 8;
    // mode 3
    for (int s = 0; s < 6; ++s) if (run(2, s, 0, "x", 0, 3) != (s >= 1 && s <= 3)) return 9;
    if (run(2, 5, 8, "x", 0, 3) != 0) return 10;
    // mode 4
    for (int s = 0; s < 6; ++s) if (run(2, s, 0, "x", 0, 4) != (s == 1 || s == 2 || s == 4)) return 11;
    if (run(2, 5, 8, "x", 0, 4) != 1 || run(2, 5, 4, "x", 0, 4) != 1) return 12;
    if (run(2, 1, 8, "x", 1LL << 36, 4) != 0 || run(2, 1, 0, "x", 1LL << 36, 4) != 0) return 13;
    // other modes
    if (run(2, 1, 8, "x", 0, 0) != 0 || run(2, 1, 8, "x", 0, 2) != 0) return 14;
    // requirement checks, in order of precedence (later wins)
    if (run(2, 3, 0, "x", 0, 1, {{14, 51, {}}}) != 2) return 15;
    if (run(2, 3, 0, "x", 0, 1, {{14, 50, {}}}) != 1) return 16;   // equal passes
    if (run(2, 2, 0, "x", 0, 1, {{15, 101, {}}}) != 3) return 17;
    if (run(2, 2, 0, "x", 0, 1, {{16, 101, {}}}) != 4) return 18;
    if (run(2, 2, 0, "x", 0, 1, {{17, 101, {}}}) != 5) return 19;
    if (run(2, 2, 0, "x", 0, 1, {{18, 101, {}}}) != 6) return 20;
    if (run(2, 2, 0, "x", 0, 1, {{15, 100, {}}, {16, 100, {}}, {17, 100, {}}, {18, 100, {}}}) != 0) return 21;
    if (run(2, 2, 0, "x", 0, 1, {{14, 99, {}}, {15, 200, {}}}) != 3) return 22;
    // skills
    if (run(2, 2, 0, "x", 0, 1, {{39, 11, {}}}) != 7) return 23;
    if (run(2, 2, 0, "x", 0, 1, {{39, 10, {}}}) != 0) return 24;
    if (run(2, 2, 0, "x", 0, 1, {{40, 11, {}}}) != 8) return 25;
    if (run(2, 2, 0, "x", 0, 1, {{39, 11, {}}, {40, 11, {}}}) != 8) return 26;
    rank60 = -5;
    // ranks compare as signed values
    if (run(2, 2, 0, "x", 0, 1, {{39, -5, {}}}) != 0 || run(2, 2, 0, "x", 0, 1, {{39, -4, {}}}) != 7) return 27;
    rank60 = rank61 = 10;
    // tag 19: needs mode == value - 1
    if (run(2, 2, 0, "x", 0, 1, {{19, 2, {}}}) != 0) return 28;
    if (run(2, 2, 0, "x", 0, 2, {{19, 2, {}}}) != 1) return 29;
    if (run(2, 2, 0, "x", 0, 0, {{19, 0, {}}}) != 0) return 30;
    if (run(2, 2, 0, "x", 0, 1, {{14, 99, {}}, {19, 3, {}}}) != 1) return 31;
    // first matching entry wins
    if (run(2, 2, 0, "x", 0, 1, {{15, 50, {}}, {15, 500, {}}}) != 0) return 32;
    // player index is passed to the skill query
    rankCalls = 0; run(2, 2, 0, "x", 0, 1);
    if (rankCalls != 2) return 33;
    return 0;
}
