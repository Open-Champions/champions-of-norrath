// Observed checkWallCollision__7Vehicle at 0x002733d0, 1236 bytes. Returns 1
// when impulse__7VehicleP6Point3N21fii reports non-zero, else 0. Nothing here
// was executed on the EE. Field meanings are not established here, so offsets
// stay numeric. P is the Point3 at +16; corner k is the Point3 at +452+12k and
// plane k is the Point3 at +548+12k (k = 0..3); V is the Point3 at +1188, copied
// when the function starts. gameLineRayCollision__FP6Point3T0P23gameLineCollisionPacketi
// fills an array of up to 8 packets of 24 bytes (called "packets" here); only
// two pointer fields are read: packet+0 (call it object: words at +4 and +8,
// pointer at +20 to an array of 64-byte entries whose first 12 bytes are a
// Point3) and packet+4 (call it record: Point3 at +0, Point3 at +24, word at
// +60).
//   1. For each corner k: if gameLineRayCollision(corner, P, packets, 8) is
//      non-zero, with n = packets[0].record+24 and d = corner - P: when
//      n.d (as (nx*dx + ny*dy) + nz*dz) is below 0.0, impulse(vehicle, corner,
//      n, getParticleVelocity(vehicle, corner), 0.15, 0, 1) runs and a non-zero
//      result ends the function through step 4.
//   2. For each edge k (corner k to corner (k+1) mod 4, ray from the later
//      corner back to the earlier): for each returned packet, the record's
//      first Point3 is appended to a list together with k; unless the object's
//      word at +8 is zero and the record's word at +60 is not below the
//      object's word at +4 minus one, the entry at index
//      (record word +60 + 1) mod (object word +4) of the object's array is
//      appended too, with k.
//   3. When the list is empty return 0. For each listed Point3 q: it is skipped
//      when distanceToPlane(corner j, plane j, q) is above 0.0 for some j in
//      0..3, in order. Otherwise impulse(vehicle, q, -(plane at +548+12*edge),
//      getParticleVelocity(vehicle, q), 0.15, 0, 1) runs and a non-zero result
//      ends the function through step 4. After the list: return 0.
//   4. Step 4: if the signed byte at +110 is non-zero return 1; else call the
//      Vehicle virtual in table slot offset 208 (eventCollisionWall's slot:
//      halfword delta, pointer at +4) as (vehicle + delta, V copy, the normal
//      used for the impulse), set the byte at +110 to 60 and return 1.
// Point3 values passed by value in the original are passed as the address of a
// caller-owned copy, and the Point3 result of getParticleVelocity is returned
// through its first (hidden) pointer.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

struct Packet {
    unsigned char* object;
    unsigned char* record;
    int unknown[4];
};

extern "C" {
int gameLineRayCollision__FP6Point3T0P23gameLineCollisionPacketi(const Point3* a, const Point3* b, Packet* packets, int limit);
void getParticleVelocity__7VehicleP6Point3(Point3* result, unsigned char* vehicle, const Point3* point);
int impulse__7VehicleP6Point3N21fii(unsigned char* vehicle, const Point3* point, const Point3* normal, const Point3* velocity, float factor, int a, int b);
float distanceToPlane__FP6Point3N20(const Point3* a, const Point3* b, const Point3* c);
}

typedef int AliasedInt __attribute__((may_alias));
typedef short AliasedShort __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));
typedef void (*WallEvent)(unsigned char*, const Point3*, const Point3*);
typedef WallEvent AliasedWallEvent __attribute__((may_alias));

static inline int word(const unsigned char* base, int offset) {
    return *reinterpret_cast<const AliasedInt*>(base + offset);
}

static int wall_contact(unsigned char* vehicle, const Point3* velocity_at_start, const Point3* normal) {
    if (*reinterpret_cast<signed char*>(vehicle + 110) != 0) return 1;
    unsigned char* const entry = *reinterpret_cast<AliasedPointer*>(vehicle + 72) + 208;
    (*reinterpret_cast<AliasedWallEvent*>(entry + 4))(vehicle + *reinterpret_cast<AliasedShort*>(entry), velocity_at_start, normal);
    *(vehicle + 110) = 60;
    return 1;
}

extern "C" int checkWallCollision__7Vehicle(unsigned char* vehicle) {
    Packet packets[8];
    Point3 slots[64];
    int edges[64];
    int count = 0;
    Point3 velocity_at_start = *reinterpret_cast<Point3*>(vehicle + 1188);
    const Point3* const position = reinterpret_cast<Point3*>(vehicle + 16);
    Point3 normal;
    Point3 point;
    Point3 velocity;
    Point3 reported;

    for (int k = 0; k < 4; ++k) {
        const Point3* const corner = reinterpret_cast<Point3*>(vehicle + 452 + 12 * k);
        if (gameLineRayCollision__FP6Point3T0P23gameLineCollisionPacketi(corner, position, packets, 8) == 0) continue;
        getParticleVelocity__7VehicleP6Point3(&reported, vehicle, corner);
        velocity = reported;
        point = *corner;
        normal = *reinterpret_cast<Point3*>(packets[0].record + 24);
        const float dx = corner->x - position->x;
        const float dy = corner->y - position->y;
        const float dz = corner->z - position->z;
        if (normal.x * dx + normal.y * dy + normal.z * dz < 0.0f) {
            if (impulse__7VehicleP6Point3N21fii(vehicle, &point, &normal, &velocity, 0x1.333332p-3f, 0, 1) != 0)
                return wall_contact(vehicle, &velocity_at_start, &normal);
        }
    }

    for (int k = 0; k < 4; ++k) {
        const int next = (k + 1) % 4;
        const int hits = gameLineRayCollision__FP6Point3T0P23gameLineCollisionPacketi(
            reinterpret_cast<Point3*>(vehicle + 452 + 12 * next), reinterpret_cast<Point3*>(vehicle + 452 + 12 * k), packets, 8);
        for (int h = 0; h < hits; ++h) {
            unsigned char* const object = packets[h].object;
            unsigned char* const record = packets[h].record;
            slots[count] = *reinterpret_cast<Point3*>(record);
            edges[count] = k;
            ++count;
            if (word(object, 8) == 0 && !(word(record, 60) < word(object, 4) - 1)) continue;
            const int index = (word(record, 60) + 1) % word(object, 4);
            slots[count] = *reinterpret_cast<Point3*>(*reinterpret_cast<AliasedPointer*>(object + 20) + index * 64);
            edges[count] = k;
            ++count;
        }
    }
    if (count <= 0) return 0;

    for (int i = 0; i < count; ++i) {
        int j = 0;
        for (; j < 4; ++j) {
            if (0.0f < distanceToPlane__FP6Point3N20(reinterpret_cast<Point3*>(vehicle + 452 + 12 * j),
                                                      reinterpret_cast<Point3*>(vehicle + 548 + 12 * j), &slots[i]))
                break;
        }
        if (j != 4) continue;
        getParticleVelocity__7VehicleP6Point3(&reported, vehicle, &slots[i]);
        velocity = reported;
        point = slots[i];
        const Point3* const plane = reinterpret_cast<Point3*>(vehicle + 548 + 12 * edges[i]);
        normal.x = -plane->x;
        normal.y = -plane->y;
        normal.z = -plane->z;
        if (impulse__7VehicleP6Point3N21fii(vehicle, &point, &normal, &velocity, 0x1.333332p-3f, 0, 1) != 0)
            return wall_contact(vehicle, &velocity_at_start, &normal);
    }
    return 0;
}
