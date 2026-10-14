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

static const unsigned OBJ = 0x3000, GROUPS = 0x100, ENTRIES = 0x200, VERTS = 0x400, TABLE = 0x800;
static float rd(unsigned off) { float f; memcpy(&f, arena + off, 4); return f; }
static void wr(unsigned off, float f) { memcpy(arena + off, &f, 4); }
static void w32(unsigned off, int v) { memcpy(arena + off, &v, 4); }
static void wh(unsigned off, short v) { memcpy(arena + off, &v, 2); }

static bool box_is(unsigned off, const float* want) {
    for (int i = 0; i < 6; ++i) if (rd(off + 4 * i) != want[i]) return false;
    return true;
}
static void setup(int type, int groups, int g0, int g1, int g2) {
    memset(arena, 0, sizeof arena);
    w32(OBJ + 8, type); w32(OBJ + 44, groups);
    w32(OBJ + 68, GROUPS); w32(OBJ + 72, ENTRIES); w32(OBJ + 76, VERTS);
    wr(OBJ + 16, 0.5f);
    w32(GROUPS, g0); w32(GROUPS + 32, g1); w32(GROUPS + 64, g2);
    memset(arena + 0x1000, 0xaa, 0x40);
    ncalls = 0;
}

int main() {
    // Type 0: entry points (floats, 12 bytes each): e0 has 2 points, e1 has 0 points, e2 has 1 point.
    setup(0, 2, 2, 1, 0);
    wh(ENTRIES + 2, 2); wh(ENTRIES + 28 + 2, 0); wh(ENTRIES + 56 + 2, 1);
    float pts[3][3] = {{1, 2, 3}, {-4, 5, 0}, {7, -8, 9}};
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) wr(VERTS + 12 * i + 4 * j, pts[i][j]);
    if (CalcBVolumes__12VICollBufferiP7VIVect3(arena + OBJ, 0, 0) != 0) return 1;
    // e0 = box of p0, p1; e1 (no points) keeps e0's box; e2 = p2 alone.
    float e0[6] = {-4, 2, 0, 1, 5, 3}, e2[6] = {7, -8, 9, 7, -8, 9};
    float g1[6] = {7, -8, 9, 7, -8, 9};
    if (!box_is(ENTRIES + 4, e0) || !box_is(ENTRIES + 28 + 4, e0)) return 2;
    if (!box_is(ENTRIES + 56 + 4, e2)) return 3;
    // group 0 spans e0 and e1 -> e0's box; group 1 is e2 only, so total restarts at its first point.
    if (!box_is(GROUPS + 8, e0) || !box_is(GROUPS + 32 + 8, g1)) return 4;
    // buffer box: first group copied, then Union(box, box, group 1 total) -> covers both.
    float all[6] = {-4, -8, 0, 7, 5, 9};
    if (!box_is(OBJ + 20, all)) return 5;
    // Bound calls: B(1) B(1) B(0) B(0) | B(1) B(1) ; then Union once for the second group.
    const char order[] = {'B', 'B', 'B', 'B', 'B', 'B', 'U'};
    if (ncalls != 7) return 6;
    for (int i = 0; i < 7; ++i) if (calls[i].kind != order[i]) return 7;
    const int flags[] = {1, 1, 0, 0, 1, 1};
    for (int i = 0; i < 6; ++i) if (calls[i].flag != flags[i]) return 8;
    if (!calls[6].flag) return 9;

    // Type 1: signed halfwords at vertices + 6*v, scale 0.5.
    setup(1, 1, 1, 0, 0);
    wh(ENTRIES + 2, 2);
    short h[2][3] = {{10, 20, 30}, {-6, 8, 2}};
    for (int v = 0; v < 2; ++v) for (int j = 0; j < 3; ++j) wh(VERTS + 6 * v + 2 * j, h[v][j]);
    if (CalcBVolumes__12VICollBufferiP7VIVect3(arena + OBJ, 0, 0) != 0) return 10;
    float t1[6] = {-3, 4, 1, 5, 10, 15};
    if (!box_is(ENTRIES + 4, t1) || !box_is(GROUPS + 8, t1) || !box_is(OBJ + 20, t1)) return 11;

    // Type 3: stride 8, signed byte at +6 picks a table vector that is added after scaling.
    setup(3, 1, 1, 0, 0);
    wh(ENTRIES + 2, 2);
    float table[3][3] = {{0, 0, 0}, {100, 200, 300}, {-1, -2, -3}};
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) wr(TABLE + 12 * i + 4 * j, table[i][j]);
    for (int v = 0; v < 2; ++v) for (int j = 0; j < 3; ++j) wh(VERTS + 8 * v + 2 * j, h[v][j]);
    arena[VERTS + 6] = 1;
    arena[VERTS + 8 + 6] = 0xff;
    if (CalcBVolumes__12VICollBufferiP7VIVect3(arena + OBJ, 0, reinterpret_cast<const float*>(arena + TABLE + 12)) != 0) return 12;
    // table base is entry 1: index +1 -> entry 2 (-1,-2,-3); index -1 -> entry 0 (0,0,0).
    float p0[3] = {5 - 1, 10 - 2, 15 - 3}, p1[3] = {-3, 4, 1};
    float t3[6];
    for (int j = 0; j < 3; ++j) { t3[j] = p0[j] < p1[j] ? p0[j] : p1[j]; t3[3 + j] = p0[j] > p1[j] ? p0[j] : p1[j]; }
    if (!box_is(ENTRIES + 4, t3)) return 13;

    // Type 2 is rejected as soon as a point is needed; with no points it is not looked at.
    setup(2, 1, 1, 0, 0);
    wh(ENTRIES + 2, 1);
    if (CalcBVolumes__12VICollBufferiP7VIVect3(arena + OBJ, 0, 0) != -1) return 14;
    setup(2, 1, 1, 0, 0);
    wh(ENTRIES + 2, 0);
    if (CalcBVolumes__12VICollBufferiP7VIVect3(arena + OBJ, 0, 0) != 0) return 15;
    setup(0, 0, 0, 0, 0);
    if (CalcBVolumes__12VICollBufferiP7VIVect3(arena + OBJ, 0, 0) != 0 || ncalls != 0) return 16;
    return 0;
}
