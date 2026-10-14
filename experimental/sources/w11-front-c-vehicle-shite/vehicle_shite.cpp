// Observed shite at 0x007d74e0, bss, symbol size 0; the next symbol, sparkParticle, is 16
// bytes later. The VehicleBodySphere constructor passes its address as the second argument
// of a call. The name is the image's own. Inferred: a three-float vector from the 16-byte
// slot. Unknown: its purpose and the callee's use of it.
struct Point3 {
    float x, y, z;
};
extern "C" {
Point3 shite;
}
