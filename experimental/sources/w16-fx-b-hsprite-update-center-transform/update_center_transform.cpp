// Observed: UpdateCenterTransform__9VIHSpriteR7VIScene at 0x010618c8, 568 bytes. Arguments (this, scene). Locals: a three-float centre
// starting at (0,0,0), two 4x4 float matrices A and B (both zeroed inline, no constructor call) and a frame block of eight floats
// starting as quaternion (0,0,0,1), scale 1.0 and translation (0,0,0).
//  1. r = InterpTransAboutCenter__9VIHSpriteR7VISceneR7VIVect3R14VIHSpriteFrame(this, scene, centre, frame). When r is negative both
//     matrices are destroyed (_$_10VIMatrix44 with flag 2, B first then A) and -1 is returned with nothing else written.
//  2. Otherwise, with M = this+320: Scale__10VIMatrix44f(M, float at this+208); Translate__10VIMatrix44RC7VIVect3(A, negated centre);
//     Mul(B, M, A); Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(M, frame quaternion, frame scale, frame translation);
//     Mul(A, B, M); Translate(B, centre); Mul(M, A, B). The 64-bit result of GetTime__7VIScene(scene) is stored at this+192.
//     Both matrices are destroyed (B first then A) and zero is returned.
// Inferred: M becomes a transform of the sprite about its centre for the scene's current time; this+192 is that time stamp, which
// CalcTransAboutCenter compares to decide whether to call this routine.
// Unknown: the meaning of this+208 (a float scale) and the operand order convention of Mul.
extern "C" {
int InterpTransAboutCenter__9VIHSpriteR7VISceneR7VIVect3R14VIHSpriteFrame(void* self, void* scene, float* centre, void* frame);
void Scale__10VIMatrix44f(void* matrix, float s);
void Translate__10VIMatrix44RC7VIVect3(void* matrix, const float* v);
void Mul__10VIMatrix44RC10VIMatrix44T1(void* out, const void* a, const void* b);
void Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(void* matrix, const float* quat, float scale, const float* translation);
long long GetTime__7VIScene(void* scene);
void _$_10VIMatrix44(void* matrix, int flags);

int UpdateCenterTransform__9VIHSpriteR7VIScene(char* self, void* scene) {
    float centre[3] = {0.0f, 0.0f, 0.0f};
    float a[16], b[16];
    for (int i = 0; i < 16; i++) a[i] = 0.0f;
    for (int i = 0; i < 16; i++) b[i] = 0.0f;
    float frame[8] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};
    if (InterpTransAboutCenter__9VIHSpriteR7VISceneR7VIVect3R14VIHSpriteFrame(self, scene, centre, frame) < 0) {
        _$_10VIMatrix44(b, 2);
        _$_10VIMatrix44(a, 2);
        return -1;
    }
    char* m = self + 320;
    Scale__10VIMatrix44f(m, *(float*)(self + 208));
    float negated[3] = {-centre[0], -centre[1], -centre[2]};
    Translate__10VIMatrix44RC7VIVect3(a, negated);
    Mul__10VIMatrix44RC10VIMatrix44T1(b, m, a);
    Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(m, frame, frame[4], frame + 5);
    Mul__10VIMatrix44RC10VIMatrix44T1(a, b, m);
    Translate__10VIMatrix44RC7VIVect3(b, centre);
    Mul__10VIMatrix44RC10VIMatrix44T1(m, a, b);
    *(long long*)(self + 192) = GetTime__7VIScene(scene);
    _$_10VIMatrix44(b, 2);
    _$_10VIMatrix44(a, 2);
    return 0;
}
}
