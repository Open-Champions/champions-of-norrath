// Observed worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2 at 0x001755f0, 480 bytes.
// Arguments: world header, door index, minimum output, maximum output (both three floats).
// A negative index zeroes both outputs. Otherwise the door record is at (header+84) + 28*index;
// its halfword at +4 is a record count and its word at +0 is a first index into the 60-byte
// record array at header+36. Floats at record+8/+12/+16 are folded into a running minimum
// (strict less-than) that starts at +1000000.0, and floats at +20/+24/+28 into a running
// maximum (strict greater-than) that starts at -1000000.0. With a count of zero or less the
// starting values are used. Afterwards every minimum component gets +48.0 and every maximum
// component gets -48.0 and the two results are written out as x, y, z.
// Inferred: the records carry axis-aligned boxes and the 48.0 is an inward margin.
// Unknown: the record meaning beyond those six floats.
struct _worldHeader;
struct Point3 { float x, y, z; };

extern "C" void worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2(
    _worldHeader* world, int index, Point3* low, Point3* high) {
    if (index < 0) {
        low->x = 0.0f; low->z = 0.0f; low->y = 0.0f;
        high->x = 0.0f; high->z = 0.0f; high->y = 0.0f;
        return;
    }
    unsigned char* header = reinterpret_cast<unsigned char*>(world);
    unsigned char* door = *reinterpret_cast<unsigned char**>(header + 84) + index * 28;
    Point3 lo = {1000000.0f, 1000000.0f, 1000000.0f};
    Point3 hi = {-1000000.0f, -1000000.0f, -1000000.0f};
    int count = *reinterpret_cast<short*>(door + 4);
    for (int i = 0; i < count; ++i) {
        unsigned char* record = *reinterpret_cast<unsigned char**>(header + 36) +
                                (*reinterpret_cast<int*>(door) + i) * 60;
        float* f = reinterpret_cast<float*>(record);
        if (f[2] < lo.x) lo.x = f[2];
        if (f[3] < lo.y) lo.y = f[3];
        if (f[4] < lo.z) lo.z = f[4];
        if (hi.x < f[5]) hi.x = f[5];
        if (hi.y < f[6]) hi.y = f[6];
        if (hi.z < f[7]) hi.z = f[7];
    }
    lo.x += 48.0f; lo.y += 48.0f; lo.z += 48.0f;
    hi.x -= 48.0f; hi.y -= 48.0f; hi.z -= 48.0f;
    *low = lo;
    *high = hi;
}
