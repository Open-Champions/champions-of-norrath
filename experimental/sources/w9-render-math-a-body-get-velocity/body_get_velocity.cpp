// Observed bodyGetVelocity__FP5_bodyP6Point3T1 at 0x0015a9f0, 280 bytes. Arguments:
// a0 = body, a1 = world point (3 singles), a2 = output (3 singles).
// Body words used (byte offsets): 0 one single, 16 three singles, 44 three singles,
// 140 three singles.
//  1. r = point - body[16..24], each component subtracted separately.
//  2. cross = body[140..148] x r:  x = w.y*r.z - w.z*r.y,  y = w.z*r.x - w.x*r.z,
//     z = w.x*r.y - w.y*r.x. It is written to the output.
//  3. The output is then read back and each component gets body[44 + 4k] * body[0]
//     added. The body words are loaded after the output store, so an output aliasing the
//     body would be seen.
// Inferred only: body[0] is an inverse mass, body[16] a position, body[44] linear
// momentum and body[140] angular velocity, because bodyInit (0x15bcd0) initialises them
// from mass, writes zero to the momenta and derives 140 from momentum 56 and the matrix at
// 104. The result is the velocity of a point carried by the body. Fresh compilation is
// not an original-byte match.
struct Point3 {
    float x, y, z;
};
struct Body {
    float inverse_mass;
    float unknown_4[3];
    Point3 position;
    unsigned char unknown_28[16];
    Point3 linear_momentum;
    unsigned char unknown_56[84];
    Point3 angular_velocity;
};

extern "C" void bodyGetVelocity__FP5_bodyP6Point3T1(const Body* body, const Point3* point, Point3* out) {
    const float rx = point->x - body->position.x;
    const float ry = point->y - body->position.y;
    const float rz = point->z - body->position.z;
    const float wx = body->angular_velocity.x;
    const float wy = body->angular_velocity.y;
    const float wz = body->angular_velocity.z;
    const float cx = wy * rz - wz * ry;
    const float cy = wz * rx - wx * rz;
    const float cz = wx * ry - wy * rx;
    out->x = cx;
    out->y = cy;
    out->z = cz;
    const float scale = body->inverse_mass;
    const float lx = body->linear_momentum.x * scale;
    const float ly = body->linear_momentum.y * scale;
    const float lz = body->linear_momentum.z * scale;
    out->x = out->x + lx;
    out->y = out->y + ly;
    out->z = out->z + lz;
}
