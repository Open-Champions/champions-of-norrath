// Observed camera spline scratch words, all zero:
//   pitch1 0x00486d80 and pitch2 0x00486d82, halfwords (2-byte gaps)
//   v1 0x008d5958 and v2 0x008d5968, 16-byte gaps each, taken as 12-byte vectors
// initPosition and moveCamera reference them; initPosition forms the addresses of v1 and v2
// (the symbol sizes are 0; the gaps are the bounds). Inferred: v1 and v2 are three-float vectors; pitch1 and pitch2 are angles.
// Unknown: whether the original declared a padded type. Neither v1 nor v2 is 16-aligned.
struct Point3 {
    float x, y, z;
};
extern "C" {
short pitch1;
short pitch2;
Point3 v1;
Point3 v2;
}
