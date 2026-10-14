// Observed getSeatPosition__7VehicleiP6Point3 at 0x002712b8, 460 bytes.
// For seat index i (not range checked):
//   seat offset  = Point3 at vehicle+1932+12*i
//   holder       = pointer at vehicle+2088+4*i
//   rotation     = Matrix33 (9 floats) at vehicle+1116
// With no holder, offset' = matTransform(rotation, seat offset).
// With a holder, a copy of the rotation is turned by matRot axis 2 and then
// axis 0, each by the short (int)(holder float * 0.005555555 * 32767.0)
// taken from holder+280 and holder+284 respectively; offset' is then the
// copy-transformed seat offset plus the copy-transformed Point3 at holder+288
// (that second transform uses the already rotated copy).
// The result stored at the output is vehicle+16 (Point3) plus offset'.
// Nothing is returned. Nothing here was executed on the EE. What the holder
// object is, and the meaning of the two angle floats, are not established.
struct Point3 { float x, y, z; };
struct Matrix33 { float m[9]; };

extern "C" {
void matTransform__FP6Point3P8Matrix33T0(Point3* out, Matrix33* matrix, Point3* in);
void matRot__FP8Matrix33is(Matrix33* matrix, int axis, short angle);
}

typedef float AliasedFloat __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));

extern "C" void getSeatPosition__7VehicleiP6Point3(unsigned char* vehicle, int seat, Point3* out) {
    Point3 sum;
    Point3 turned;
    Matrix33 rotation;
    unsigned char* const holder = *reinterpret_cast<AliasedPointer*>(vehicle + 2088 + seat * 4);
    Matrix33* const vehicle_rotation = reinterpret_cast<Matrix33*>(vehicle + 1116);
    Point3* const seat_offset = reinterpret_cast<Point3*>(vehicle + 1932 + seat * 12);
    if (holder != 0) {
        rotation = *vehicle_rotation;
        matTransform__FP6Point3P8Matrix33T0(&turned, &rotation, seat_offset);
        matRot__FP8Matrix33is(&rotation, 2,
            static_cast<short>(static_cast<int>(*reinterpret_cast<AliasedFloat*>(holder + 280) * 0x1.6c16c0p-8f * 0x1.fffc00p+14f)));
        matRot__FP8Matrix33is(&rotation, 0,
            static_cast<short>(static_cast<int>(*reinterpret_cast<AliasedFloat*>(holder + 284) * 0x1.6c16c0p-8f * 0x1.fffc00p+14f)));
        matTransform__FP6Point3P8Matrix33T0(&sum, &rotation, reinterpret_cast<Point3*>(holder + 288));
        sum.x = sum.x + turned.x;
        sum.y = sum.y + turned.y;
        sum.z = sum.z + turned.z;
    } else {
        matTransform__FP6Point3P8Matrix33T0(&sum, vehicle_rotation, seat_offset);
    }
    const Point3* const position = reinterpret_cast<Point3*>(vehicle + 16);
    out->x = position->x + sum.x;
    out->y = position->y + sum.y;
    out->z = position->z + sum.z;
}
