// Observed modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f
// at 0x0016ca18, 228 bytes. Arguments: a model header, an animation state (may be
// null), an entry number, a destination 3x4 matrix and a float scale. No result.
// The header byte at +34 selects a table at header + (byte << 4); each table entry
// is 52 bytes: a 32-bit word (passed as the bone argument to animGetBoneMatrix)
// followed by a 3x4 matrix at entry + 4.
//   local = animation state ? animGetBoneMatrix(state, entry.word) : identity;
//   out = entry.matrix * local   (matMul(out, entry.matrix, local));
//   s = identity with matScale(s, scale); out = out * s.
// Inferred: an attachment point matrix on a bone, scaled uniformly. The meaning of
// the matMul operand order is as called; the multiply convention is unknown here.
struct Matrix34 {
    float m[12];
};
struct AnimationState;
struct _modelHeader;

extern "C" void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState* state, int bone, Matrix34* out);
extern "C" void matIdent__FP8Matrix34(Matrix34* m);
extern "C" void matScale__FP8Matrix34f(Matrix34* m, float scale);
extern "C" void matMul__FP8Matrix34N20(Matrix34* dst, const Matrix34* a, const Matrix34* b);

extern "C" void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(_modelHeader* header,
                                                                                        AnimationState* state,
                                                                                        int index, Matrix34* out,
                                                                                        float scale) {
    unsigned char* base = reinterpret_cast<unsigned char*>(header);
    unsigned char* table = base + (static_cast<unsigned>(base[34]) << 4);
    unsigned char* entry = table + index * 52;
    Matrix34 local;
    if (state != 0) {
        animGetBoneMatrix__FP14AnimationStateiP8Matrix34(state, *reinterpret_cast<int*>(entry), &local);
    } else {
        matIdent__FP8Matrix34(&local);
    }
    matMul__FP8Matrix34N20(out, reinterpret_cast<Matrix34*>(entry + 4), &local);
    Matrix34 scaled;
    matIdent__FP8Matrix34(&scaled);
    matScale__FP8Matrix34f(&scaled, scale);
    matMul__FP8Matrix34N20(out, out, &scaled);
}
