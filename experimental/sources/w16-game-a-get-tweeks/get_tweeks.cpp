// Observed getTweeks__FiPsT1P6Point3 at 0x0027e7d0, 800 bytes, one call, 336-byte frame. Arguments: a0 = index, a1 and a2 =
// short outputs, a3 = vector output.
//  1. A table of ten 16-byte records is built on the stack: two shorts then three singles. Index is used as a plain
//     multiplier (index*16), unchecked.
//  2. *a1 = second short of the record, *a2 = first short, then
//     matUntransform34Vec__FP6Point3P8Matrix34T0(a3, &worldCameraMatrix, &record.vector) is called and its result returned
//     as the function result (the original leaves the callee's v0 in place).
// Table values (short0, short1, x, y, z), observed in the immediate constants:
//   0: 991 -935 -18 -6 3      1: 668 -4068 12 2 1      2: 1194 0 -21 -5 11     3: 0 0 -21 9 8       4: 570 176 -29 3 0
//   5: 0 0 -22 5 7            6: 0 0 -28 4 -7          7: 0 0 -24 4 -1         8: 716 281 -27 2 0   9: 0 0 -24 9 2
// Inferred only: the callee takes a point through the inverse of the camera matrix; which of the shorts is an angle is
// unknown. Behavior for an index outside 0..9 is not specified (the original reads beyond the table).
struct Point3 {
    float x, y, z;
};
struct Matrix34;

extern "C" {
extern Matrix34 worldCameraMatrix;
Point3* matUntransform34Vec__FP6Point3P8Matrix34T0(Point3* out, const Matrix34* matrix, const Point3* in);
}

namespace {
struct Record {
    short first;
    short second;
    Point3 vector;
};
}  // namespace

extern "C" Point3* getTweeks__FiPsT1P6Point3(int index, short* second_out, short* first_out, Point3* out) {
    const Record table[10] = {
        {991, -935, {-18.0f, -6.0f, 3.0f}},  {668, -4068, {12.0f, 2.0f, 1.0f}}, {1194, 0, {-21.0f, -5.0f, 11.0f}},
        {0, 0, {-21.0f, 9.0f, 8.0f}},        {570, 176, {-29.0f, 3.0f, 0.0f}},  {0, 0, {-22.0f, 5.0f, 7.0f}},
        {0, 0, {-28.0f, 4.0f, -7.0f}},       {0, 0, {-24.0f, 4.0f, -1.0f}},     {716, 281, {-27.0f, 2.0f, 0.0f}},
        {0, 0, {-24.0f, 9.0f, 2.0f}},
    };
    const Record* record = &table[index];
    *second_out = record->second;
    *first_out = record->first;
    return matUntransform34Vec__FP6Point3P8Matrix34T0(out, &worldCameraMatrix, &record->vector);
}
