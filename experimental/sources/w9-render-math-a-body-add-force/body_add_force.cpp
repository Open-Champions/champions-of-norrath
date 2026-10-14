// Observed bodyAddForce__FP5_bodyP6Point3N31 at 0x0015b600, 248 bytes. Arguments:
// a0 = body, a1 = world point, a2 = force, a3 = force accumulator, t0 = torque
// accumulator; each of the last four is three singles. Mangling N31 repeats the second
// parameter type three more times.
//  1. force accumulator += force, component by component. The force words are read
//     again after that, so an accumulator aliasing the force is seen doubled.
//  2. r = point - body[16..24] (three singles at body offset 16).
//  3. torque = r x force:  x = r.y*F.z - r.z*F.y,  y = r.z*F.x - r.x*F.z,
//     z = r.x*F.y - r.y*F.x. It is added component by component to the torque
//     accumulator. Nothing else in the body is read or written.
// Inferred only: body[16] is the body position. Fresh compilation is not an original-byte
// match.
struct Point3 {
    float x, y, z;
};
struct Body {
    unsigned char unknown_0[16];
    Point3 position;
};

extern "C" void bodyAddForce__FP5_bodyP6Point3N31(const Body* body, const Point3* point, const Point3* force,
                                                  Point3* force_accumulator, Point3* torque_accumulator) {
    force_accumulator->x = force_accumulator->x + force->x;
    force_accumulator->y = force_accumulator->y + force->y;
    force_accumulator->z = force_accumulator->z + force->z;
    const float rx = point->x - body->position.x;
    const float ry = point->y - body->position.y;
    const float rz = point->z - body->position.z;
    const float fx = force->x;
    const float fy = force->y;
    const float fz = force->z;
    const float tx = ry * fz - rz * fy;
    const float ty = rz * fx - rx * fz;
    const float tz = rx * fy - ry * fx;
    torque_accumulator->x = torque_accumulator->x + tx;
    torque_accumulator->y = torque_accumulator->y + ty;
    torque_accumulator->z = torque_accumulator->z + tz;
}
