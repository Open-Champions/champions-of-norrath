// Host contract for makeCollisionList__FP12_worldHeaderffffPP13collisionParti. Not an EE plan input.
// The structs repeat the source's; on the host the pointers are wider, which the source's indexing allows.
#include <cstdio>
#include <cstring>

struct CollisionPart { short bounds[4]; int frame_stamp; unsigned char gap[4]; unsigned short flags; unsigned char rest[18]; };
struct CollisionWorld {
    unsigned char before_origin[8]; int origin_column; int origin_row; int columns; int rows;
    short** cells; unsigned char gap[4]; CollisionPart* parts;
};
extern "C" int collisionFrame = 5;
extern "C" void makeCollisionList__FP12_worldHeaderffffPP13collisionParti(CollisionWorld*, float, float, float, float, CollisionPart**, int);

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

static CollisionPart parts[6];
static short l_p0[] = {0, -1}, l_p2[] = {2, -1}, l_p1p3[] = {1, 3, -1}, l_empty[] = {-1}, l_p4[] = {4, -1}, l_p0b[] = {0, -1};
static short* cells[12];  // 4 columns x 3 rows
static CollisionWorld world;
static CollisionPart* out[16];

static void setup() {
    std::memset(parts, 0, sizeof parts);
    for (auto& p : parts) { p.bounds[0] = 0; p.bounds[1] = 0; p.bounds[2] = 0; p.bounds[3] = 0; p.frame_stamp = 1; }
    // Parts 0, 1 (outside), 3: bounds around the query box a=1700 b=3300 c=1800 d=3400; part 1 ends left of a.
    parts[0].bounds[0] = 1000; parts[0].bounds[1] = 3000; parts[0].bounds[2] = 2000; parts[0].bounds[3] = 3600;
    parts[1].bounds[0] = 1000; parts[1].bounds[1] = 3000; parts[1].bounds[2] = 1699; parts[1].bounds[3] = 3600;
    parts[2] = parts[0]; parts[2].flags = 2;
    parts[3] = parts[0];
    parts[4] = parts[0];
    for (auto& c : cells) c = l_empty;
    cells[0] = l_p0;          // (col 0, row 0)
    cells[1] = l_p0b;         // (col 1, row 0)
    cells[4] = l_p2;          // (col 0, row 1)
    cells[5] = l_p1p3;        // (col 1, row 1)
    cells[2 + 2 * 4 + 1] = l_p4;  // (col 3, row 2)
    world.origin_column = 10; world.origin_row = 20; world.columns = 4; world.rows = 3;
    world.cells = cells; world.parts = parts;
    std::memset(out, 0x55, sizeof out);
    collisionFrame = 5;
}

int main() {
    setup();
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 1700, 3300, 1800, 3400, out, 8);
    expect(out[0] == &parts[0] && out[1] == &parts[3] && out[2] == nullptr, "parts 0 and 3 listed once, then a zero entry");
    expect(parts[0].frame_stamp == 5 && parts[3].frame_stamp == 5, "listed parts stamped with collisionFrame");
    expect(parts[1].frame_stamp == 1 && parts[2].frame_stamp == 1 && parts[4].frame_stamp == 1, "skipped and out-of-range parts untouched");

    setup();
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 1700, 3300, 1800, 3400, out, 2);
    expect(out[0] == &parts[0] && out[1] == nullptr && parts[3].frame_stamp == 1, "capacity 2 stops after the first append");
    setup();
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 1700, 3300, 1800, 3400, out, 3);
    expect(out[0] == &parts[0] && out[1] == &parts[3] && out[2] == nullptr, "capacity 3 holds both");

    setup();
    parts[0].frame_stamp = 5;
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 1700, 3300, 1800, 3400, out, 8);
    expect(out[0] == &parts[3] && out[1] == nullptr, "a part already stamped this frame is skipped");

    setup();
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 900000, 900000, 900001, 900001, out, 8);
    expect(out[0] == nullptr, "box beyond the grid: empty list");
    setup();
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 0, 0, 100, 100, out, 8);
    expect(out[0] == nullptr, "box before the grid origin: empty list");
    setup();
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 1000, 3000, 100000, 100000, out, 12);
    expect(out[0] == &parts[0] && out[1] == &parts[1] && out[2] == &parts[3] && out[3] == &parts[4] && out[4] == nullptr,
           "large box clamped to the grid; the last cell is reached");
    setup();
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 1800, 3400, 1700, 3300, out, 8);
    expect(out[0] == nullptr, "reversed box: no rows or columns");

    // Negative coordinates floor: origin (-1, -1), box (-10..-5) is cell (-1, -1), i.e. index (0, 0).
    setup();
    world.origin_column = -1; world.origin_row = -1;
    parts[0].bounds[0] = -100; parts[0].bounds[1] = -100; parts[0].bounds[2] = 100; parts[0].bounds[3] = 100;
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, -10, -10, -5, -5, out, 8);
    expect(out[0] == &parts[0] && out[1] == nullptr, "negative coordinates use floor");

    // The clamps: a box that starts before the grid origin still reaches the first column and row.
    setup();
    cells[1] = l_empty;
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 0, 3300, 1800, 3400, out, 8);
    expect(out[0] == &parts[0] && out[1] == &parts[1] && out[2] == &parts[3] && out[3] == nullptr, "low column clamped to zero, not dropped");
    setup();
    cells[1] = l_empty;
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 1700, 0, 1800, 3400, out, 8);
    expect(out[0] == &parts[0], "low row clamped to zero");
    // The scale is the float 0x3bcccccc, not 0.00625f: 160 * scale floors to 0 (it would be 1 with 0x3bcccccd).
    setup();
    world.origin_column = 0; world.origin_row = 0;
    parts[0].bounds[0] = 0; parts[0].bounds[1] = 0; parts[0].bounds[2] = 1000; parts[0].bounds[3] = 1000;
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 160, 160, 160, 160, out, 8);
    expect(out[0] == &parts[0] && out[1] == nullptr, "exact scale constant: 160 falls in cell 0");

    // Inclusive bound tests, one side at a time (box 1700..1800 x 3300..3400, part 3 in the last cell).
    const int edges[4][2] = {{0, 1800}, {1, 3400}, {2, 1700}, {3, 3300}};
    for (const auto& e : edges) {
        for (int delta = 0; delta <= 1; ++delta) {
            setup();
            cells[0] = l_empty; cells[1] = l_empty; cells[4] = l_empty; cells[5] = l_p1p3;
            parts[1].bounds[0] = 1000; parts[1].bounds[1] = 3000; parts[1].bounds[2] = 2000; parts[1].bounds[3] = 3600;
            parts[3] = parts[1]; parts[3].bounds[e[0]] = (short)(e[1] + (e[0] < 2 ? delta : -delta));
            makeCollisionList__FP12_worldHeaderffffPP13collisionParti(&world, 1700, 3300, 1800, 3400, out, 8);
            const bool listed = out[0] == &parts[1] && out[1] == &parts[3];
            const bool only1 = out[0] == &parts[1] && out[1] == nullptr;
            expect(delta == 0 ? listed : only1, "a bound equal to the box edge is kept; one unit outside is dropped");
        }
    }
    if (failures) return 1;
    std::printf("makeCollisionList contract passed\n");
    return 0;
}
