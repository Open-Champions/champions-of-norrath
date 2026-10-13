// Observed initPosition__Fii at 0x0027df00, 320 bytes in SLUS_205.65. Arguments: two signed
// indices, a (first) and b (second). Seeds the static transition records used by the
// interpolation code from the two tables of 12-byte vectors hvecs (0x008d5978) and hpos
// (0x008d5a08):
//   v1 = hvecs[a], v2 = hvecs[b], p1 = hpos[a], p2 = hpos[b]  (12-byte copies into the
//   four vectors at 0x008d5958, 0x008d5968, 0x008d5938 and 0x008d5948);
//   p1.z += 61.0f and p2.z += 61.0f;
//   every component of v1 and v2 is multiplied by 15.0f;
//   pitch1 = pitch2 = 1812 (halfwords at 0x00486d80 and 0x00486d82);
//   src = a and dst = b (words at 0x00486d00 and 0x00486d04).
// Nothing is returned. No bound is applied to the indices.
// Inferred: the vectors are position and tangent pairs for a Hermite-style move between two
// numbered points, from the neighbouring symbol names (hermiteTime and others); the +61 and
// 15 factors are used as given. Unknown: why the pitch halfwords are 1812, and the units.
struct Point3 { float x, y, z; };
extern Point3 hvecs[];
extern Point3 hpos[];
extern Point3 p1, p2, v1, v2;
extern short pitch1, pitch2;
extern int src, dst;

extern "C" void initPosition__Fii(int a, int b) {
    v1 = hvecs[a];
    v2 = hvecs[b];
    p1 = hpos[a];
    p2 = hpos[b];
    p1.z = p1.z + 61.0f;
    p2.z = p2.z + 61.0f;
    v1.x = v1.x * 15.0f;
    v1.y = v1.y * 15.0f;
    v1.z = v1.z * 15.0f;
    v2.x = v2.x * 15.0f;
    v2.y = v2.y * 15.0f;
    v2.z = v2.z * 15.0f;
    pitch1 = 1812;
    pitch2 = 1812;
    src = a;
    dst = b;
}
