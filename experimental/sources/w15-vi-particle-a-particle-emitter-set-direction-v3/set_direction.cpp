// Observed: SetDirection__17VIParticleEmitterRC7VIVect3 at 0x010b64e8 is 516 bytes. Argument: the emitter and a direction d (three floats).
//  1. An up vector starts as (0, 1, 0). When |d . up| is greater than the float 0x3f7fbe76 (about 0.999), it becomes (0, 0, 1) instead.
//  2. C = up x d, with components (up.y*d.z - up.z*d.y, up.z*d.x - up.x*d.z, up.x*d.y - up.y*d.x). The first basis vector is C.
//     The second is d x C, with components (d.y*C.z - d.z*C.y, d.z*C.x - d.x*C.z, d.x*C.y - d.y*C.x). The third is d itself.
//  3. Each basis vector goes through Normalize__7VIVect3 in the order first, second, third.
//  4. A VIMatrix33 is constructed (__10VIMatrix33) on the stack and its nine floats are set row by row to the first, second and third
//     basis vectors. SetDirection__17VIParticleEmitterRC10VIMatrix33(emitter, matrix) runs, then the matrix destructor
//     (_$_10VIMatrix33 with flag 2). Nothing is returned.
// Inferred: this builds an orthonormal basis whose third row is the emitter direction and installs it as the emitter direction matrix.
// Unknown: the handedness convention of the rest of the engine, and what Normalize does for a zero-length vector (it is a separate
// routine and is not reproduced here).
extern "C" {
void* Normalize__7VIVect3(void* vector);
void __10VIMatrix33(void* matrix);
void _$_10VIMatrix33(void* matrix, int flags);
void SetDirection__17VIParticleEmitterRC10VIMatrix33(void* emitter, const void* matrix);

void SetDirection__17VIParticleEmitterRC7VIVect3(void* emitter, const float* d) {
    float first[3] = {0.0f, 0.0f, 0.0f};
    float second[3] = {0.0f, 1.0f, 0.0f};
    float third[3] = {0.0f, 0.0f, 0.0f};
    const float dx = d[0], dy = d[1], dz = d[2];
    if (__builtin_fabsf(dx * second[0] + dy * second[1] + dz * second[2]) > 0x1.ff7cecp-1f) {
        second[0] = 0.0f;
        second[1] = 0.0f;
        second[2] = 1.0f;
    }
    const float cx = second[1] * dz - second[2] * dy;
    const float cy = second[2] * dx - second[0] * dz;
    const float cz = second[0] * dy - second[1] * dx;
    first[0] = cx;
    first[1] = cy;
    first[2] = cz;
    second[0] = dy * cz - dz * cy;
    second[1] = dz * cx - dx * cz;
    second[2] = dx * cy - dy * cx;
    third[0] = dx;
    third[1] = dy;
    third[2] = dz;
    Normalize__7VIVect3(first);
    Normalize__7VIVect3(second);
    Normalize__7VIVect3(third);
    float matrix[9];
    __10VIMatrix33(matrix);
    matrix[0] = first[0];
    matrix[1] = first[1];
    matrix[2] = first[2];
    matrix[3] = second[0];
    matrix[4] = second[1];
    matrix[5] = second[2];
    matrix[6] = third[0];
    matrix[7] = third[1];
    matrix[8] = third[2];
    SetDirection__17VIParticleEmitterRC10VIMatrix33(emitter, matrix);
    _$_10VIMatrix33(matrix, 2);
}
}
