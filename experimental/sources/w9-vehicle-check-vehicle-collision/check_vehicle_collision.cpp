// Observed checkVehicleCollision__7Vehicle at 0x002738a8, 552 bytes. Always
// returns 0. Nothing here was executed on the EE. Field meanings are not
// established here, so offsets stay numeric.
// Only the vehicle whose index word (+2156) equals vehicleCount - 1 does any
// work, so the pair test runs once per frame, from the last registered vehicle.
// For every ordered pair (i, j) of different entries of vehicleList below
// vehicleCount, with B = vehicleList[i] and A = vehicleList[j]: when the
// distance between their Point3 positions at +16 (square root of the sum of
// squares in x, y, z order) is below float(B+836) + float(A+836), the following
// runs for two passes, the second with A and B swapped: for each of the four
// Point3 values at B+452+12*s and each edge k of A (edge k runs from the Point3
// at A+452+12*k to the one at A+452+12*((k+1) mod 4), and has a companion
// Point3 at A+548+12*k), segmentLineHit__FP6Point3N50 is called with
// (edge end, edge start, companion, B's value, B's position, a local Point3);
// when it returns non-zero, dualImpulse__7VehicleP7VehicleT1P6Point3T3 is called
// with the vehicle given to this function, B, A, B's value and the companion.
// (After the swap in the second pass "A" and "B" are exchanged in all of that.)
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
int segmentLineHit__FP6Point3N50(const Point3* a, const Point3* b, const Point3* c, const Point3* d, const Point3* e, Point3* f);
void dualImpulse__7VehicleP7VehicleT1P6Point3T3(unsigned char* vehicle, unsigned char* b, unsigned char* a, const Point3* point, const Point3* other);
extern int vehicleCount;
extern unsigned char* vehicleList[];
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));

extern "C" int checkVehicleCollision__7Vehicle(unsigned char* vehicle) {
    const int count = vehicleCount;
    if (*reinterpret_cast<AliasedInt*>(vehicle + 2156) != count - 1) return 0;
    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < count; ++j) {
            if (j == i) continue;
            unsigned char* a = vehicleList[j];
            unsigned char* b = vehicleList[i];
            const Point3* const pos_a = reinterpret_cast<Point3*>(a + 16);
            const Point3* const pos_b = reinterpret_cast<Point3*>(b + 16);
            const float dx = pos_b->x - pos_a->x;
            const float dy = pos_b->y - pos_a->y;
            const float dz = pos_b->z - pos_a->z;
            const float reach = *reinterpret_cast<AliasedFloat*>(b + 836) + *reinterpret_cast<AliasedFloat*>(a + 836);
            if (!(__builtin_sqrtf(dx * dx + dy * dy + dz * dz) < reach)) continue;
            for (int pass = 0; pass < 2; ++pass) {
                for (int s = 0; s < 4; ++s) {
                    const Point3* const value = reinterpret_cast<Point3*>(b + 452 + 12 * s);
                    for (int k = 0; k < 4; ++k) {
                        const int next = (k + 1) % 4;
                        const Point3* const companion = reinterpret_cast<Point3*>(a + 548 + 12 * k);
                        Point3 hit;
                        if (segmentLineHit__FP6Point3N50(reinterpret_cast<Point3*>(a + 452 + 12 * next),
                                                         reinterpret_cast<Point3*>(a + 452 + 12 * k), companion, value,
                                                         reinterpret_cast<Point3*>(b + 16), &hit) != 0) {
                            dualImpulse__7VehicleP7VehicleT1P6Point3T3(vehicle, b, a, value, companion);
                        }
                    }
                }
                unsigned char* const swap = b;
                b = a;
                a = swap;
            }
        }
    }
    return 0;
}
