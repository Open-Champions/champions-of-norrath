#include <cstring>

extern "C" void worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(
    void* world, const float* low, const float* high, unsigned char** out, int limit);

alignas(8) static unsigned char arena[65536];
unsigned char* norrath_world_arena = arena;

static unsigned& W(int off) { return *reinterpret_cast<unsigned*>(arena + off); }
static short& H(int off) { return *reinterpret_cast<short*>(arena + off); }
static float& F(int off) { return *reinterpret_cast<float*>(arena + off); }

static const int kRecords = 1024;
static void record(int n, unsigned flags, float x0, float y0, float z0, float x1, float y1, float z1) {
    const int r = kRecords + n * 60;
    F(r + 8) = x0; F(r + 12) = y0; F(r + 16) = z0;
    F(r + 20) = x1; F(r + 24) = y1; F(r + 28) = z1;
    W(r + 36) = flags;
}
static void leaf(int at, int n, const int* idx) {
    H(at) = static_cast<short>(n);
    for (int i = 0; i < n; ++i) H(at + 2 + i * 2) = static_cast<short>(idx[i]);
}

static int run(const float* a, const float* b, int limit, unsigned char** out) {
    unsigned char** end = out;
    std::memset(out, 0xff, 16 * sizeof(unsigned char*));
    worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(arena, a, b, out, limit);
    while (*end) ++end;
    return int(end - out);
}

int main() {
    std::memset(arena, 0, sizeof arena);
    W(4) = 4096;       // root node
    W(36) = kRecords;  // record table
    // Records 0..4. Record 3 lacks the 0x80 flag.
    record(0, 0x80, 0, 0, 0, 10, 10, 10);
    record(1, 0x80, 5, 5, 5, 20, 20, 20);
    record(2, 0x80, 200, 0, 0, 300, 10, 10);
    record(3, 0x00, 0, 0, 0, 1000, 1000, 1000);
    record(4, 0x80, 210, 0, 0, 220, 10, 10);

    // Root split node on axis 0; near key 100.0 (25<<2), far key 100.0.
    const int root = 4096;
    H(root) = static_cast<short>(0xc000 | 0);
    H(root + 2) = 25; H(root + 4) = 0;
    H(root + 6) = 25; H(root + 8) = 0;
    // children: left leaf at root+20, right leaf at root+18+2*off.
    const int left_idx[3] = {0, 1, 3};
    leaf(root + 20, 3, left_idx);
    const int right = root + 40;
    H(root + 18) = static_cast<short>((right - (root + 18)) / 2);
    const int right_idx[2] = {2, 4};
    leaf(right, 2, right_idx);

    unsigned char* out[16];
    const float lo[3] = {0, 0, 0};
    float hi[3] = {50, 50, 50};
    // Only the near side: x range 0..50 never reaches 100.
    int n = run(lo, hi, 16, out);
    if (n != 2 || out[0] != arena + kRecords || out[1] != arena + kRecords + 60) return 1;

    // Both sides, box wide in x.
    hi[0] = 250;
    n = run(lo, hi, 16, out);
    // Near leaf pushed first then far; the far leaf is popped first.
    if (n != 4) return 2;
    if (out[0] != arena + kRecords + 2 * 60 || out[1] != arena + kRecords + 4 * 60) return 3;
    if (out[2] != arena + kRecords || out[3] != arena + kRecords + 60) return 4;

    // Only the far side: A.x above the near key.
    const float lo_far[3] = {150, 0, 0};
    n = run(lo_far, hi, 16, out);
    if (n != 2 || out[0] != arena + kRecords + 2 * 60) return 5;

    // y range excludes everything (strict tests: touching still counts).
    const float lo_y[3] = {0, 11, 0};
    const float hi_y[3] = {50, 12, 50};
    n = run(lo_y, hi_y, 16, out);
    if (n != 1 || out[0] != arena + kRecords + 60) return 6;  // record 1 spans y 5..20
    const float lo_t[3] = {10, 10, 10};
    const float hi_t[3] = {10, 10, 10};
    n = run(lo_t, hi_t, 16, out);
    if (n != 2) return 7;  // touching at exactly 10 still overlaps records 0 and 1

    // The walk ends when the decremented limit reaches 1.
    hi[0] = 250;
    n = run(lo, hi, 3, out);
    if (n != 2) return 8;  // limit 3: stored 1 -> 2, stored 2 -> 1, stop with two entries
    n = run(lo, hi, 2, out);
    if (n != 1) return 9;
    // A node with 0x8000 but not 0x4000 is ignored.
    H(root) = static_cast<short>(0x8000);
    n = run(lo, hi, 16, out);
    if (n != 0) return 10;
    // Leaf with a negative-combine key: the low half is sign extended and OR'd.
    H(root) = static_cast<short>(0xc000);
    H(root + 2) = -1; H(root + 4) = 0;   // value = -1 | 0 = -1 -> -4.0
    H(root + 6) = -1; H(root + 8) = 0;
    const float lo_neg[3] = {-4.0f, 0, 0};
    const float hi_neg[3] = {-4.0f, 50, 50};
    record(2, 0x80, -10, 0, 0, 0, 10, 10);
    n = run(lo_neg, hi_neg, 16, out);
    // Far key -4.0 is reachable only with the sign-extended combine: the far
    // leaf is walked and record 2 (x -10..0) overlaps, record 4 does not.
    if (n != 1 || out[0] != arena + kRecords + 2 * 60) return 11;
    return 0;
}
