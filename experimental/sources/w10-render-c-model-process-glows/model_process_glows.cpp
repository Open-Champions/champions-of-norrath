// Observed modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34 at
// 0x00169110, 452 bytes. Arguments: a model header, an animation state (may be
// null) and a world matrix. No result.
// Does nothing unless bit 0x40 of the header halfword at +16 is set. Otherwise it
// walks 16-byte records starting at header + (byte at +35 << 4) until a record whose
// first signed byte is -1. A scratch matrix is first set to identity. For a record:
//   p = (signed bytes at +8, +9, +10) * (1/128);
//   with an animation state: animGetBoneMatrix(state, signed byte at +15, scratch)
//   and p = scratch * p (matTransform34Vec);
//   p = world * p (matTransform34Vec);
//   d = (p.x * c[2] + p.y * c[5]) + p.z * c[8], c being worldCameraMatrix as floats;
//   when d is not below zero: q = (halfwords at +2, +4, +6, signed) * (1/16),
//   transformed by the scratch matrix when there is an animation state and then by
//   the world matrix (matTransform).
// q is never stored anywhere the original keeps, so after those transforms the
// record produces no other effect; only the callee side effects remain. Inferred:
// a facing test and a glow position per record; the original may have dropped the
// code that used q. Unknown: what the records describe.
struct Point3 {
    float x, y, z;
};
struct Matrix34 {
    float m[12];
};
struct AnimationState;
struct _modelHeader;

extern "C" float worldCameraMatrix[12];
extern "C" void matIdent__FP8Matrix34(Matrix34* m);
extern "C" void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState* state, int bone, Matrix34* out);
extern "C" Point3* matTransform34Vec__FP6Point3P8Matrix34T0(Point3* out, const Matrix34* matrix, const Point3* in);
extern "C" void matTransform__FP6Point3P8Matrix34T0(Point3* out, const Matrix34* matrix, const Point3* in);

extern "C" void modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34(_modelHeader* header,
                                                                               AnimationState* state,
                                                                               Matrix34* world) {
    const unsigned char* base = reinterpret_cast<const unsigned char*>(header);
    if ((*reinterpret_cast<const unsigned short*>(base + 16) & 0x40) == 0) return;
    const signed char* rec = reinterpret_cast<const signed char*>(base + (static_cast<unsigned>(base[35]) << 4));
    Matrix34 scratch;
    matIdent__FP8Matrix34(&scratch);
    if (rec[0] == -1) return;
    do {
        Point3 p;
        p.x = static_cast<float>(rec[8]) * 0.0078125f;
        p.y = static_cast<float>(rec[9]) * 0.0078125f;
        p.z = static_cast<float>(rec[10]) * 0.0078125f;
        if (state != 0) {
            animGetBoneMatrix__FP14AnimationStateiP8Matrix34(state, rec[15], &scratch);
            matTransform34Vec__FP6Point3P8Matrix34T0(&p, &scratch, &p);
        }
        matTransform34Vec__FP6Point3P8Matrix34T0(&p, world, &p);
        float d = (p.x * worldCameraMatrix[2] + p.y * worldCameraMatrix[5]) + p.z * worldCameraMatrix[8];
        if (!(d < 0.0f)) {
            const short* h = reinterpret_cast<const short*>(rec);
            Point3 q;
            q.x = static_cast<float>(h[1]) * 0.0625f;
            q.y = static_cast<float>(h[2]) * 0.0625f;
            q.z = static_cast<float>(h[3]) * 0.0625f;
            if (state != 0) matTransform__FP6Point3P8Matrix34T0(&q, &scratch, &q);
            matTransform__FP6Point3P8Matrix34T0(&q, world, &q);
        }
        rec += 16;
    } while (rec[0] != -1);
}
