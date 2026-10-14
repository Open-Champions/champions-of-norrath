// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "calc_bvolumes.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

struct Call { char kind; int flag; float p[3]; };
static Call calls[64];
static int ncalls;
extern "C" void Bound__6VIBBoxiRC7VIVect3(float* box, int restart, const float* p) {
    Call& c = calls[ncalls++];
    c.kind = 'B'; c.flag = restart; memcpy(c.p, p, 12);
    if (restart) { for (int i = 0; i < 3; ++i) { box[i] = p[i]; box[3 + i] = p[i]; } return; }
    for (int i = 0; i < 3; ++i) { if (p[i] < box[i]) box[i] = p[i]; if (p[i] > box[3 + i]) box[3 + i] = p[i]; }
}
extern "C" void Union__6VIBBoxRC6VIBBoxT1(float* d, const float* a, const float* b) {
    Call& c = calls[ncalls++];
    c.kind = 'U'; c.flag = d == a; memcpy(c.p, b, 12);
    for (int i = 0; i < 3; ++i) {
        float lo = b[i] < a[i] ? b[i] : a[i], hi = b[3 + i] > a[3 + i] ? b[3 + i] : a[3 + i];
        d[i] = lo; d[3 + i] = hi;
    }
}

static const unsigned OBJ = 0x3000, GROUPS = 0x100, ENTRIES = 0x200, DATA = 0x400, TABLE = 0x2000;
static float rd(unsigned off) { float f; memcpy(&f, arena + off, 4); return f; }
static void wr(unsigned off, float f) { memcpy(arena + off, &f, 4); }
static void w32(unsigned off, int v) { memcpy(arena + off, &v, 4); }
static void wh(unsigned off, short v) { memcpy(arena + off, &v, 2); }
static bool box_is(unsigned off, const float* want) {
    for (int i = 0; i < 6; ++i) if (rd(off + 4 * i) != want[i]) return false;
    return true;
}
static void setup(int type, int groups) {
    memset(arena, 0, sizeof arena);
    w32(OBJ + 8, type); w32(OBJ + 64, groups);
    w32(OBJ + 88, GROUPS); w32(OBJ + 92, ENTRIES); w32(OBJ + 96, DATA);
    wr(OBJ + 48, 0.5f);
    ncalls = 0;
}

int main() {
    // Type 0, stride 64: groups of 2 entries (2 and 1 points) and 1 entry (0 points).
    setup(0, 2);
    w32(GROUPS + 4, 2); w32(GROUPS + 16 + 4, 1);
    w32(ENTRIES, 2); w32(ENTRIES + 28, 1); w32(ENTRIES + 56, 0);
    float pts[3][3] = {{1, 2, 3}, {-4, 5, 0}, {7, -8, 9}};
    unsigned at_[3] = {DATA + 32, DATA + 32 + 64, DATA + 160 + 16 + 32};
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) wr(at_[i] + 4 * j, pts[i][j]);
    if (CalcBVolumes__12VIPrimBufferiP7VIVect3(arena + OBJ, 0, 0) != 0) return 1;
    float e0[6] = {-4, 2, 0, 1, 5, 3}, e1[6] = {7, -8, 9, 7, -8, 9};
    float g0[6] = {-4, -8, 0, 7, 5, 9};
    if (!box_is(ENTRIES + 4, e0) || !box_is(ENTRIES + 28 + 4, e1)) return 2;
    if (!box_is(ENTRIES + 56 + 4, e1)) return 3;
    if (!box_is(OBJ + 20, g0)) return 4;
    // call order: B(1) B(0) | B(1) | U (total, e1) ; then group 1: no points, total = cur, U(box, total)
    const char order[] = {'B', 'B', 'B', 'U', 'U'};
    if (ncalls != 5) return 5;
    for (int i = 0; i < 5; ++i) if (calls[i].kind != order[i]) return 6;
    if (calls[0].flag != 1 || calls[1].flag != 0 || calls[2].flag != 1 || calls[3].flag != 1) return 7;
    if (calls[0].p[0] != 1 || calls[1].p[0] != -4 || calls[2].p[0] != 7) return 8;

    // Packed halfword types: x, y, z at +4, +6, +8, scale 0.5; strides 32 (2, 5, 4) and 16 (8).
    for (int type : {2, 4, 5, 8}) {
        setup(type, 1);
        w32(GROUPS + 4, 2); w32(ENTRIES, 1); w32(ENTRIES + 28, 1);
        int stride = type == 8 ? 16 : 32;
        unsigned first = DATA + 32, second = DATA + 32 + stride + 16 + 32;
        short v0[3] = {10, 20, 30}, v1[3] = {-6, 8, 2};
        for (int j = 0; j < 3; ++j) { wh(first + 4 + 2 * j, v0[j]); wh(second + 4 + 2 * j, v1[j]); }
        float shift[2][3] = {{100, 200, 300}, {1000, 2000, 3000}};
        for (int i = 0; i < 2; ++i) for (int j = 0; j < 3; ++j) wr(TABLE + 12 * i + 4 * j, shift[i][j]);
        wh(first + 10, 1);
        wh(second + 10, 0);
        if (CalcBVolumes__12VIPrimBufferiP7VIVect3(arena + OBJ, 0, reinterpret_cast<const float*>(arena + TABLE)) != 0) return 10;
        float a[3], b[3];
        for (int j = 0; j < 3; ++j) { a[j] = v0[j] * 0.5f; b[j] = v1[j] * 0.5f; }
        if (type == 4) for (int j = 0; j < 3; ++j) { a[j] += shift[1][j]; b[j] += shift[0][j]; }
        float eb[6], ea[6];
        for (int j = 0; j < 3; ++j) { ea[j] = ea[3 + j] = a[j]; eb[j] = eb[3 + j] = b[j]; }
        if (!box_is(ENTRIES + 4, ea) || !box_is(ENTRIES + 28 + 4, eb)) return 11;
    }
    // Rejected types.
    for (int type : {1, 3, 6, 7, 9}) {
        setup(type, 1);
        w32(GROUPS + 4, 1);
        if (CalcBVolumes__12VIPrimBufferiP7VIVect3(arena + OBJ, 0, 0) != -1 || ncalls != 0) return 12;
    }
    setup(1, 0);
    if (CalcBVolumes__12VIPrimBufferiP7VIVect3(arena + OBJ, 0, 0) != 0) return 13;
    return 0;
}
