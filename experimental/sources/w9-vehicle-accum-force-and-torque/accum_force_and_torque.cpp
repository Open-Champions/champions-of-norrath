// Observed accumForceAndTorque__7VehicleP6Point3T1i at 0x00271c90, 468 bytes.
// Arguments: vehicle, point, force, flag. Steps:
//   scaled = point * S, with S the float 0x3cd20d20 (about 0.02564);
//   local  = matUntransform of the force by the 3x3 matrix at vehicle+1116
//            (matUntransform__FP6Point3P8Matrix33T0, output, matrix, input);
//   torque = scaled x local, with
//            torque.x = scaled.y*local.z - scaled.z*local.y,
//            torque.y = scaled.z*local.x - scaled.x*local.z,
//            torque.z = scaled.x*local.y - scaled.y*local.x.
// With flag zero the force (the original argument, not the local one) is added
// to the Point3 at vehicle+2004 and the torque to the Point3 at +1992; with a
// non-zero flag they go to +2016 and +2028 instead. Each add is accumulator +
// value, component by component. Nothing is returned. Nothing here was
// executed on the EE. Which direction each accumulator pair is used in is
// not established here.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);


extern "C" void accumForceAndTorque__7VehicleP6Point3T1i(unsigned char* vehicle, const Point3* point, const Point3* force, int flag) {
    Point3 local;
    Point3 scaled;
    Point3 torque;
    scaled.x = point->x * 0x1.a41a40p-6f;
    scaled.y = point->y * 0x1.a41a40p-6f;
    scaled.z = point->z * 0x1.a41a40p-6f;
    matUntransform__FP6Point3P8Matrix33T0(&local, vehicle + 1116, force);
    torque.x = scaled.y * local.z - scaled.z * local.y;
    torque.y = scaled.z * local.x - scaled.x * local.z;
    torque.z = scaled.x * local.y - scaled.y * local.x;
    Point3* const force_total = reinterpret_cast<Point3*>(vehicle + (flag ? 2016 : 2004));
    Point3* const torque_total = reinterpret_cast<Point3*>(vehicle + (flag ? 2028 : 1992));
    force_total->x = force_total->x + force->x;
    force_total->y = force_total->y + force->y;
    force_total->z = force_total->z + force->z;
    torque_total->x = torque_total->x + torque.x;
    torque_total->y = torque_total->y + torque.y;
    torque_total->z = torque_total->z + torque.z;
}
