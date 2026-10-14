// Host check for cleanUp__Fv. Not an EE plan input; callees are mocks. The mock for removeItemFromInventory
// shifts later entries down and decrements the count, which is the behavior the loop's index reuse relies on.
#include "clean_up.cpp"
#include <cstring>
#include <string>
#include <vector>

extern "C" {
unsigned char player[24000];
}

static std::string trace;
static std::vector<std::vector<int>> removals;  // (index, group)

extern "C" {
void objectIterateGrid__FPFP10GameObject_v(void (*v)(unsigned char*)) {
    if (v == cleanUpFunc2__FP10GameObject) trace += "2";
    else if (v == cleanUpFunc1__FP10GameObject) trace += "1";
    else trace += "?";
}
void cleanUpFunc1__FP10GameObject(unsigned char*) {}
void cleanUpFunc2__FP10GameObject(unsigned char*) {}
void playerRecomputeWeight__Fi(int a) { trace += "W"; if (a != 0) trace += "!"; }
}

// Entries: first word is a pointer to a description (30+ bytes); value lives in the entry at +8.
struct Desc {
    unsigned char bytes[32];
};
static Desc descs[8];

extern "C" int getValue__4itemPQ24item11description(unsigned char* item) {
    int v;
    std::memcpy(&v, item + 8, 4);
    return v;
}

extern "C" void removeItemFromInventory__Fiiii(int a, int index, int group, int b) {
    if (a != 0 || b != 0) trace += "!";
    trace += "R";
    removals.push_back({index, group});
    unsigned char* base = player + 328 + 4616 * group;
    int* count = reinterpret_cast<int*>(player + 4936 + 4616 * group);
    for (int i = index; i + 1 < *count; ++i) std::memcpy(base + 72 * i, base + 72 * (i + 1), 72);
    std::memset(base + 72 * (*count - 1), 0, 72);
    --*count;
}

static void setEntry(int group, int index, int desc, int value) {
    unsigned char* e = player + 328 + 4616 * group + 72 * index;
    const Desc* d = &descs[desc];
    std::memset(e, 0, 72);
    std::memcpy(e, &d, sizeof d);
    std::memcpy(e + 8, &value, 4);
}
static void setCount(int group, int n) { std::memcpy(player + 4936 + 4616 * group, &n, 4); }
static int getCount(int group) { int n; std::memcpy(&n, player + 4936 + 4616 * group, 4); return n; }
static int valueAt(int group, int index) {
    int v; std::memcpy(&v, player + 328 + 4616 * group + 72 * index + 8, 4); return v;
}
static int gold() { int v; std::memcpy(&v, player + 232, 4); return v; }

static void reset() {
    std::memset(player, 0, sizeof player);
    std::memset(descs, 0, sizeof descs);
    trace.clear();
    removals.clear();
}

int main() {
    // group 1: class is the signed byte at desc+28.
    reset();
    descs[0].bytes[28] = 3;
    descs[1].bytes[28] = 5;
    descs[2].bytes[28] = 0xff;  // -1: ignored
    int g1[][2] = {{0, 10}, {0, 30}, {1, 7}, {0, 30}, {2, 99}, {0, 20}, {1, 7}};
    for (int i = 0; i < 7; ++i) setEntry(1, i, g1[i][0], g1[i][1]);
    setCount(1, 7);
    std::memcpy(player + 232, "\x64\0\0\0", 4);  // 100 gold to start
    cleanUp__Fv();
    // class 3: values 10,30,30,20 -> keep first 30 (index 1); remove 10, second 30, 20.
    // class 5: values 7,7 -> keep the first 7, remove the second. Class -1 stays.
    if (getCount(1) != 3) return 1;
    if (valueAt(1, 0) != 30 || valueAt(1, 1) != 7 || valueAt(1, 2) != 99) return 2;
    // Gold uses (int)(float(value) * 0.39999998f): 10 -> 3, 30 -> 11, 20 -> 7, 7 -> 2; 100 + 3 + 11 + 7 + 2.
    if (gold() != 123) return 3;
    if (trace != "21RRRRW") return 4;
    if (removals.size() != 4) return 5;
    const int want[4][2] = {{0, 1}, {2, 1}, {3, 1}, {3, 1}};
    for (int i = 0; i < 4; ++i) if (removals[i][0] != want[i][0] || removals[i][1] != want[i][1]) return 6;

    // group 0: class 1 when desc+30 == 4, else class 0
    reset();
    descs[0].bytes[30] = 4;
    descs[1].bytes[30] = 0;
    descs[2].bytes[30] = 4;
    int g0[][2] = {{0, 5}, {1, 9}, {2, 8}, {1, 9}, {0, 6}};
    for (int i = 0; i < 5; ++i) setEntry(0, i, g0[i][0], g0[i][1]);
    setCount(0, 5);
    cleanUp__Fv();
    // class 1 (desc 0 and 2): 5,8,6 -> keep 8. class 0: 9,9 -> keep the first.
    if (getCount(0) != 2 || valueAt(0, 0) != 9 || valueAt(0, 1) != 8) return 7;
    // removals: 5 (gold 1), 9 (3), 6 (2) -> 6 gold; indexes 0, 2, 2 of group 0
    if (gold() != 6) return 8;
    if (removals.size() != 3) return 9;
    const int want0[3][2] = {{0, 0}, {2, 0}, {2, 0}};
    for (int i = 0; i < 3; ++i) if (removals[i][0] != want0[i][0] || removals[i][1] != want0[i][1]) return 9;

    // group 2: class is desc+28 except 15, which is ignored
    reset();
    descs[0].bytes[28] = 15;
    descs[1].bytes[28] = 2;
    setEntry(2, 0, 0, 1);
    setEntry(2, 1, 0, 50);
    setEntry(2, 2, 1, 4);
    setEntry(2, 3, 1, 9);
    setCount(2, 4);
    cleanUp__Fv();
    if (getCount(2) != 3 || valueAt(2, 2) != 9) return 10;
    if (gold() != 1) return 11;  // 0.4 * 4 = 1.6 -> 1

    // empty containers: nothing is removed, the weight is still recomputed
    reset();
    cleanUp__Fv();
    if (trace != "21W" || gold() != 0) return 12;
    return 0;
}
