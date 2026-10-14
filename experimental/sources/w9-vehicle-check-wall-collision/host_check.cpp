// Host contract for checkWallCollision__7Vehicle. Not an EE plan input. The
// Packet struct has host-sized pointers here; every other pointer field read
// by the function (+72, object+20) sits where a host pointer does not overlap
// a field that is read.
#include "check_wall_collision.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static int failures = 0;
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static int& wd(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }

static unsigned char v[2400];
static unsigned char vtable[256];
static unsigned char object[128], entries[512], record[128], record2[128];

// Scripted collaborators.
static int ray_script[16], ray_calls;
static Packet ray_packets[16][8];
static int impulse_script[16], impulse_calls;
static float dist_script[64]; static int dist_calls;
struct ImpulseLog { Point3 point, normal, velocity; float factor; int a, b; };
static ImpulseLog impulse_log[16];
static int event_calls; static unsigned char* event_arg; static Point3 event_v, event_n;
static void event(unsigned char* self, const Point3* a, const Point3* b) { ++event_calls; event_arg = self; event_v = *a; event_n = *b; }

extern "C" int gameLineRayCollision__FP6Point3T0P23gameLineCollisionPacketi(const Point3*, const Point3*, Packet* p, int limit) {
    if (limit != 8) ++failures;
    const int n = ray_script[ray_calls];
    for (int h = 0; h < n; ++h) p[h] = ray_packets[ray_calls][h];
    return ray_script[ray_calls++] ? n : 0;
}
extern "C" void getParticleVelocity__7VehicleP6Point3(Point3* r, unsigned char*, const Point3* p) { *r = {p->x + 1000, p->y + 1000, p->z + 1000}; }
extern "C" int impulse__7VehicleP6Point3N21fii(unsigned char*, const Point3* p, const Point3* n, const Point3* vel, float f, int a, int b) {
    impulse_log[impulse_calls] = {*p, *n, *vel, f, a, b};
    return impulse_script[impulse_calls++];
}
extern "C" float distanceToPlane__FP6Point3N20(const Point3*, const Point3*, const Point3*) { return dist_script[dist_calls++]; }

static void setup() {
    std::memset(v, 0, sizeof v); std::memset(vtable, 0, sizeof vtable);
    std::memset(object, 0, sizeof object); std::memset(entries, 0, sizeof entries);
    std::memset(record, 0, sizeof record); std::memset(record2, 0, sizeof record2);
    std::memset(ray_script, 0, sizeof ray_script); std::memset(impulse_script, 0, sizeof impulse_script);
    std::memset(dist_script, 0, sizeof dist_script); std::memset(ray_packets, 0, sizeof ray_packets);
    ray_calls = impulse_calls = dist_calls = event_calls = 0;
    short delta = 0x14; void (*fn)(unsigned char*, const Point3*, const Point3*) = event;
    std::memcpy(vtable + 212, &fn, sizeof fn); std::memcpy(vtable + 208, &delta, 2);
    unsigned char* vt = vtable; std::memcpy(v + 72, &vt, sizeof vt);
    at(v, 16) = 0; at(v, 20) = 0; at(v, 24) = 0;
    for (int k = 0; k < 4; ++k) { at(v, 452 + 12 * k) = float(k + 1); at(v, 456 + 12 * k) = 0; at(v, 460 + 12 * k) = 0; at(v, 548 + 12 * k) = float(10 * (k + 1)); }
    at(v, 1188) = 7; at(v, 1192) = 8; at(v, 1196) = 9;
    unsigned char* e = entries; std::memcpy(object + 20, &e, sizeof e);
    wd(object, 4) = 3; wd(object, 8) = 1;
    for (int c = 0; c < 16; ++c) { ray_packets[c][0].object = object; ray_packets[c][0].record = record; }
    at(record, 24) = 1; at(record, 28) = 0; at(record, 32) = 0;
}


int main() {
    // No hits anywhere: returns 0 after 8 ray tests (4 corners + 4 edges).
    setup();
    expect(checkWallCollision__7Vehicle(v) == 0 && ray_calls == 8 && impulse_calls == 0, "quiet: 8 ray tests, no impulse");
    // Corner 0 hits with the normal facing the corner (n.d < 0): impulse (0.15, 0, 1), event, byte 60.
    setup(); ray_script[0] = 1; at(record, 24) = -1;           // normal (-1,0,0), corner 0 at (1,0,0) -> n.d = -1
    impulse_script[0] = 1;
    expect(checkWallCollision__7Vehicle(v) == 1, "impulse non-zero returns 1");
    expect(impulse_calls == 1 && impulse_log[0].point.x == 1 && impulse_log[0].velocity.x == 1001 && impulse_log[0].normal.x == -1, "impulse arguments");
    expect(std::fabs(impulse_log[0].factor - 0.15f) < 1e-6f && impulse_log[0].a == 0 && impulse_log[0].b == 1, "factor 0.15 and ints (0, 1)");
    expect(event_calls == 1 && event_v.x == 7 && event_v.y == 8 && event_n.x == -1 && v[110] == 60, "wall event with the starting velocity and the normal, byte set");
    expect(ray_calls == 1, "function ends at once");
    // Corner hit with the normal facing away (n.d >= 0): no impulse.
    setup(); ray_script[0] = 1; at(record, 24) = 1;
    checkWallCollision__7Vehicle(v);
    expect(impulse_calls == 0, "no impulse when the corner is not moving into the wall side");
    // The byte at +110 already set: returns 1 without the event.
    setup(); ray_script[0] = 1; at(record, 24) = -1; impulse_script[0] = 1; v[110] = 3;
    expect(checkWallCollision__7Vehicle(v) == 1 && event_calls == 0 && v[110] == 3, "byte non-zero: no event");
    // Edge hit: two slots (record point, ring neighbour), both inside all planes -> impulse per slot with -plane normal.
    setup(); ray_script[4] = 1;                                 // 5th ray call = edge 0
    at(record, 0) = 5; at(record, 4) = 6; at(record, 8) = 7; wd(record, 60) = 1;
    at(entries, 128) = 50; at(entries, 132) = 60; at(entries, 136) = 70;
    for (int i = 0; i < 8; ++i) dist_script[i] = -1.0f;          // every plane test negative
    impulse_script[0] = 0; impulse_script[1] = 1;
    expect(checkWallCollision__7Vehicle(v) == 1, "edge hit: second slot's impulse returns 1");
    expect(impulse_calls == 2 && impulse_log[0].point.x == 5 && impulse_log[1].point.x == 50, "slots: the record point, then entry (60+1) % 3 = 2");
    expect(impulse_log[0].normal.x == -10 && impulse_log[1].normal.x == -10, "normal is minus the plane of the edge (plane 0)");
    expect(dist_calls == 8, "four plane tests per slot");
    // A slot in front of a plane is dropped.
    setup(); ray_script[4] = 1; wd(record, 60) = 0; wd(object, 8) = 0; wd(object, 4) = 1;   // no second slot: count - 1 = 0, 0 < 0 false
    dist_script[0] = -1; dist_script[1] = 0.5f;
    expect(checkWallCollision__7Vehicle(v) == 0 && impulse_calls == 0 && dist_calls == 2, "a plane in front of the slot stops the tests and drops it");
    return failures ? 1 : 0;
}
