// Observed modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1 at 0x0016c5c0, extent 192.
// Arguments: a model header and two Point3 outputs (each by address). The header byte at +39
// is an index k. With k = 0 the outputs are the fixed points (0, 4.5, 0) and (0, 38, 0) and the
// result is 0. Otherwise the record at header + k * 16 supplies the first output from its bytes
// 0..11 and the second from its bytes 12..23 (the two 12-byte values overlap the next record's
// first eight bytes), and the result is 1.
// Inferred: the two points are the base and the tip of a weapon, used to draw a trail, with
// built-in defaults for a model that has no weapon record. Unknown: the unit of the defaults
// and which record layout holds the 24 bytes.
struct Point3 {
    float x, y, z;
};
extern "C" {
int modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1(const unsigned char* model, Point3* base, Point3* tip) {
    unsigned index = model[39];
    if (index == 0) {
        base->x = 0.0f;
        base->y = 4.5f;
        base->z = 0.0f;
        tip->x = 0.0f;
        tip->y = 38.0f;
        tip->z = 0.0f;
        return 0;
    }
    const Point3* record = reinterpret_cast<const Point3*>(model + index * 16);
    *base = record[0];
    *tip = *reinterpret_cast<const Point3*>(model + index * 16 + 12);
    return 1;
}
}
