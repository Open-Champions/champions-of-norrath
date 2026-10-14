#pragma once
// Plain float layouts implied by the mangled names and by the byte offsets the
// matrix routines address: Matrix33 is nine floats, Matrix34 is twelve (three rows
// of three, then a translation row), Matrix44 is sixteen. Point3 is three floats.
struct Point3 {
    float x, y, z;
};
struct Matrix33 {
    float m[9];
};
struct Matrix34 {
    float m[12];
};
struct Matrix44 {
    float m[16];
};
