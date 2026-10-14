// Observed: RecomputeStaticBBox__17VIParticleEmitter at 0x010b7530 is 1928 bytes. Argument: the emitter E. Nothing is returned.
// Locals: a 4x4 float matrix M, zero-filled and destroyed on return (_$_10VIMatrix44 with flag 2), four zeroed float triples V1..V4,
// and a 4-float table T copied from read-only data: -1.0f, -0.33f (0xbea8f5c2), 0.33f (0x3ea8f5c2), 1.0f.
//  1. Floats at E+916..924 and E+928..936 are cleared. H = 0.5f * max(E[236] + |E[240]|, E[228] + |E[232]|) and
//     K = (E[200] + |E[204]|) * (E[208] + |E[212]|).
//  2. For i = 0..11: V1 = (0,0,0) and V2 = (floats at E+848..856) * K. The stage 'rotate' of (angle base, index n, in, out) is: with
//     m = n mod 3, an angle vector is zero except component m, which is min(pi, float at base + 4*m) * T[i & 3] (pi = 0x40490fda);
//     RotateHPR__10VIMatrix44RC7VIVect3(M, angle vector) and Mul__C10VIMatrix44RC7VIVect3R7VIVect3(M, in, out) follow.
//       stage 1: when the word at E+888 is non-zero, V4 = V2, then rotate(E+860, i, V4, V2).
//       stage 2: when the words at E+880 or E+884 are non-zero, V3 = V1 + (floats at E+824..832), V4 = V2, rotate(E+836, i+4, V3, V1),
//                then Mul(M, V4, V2).
//       stage 3: the same with the words at E+872 or E+876, the floats at E+800..808, rotate(E+812, i+8, V3, V1), then Mul(M, V4, V2).
//     C = V1 + V2. The box at E+916 (minimum) and E+928 (maximum) is widened to contain C, then V1, using min.s and max.s in the
//     operand order min(old, new) and max(new, old).
//  3. After the loop H is subtracted from the three minimum floats and added to the three maximum floats.
// Inferred: E+916/928 is a static bounding box of the emitter; stages 1..3 are three independent motion terms and the 12 iterations
// sample the phases of a rotating box. Which float of the box terms is an amplitude or a speed is not established.
// Unknown: what the words at E+872..888 hold (only tested against zero), the meaning of E+800..856, and the VIMatrix44 and VIVect3 layouts
// beyond 16 and 3 floats.
extern "C" {
void* RotateHPR__10VIMatrix44RC7VIVect3(void* matrix, const void* angles);
void* Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const void* in, void* out);
void _$_10VIMatrix44(void* matrix, int flags);

#if defined(__mips__)
static float min_s(float a, float b) {
    float result;
    __asm__ volatile ("min.s %0, %1, %2" : "=f" (result) : "f" (a), "f" (b));
    return result;
}
static float max_s(float a, float b) {
    float result;
    __asm__ volatile ("max.s %0, %1, %2" : "=f" (result) : "f" (a), "f" (b));
    return result;
}
#else
static float min_s(float a, float b) { return a < b ? a : b; }
static float max_s(float a, float b) { return a > b ? a : b; }
#endif

static float absolute(float x) { return __builtin_fabsf(x); }

static void rotate(void* matrix, const unsigned char* base, int index, const float* table, int phase, const float* in, float* out) {
    const int m = index % 3;
    float angles[3] = {0.0f, 0.0f, 0.0f};
    angles[m] = min_s(3.1415927f, *reinterpret_cast<const float*>(base + 4 * m)) * table[phase];
    RotateHPR__10VIMatrix44RC7VIVect3(matrix, angles);
    Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, in, out);
}

void RecomputeStaticBBox__17VIParticleEmitter(unsigned char* self) {
    alignas(16) float matrix[16];
    float v1[3], v2[3], v3[3], v4[3];
    const float table[4] = {-1.0f, -0.33f, 0.33f, 1.0f};
    float* const low = reinterpret_cast<float*>(self + 916);
    float* const high = reinterpret_cast<float*>(self + 928);
    const float* const stage1_vector = reinterpret_cast<const float*>(self + 848);
    const float* const stage2_vector = reinterpret_cast<const float*>(self + 824);
    const float* const stage3_vector = reinterpret_cast<const float*>(self + 800);
    for (int i = 0; i < 3; ++i) low[i] = 0.0f;
    for (int i = 0; i < 3; ++i) high[i] = 0.0f;
    const float half = max_s(*reinterpret_cast<float*>(self + 236) + absolute(*reinterpret_cast<float*>(self + 240)),
                             *reinterpret_cast<float*>(self + 228) + absolute(*reinterpret_cast<float*>(self + 232))) * 0.5f;
    for (int i = 0; i < 16; ++i) matrix[i] = 0.0f;
    for (int i = 0; i < 3; ++i) { v1[i] = 0.0f; v2[i] = 0.0f; v3[i] = 0.0f; v4[i] = 0.0f; }
    const float scale = (*reinterpret_cast<float*>(self + 200) + absolute(*reinterpret_cast<float*>(self + 204))) *
                        (*reinterpret_cast<float*>(self + 208) + absolute(*reinterpret_cast<float*>(self + 212)));
    for (int i = 0; i < 12; ++i) {
        for (int k = 0; k < 3; ++k) { v1[k] = 0.0f; v2[k] = stage1_vector[k] * scale; }
        const int phase = i & 3;
        if (*reinterpret_cast<unsigned*>(self + 888) != 0) {
            for (int k = 0; k < 3; ++k) v4[k] = v2[k];
            rotate(matrix, self + 860, i, table, phase, v4, v2);
        }
        if ((*reinterpret_cast<unsigned*>(self + 884) | *reinterpret_cast<unsigned*>(self + 880)) != 0) {
            for (int k = 0; k < 3; ++k) { v3[k] = v1[k] + stage2_vector[k]; v4[k] = v2[k]; }
            rotate(matrix, self + 836, i + 4, table, phase, v3, v1);
            Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, v4, v2);
        }
        if ((*reinterpret_cast<unsigned*>(self + 876) | *reinterpret_cast<unsigned*>(self + 872)) != 0) {
            for (int k = 0; k < 3; ++k) { v3[k] = v1[k] + stage3_vector[k]; v4[k] = v2[k]; }
            rotate(matrix, self + 812, i + 8, table, phase, v3, v1);
            Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, v4, v2);
        }
        const float c0 = v1[0] + v2[0];
        const float c1 = v1[1] + v2[1];
        const float c2 = v1[2] + v2[2];
        float lo0 = min_s(low[0], c0), hi0 = max_s(c0, high[0]);
        float lo1 = min_s(low[1], c1), lo2 = min_s(low[2], c2);
        float hi1 = max_s(c1, high[1]), hi2 = max_s(c2, high[2]);
        hi0 = max_s(v1[0], hi0);
        hi1 = max_s(v1[1], hi1);
        lo0 = min_s(lo0, v1[0]);
        hi2 = max_s(v1[2], hi2);
        lo1 = min_s(lo1, v1[1]);
        lo2 = min_s(lo2, v1[2]);
        low[0] = lo0; low[1] = lo1; low[2] = lo2;
        high[0] = hi0; high[1] = hi1; high[2] = hi2;
    }
    for (int i = 0; i < 3; ++i) low[i] = low[i] - half;
    for (int i = 0; i < 3; ++i) high[i] = high[i] + half;
    _$_10VIMatrix44(matrix, 2);
}
}
