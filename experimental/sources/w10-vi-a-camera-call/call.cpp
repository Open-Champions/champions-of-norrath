// Observed: __cl__8VICameraR7VIVect3T1 (VICamera::operator()(VIVect3&, VIVect3&))
// at 0x010871b0 is 172 bytes. Arguments: this, first vector a, second vector b.
// It builds three zeroed VIVect3 locals (three floats each, 16 bytes apart in
// the frame), then calls
//   CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(this, b, x, y, z)
// followed by
//   Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44(this, a, x, y, z, this + 112)
// and returns -1 if the Update result is negative, otherwise 0.
// Inferred: this + 112 is a VIMatrix44 member that receives the view result;
// the mangled parameter list is the only evidence for the types.
// Unknown: what a and b mean (a position and a look target are plausible) and the
// 16-byte frame spacing of the locals as an alignment requirement.
extern "C" {
struct VIVect3Local {
    float x, y, z;
};
int CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(void* camera, void* point, VIVect3Local* x, VIVect3Local* y, VIVect3Local* z);
int Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44(void* camera, void* point, VIVect3Local* x, VIVect3Local* y, VIVect3Local* z, void* matrix);

int __cl__8VICameraR7VIVect3T1(void* camera, void* a, void* b) {
    alignas(16) VIVect3Local x = {0.0f, 0.0f, 0.0f};
    alignas(16) VIVect3Local y = {0.0f, 0.0f, 0.0f};
    alignas(16) VIVect3Local z = {0.0f, 0.0f, 0.0f};
    CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(camera, b, &x, &y, &z);
    int result = Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44(
        camera, a, &x, &y, &z, static_cast<unsigned char*>(camera) + 112);
    return result < 0 ? -1 : 0;
}
}
