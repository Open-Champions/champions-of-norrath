// Observed: Update__17VIParticleEmitter at 0x010b66f0 is 2,856 bytes.
// Returns -1 at once when the word at emitter + 80 (system) or + 84 (definition) is zero.
// Otherwise (offsets below are bytes from the emitter; the attribute copy that Create stores
// at emitter + 188 is named by its own offset a = e - 188):
//  1. calls BlendMotifs__17VIParticleEmitter(emitter); dt is the float at system + 16;
//  2. two rand() calls give r1, r2; spawn = (float at 192 + ((float)r1 - (float)r2) * 2^-31 *
//     float at 196) * dt; if the float at 792 is non-zero, spawn += Distance(emitter + 16,
//     emitter + 28) * float at 792; spawn += float at 76. count = (int)spawn, the float at 76
//     is set to spawn - (float)count, inverse = 1.0f / (float)count;
//  3. pos = the float triple at 28, step = (triple at 16 - triple at 28) * inverse; a 64-byte
//     matrix temporary is zero-filled and four float triples A, B, C, D are zeroed;
//  4. TextureConfiguration__C22VIParticleDefinitionEx(definition) selects (base, divisor):
//     1 -> (0, 1), 2 -> (5, 2), 3 -> (1, 3), 4 -> (1, 4);
//  5. up to count times: node = RemHead__7VILList(system + 64), stopping when it is null.
//     For each node (a particle record):
//     - node + 32 (float triple) = pos, or when the word at 892 is non-zero,
//       rotate(pattern[rand() % word at 896] ) via __ml__(VIVect3, VIMatrix33 at emitter + 40)
//       plus pos; pos += step; AddTail(emitter + 160, node); node + 16 = 0; the count word at
//       172 is incremented before the next rand;
//     - lifespan L = float at 200 + ((r1 - r2) * 2^-31) * float at 204 (two rands), kept
//       as 0 when negative or NaN and capped at 1000.0f; node + 20 = 1.0f / L, node + 24 =
//       float at 188, node + 96 = float at 908, node + 28 = float at 796;
//     - node + 100 = 0 when base is 0, else base + rand() % divisor;
//     - node + 88 = (float at 228 + ((r1 - r2) * 2^-31) * float at 232) * 0.5f;
//       node + 92 = ((float at 236 + ((r1 - r2) * 2^-31) * float at 240) * 0.5f - node + 88)
//       * node + 20;
//     - B = float triple at 848, A = 0; if the word at 888 is non-zero, D = B, a random
//       heading/pitch/roll triple (floats at 860, 864, 868 times (r1 - r2) * 2^-31, six rands)
//       is fed to RotateHPR__10VIMatrix44RC7VIVect3, and B = matrix * D;
//     - if either word at 884 or 880 is non-zero, C = A + triple at 824, D = B, the same
//       random triple with floats at 836, 840, 844, RotateHPR, then A = matrix * C and
//       B = matrix * D; the same again with words 876/872, triple at 800 and floats 812,
//       816, 820 (this block runs after the first);
//     - B = B * VIMatrix33 at 40 and A = A * VIMatrix33 at 40 (__ml__ calls, B first);
//       node + 32 += A; speed = float at 208 + ((r1 - r2) * 2^-31) * float at 212; B *= speed;
//       node + 44 = B;
//     - if the float at 788 is non-zero: D = (triple at 16 - triple at 28) * (1.0f / dt) * the
//       float at 788, and node + 44 += D.
//  6. calls the VIMatrix44 destructor (_$_10VIMatrix44) on the temporary with flag 2 and returns 0.
// Inferred: attribute copy offsets match the setter functions in this package (a+4 birth rate,
// a+8 variance, a+12 lifespan, a+20 velocity, a+40 start size, a+48 end size, a+600 inherit-velocity, a+604 delta-spawn, a+608 gradient repeat, a+612/624 inner
// offset/hpr var with flags a+684/688, a+636/648 outer with flags a+692/696, a+660 nozzle axis,
// a+672 nozzle hpr var with flag a+700, a+720 gravity), so this routine emits particles with
// those parameters.
// Unknown: the particle record's full layout (only the stored offsets above are observed);
// the pattern array at word 892 (12-byte entries) and its count at 896; the original leaves
// the (base, divisor) pair uninitialised for texture-configuration values outside 1..4 and this
// source zeroes both, which makes the node + 100 store zero there; hardware rand behaviour.
extern "C" {
void BlendMotifs__17VIParticleEmitter(void* emitter);
int rand();
float Distance__C7VIVect3RC7VIVect3(const void* a, const void* b);
int TextureConfiguration__C22VIParticleDefinitionEx(const void* definition);
void __ml__FRC7VIVect3RC10VIMatrix33(void* result, const void* vector, const void* matrix);
void RotateHPR__10VIMatrix44RC7VIVect3(void* matrix, const void* hpr);
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const void* in, void* out);
void AddTail__7VILListP7VILNode(void* list, void* node);
void* RemHead__7VILList(void* list);
void _$_10VIMatrix44(void* matrix, int flags);

// A host check defines NORRATH_HOST_ARENA because a 64-bit host cannot keep pointers in
// 4-byte words; the words then hold arena offsets.
#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define POINTER_OF(word) norrath_host_pointer(word)
#else
#define POINTER_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

static inline float& F(unsigned char* base, int offset) { return *reinterpret_cast<float*>(base + offset); }
static inline unsigned& W(unsigned char* base, int offset) { return *reinterpret_cast<unsigned*>(base + offset); }

// (r1 - r2) * 2^-31 from two consecutive rand() calls.
static inline float centered_random() {
    int first = rand();
    int second = rand();
    return (static_cast<float>(first) - static_cast<float>(second)) * 0x1p-31f;
}

// The random heading/pitch/roll triple scaled by the three floats at `scale` (six rand calls).
static inline void random_hpr(float* hpr, unsigned char* scale) {
    float x = F(scale, 0) * centered_random();
    float y = F(scale, 4) * centered_random();
    float z = F(scale, 8) * centered_random();
    hpr[0] = x;
    hpr[1] = y;
    hpr[2] = z;
}

// One offset/rotation block: C = A + offset, D = B, rotate by a random hpr, A = M*C, B = M*D.
static inline void offset_block(unsigned char* matrix, float* A, float* B, float* C, float* D,
                                unsigned char* offset, unsigned char* hpr_scale) {
    C[0] = A[0];
    C[1] = A[1];
    C[2] = A[2];
    C[0] = C[0] + F(offset, 0);
    C[1] = C[1] + F(offset, 4);
    C[2] = C[2] + F(offset, 8);
    D[0] = B[0];
    D[1] = B[1];
    D[2] = B[2];
    float hpr[3];
    random_hpr(hpr, hpr_scale);
    RotateHPR__10VIMatrix44RC7VIVect3(matrix, hpr);
    Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, C, A);
    Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, D, B);
}

int Update__17VIParticleEmitter(unsigned char* e) {
    if (W(e, 80) == 0 || W(e, 84) == 0) return -1;
    BlendMotifs__17VIParticleEmitter(e);
    unsigned char* system = POINTER_OF(W(e, 80));
    float dt = F(system, 16);
    float inverse_dt = 1.0f / dt;
    float birth = F(e, 192) + centered_random() * F(e, 196);
    float spawn = birth * dt;
    unsigned char* current = e + 16;
    unsigned char* last = e + 28;
    if (F(e, 792) != 0.0f) {
        float travelled = Distance__C7VIVect3RC7VIVect3(current, last);
        spawn = spawn + travelled * F(e, 792);
    }
    spawn = spawn + F(e, 76);
    int count = static_cast<int>(spawn);
    float whole = static_cast<float>(count);
    F(e, 76) = spawn - whole;
    float inverse_count = 1.0f / whole;

    float pos[3];
    float step[3];
    pos[0] = F(last, 0);
    pos[1] = F(last, 4);
    pos[2] = F(last, 8);
    step[0] = (F(current, 0) - pos[0]) * inverse_count;
    step[1] = (F(current, 4) - pos[1]) * inverse_count;
    step[2] = (F(current, 8) - pos[2]) * inverse_count;

    unsigned char matrix[64];
    for (int i = 0; i < 64; i++) matrix[i] = 0;
    float A[3] = {0.0f, 0.0f, 0.0f};
    float B[3] = {0.0f, 0.0f, 0.0f};
    float C[3] = {0.0f, 0.0f, 0.0f};
    float D[3] = {0.0f, 0.0f, 0.0f};

    int base = 0;
    int divisor = 0;
    switch (TextureConfiguration__C22VIParticleDefinitionEx(POINTER_OF(W(e, 84)))) {
    case 1: base = 0; divisor = 1; break;
    case 2: base = 5; divisor = 2; break;
    case 3: base = 1; divisor = 3; break;
    case 4: base = 1; divisor = 4; break;
    default: break;
    }

    for (int i = 0; i < count; i++) {
        unsigned char* node = static_cast<unsigned char*>(RemHead__7VILList(POINTER_OF(W(e, 80)) + 64));
        if (node == 0) break;
        float* position = reinterpret_cast<float*>(node + 32);
        if (W(e, 892) != 0) {
            int pick = rand() % static_cast<int>(W(e, 896));
            const float* entry = reinterpret_cast<const float*>(POINTER_OF(W(e, 892)) + pick * 12);
            position[0] = entry[0];
            position[1] = entry[1];
            position[2] = entry[2];
            float rotated[3];
            __ml__FRC7VIVect3RC10VIMatrix33(rotated, position, e + 40);
            position[0] = rotated[0];
            position[1] = rotated[1];
            position[2] = rotated[2];
            position[0] = position[0] + pos[0];
            position[1] = position[1] + pos[1];
            position[2] = position[2] + pos[2];
        } else {
            position[0] = pos[0];
            position[1] = pos[1];
            position[2] = pos[2];
        }
        pos[0] = pos[0] + step[0];
        pos[1] = pos[1] + step[1];
        pos[2] = pos[2] + step[2];
        AddTail__7VILListP7VILNode(e + 160, node);
        W(node, 16) = 0;
        W(e, 172) = W(e, 172) + 1;

        float lifespan = F(e, 200) + centered_random() * F(e, 204);
        if (0.0f <= lifespan) {
            if (lifespan > 1000.0f) lifespan = 1000.0f;
        } else {
            lifespan = 0.0f;
        }
        F(node, 20) = 1.0f / lifespan;
        F(node, 24) = F(e, 188);
        F(node, 96) = F(e, 908);
        if (base == 0) {
            W(node, 100) = 0;
        } else {
            int frame = rand() % divisor;
            W(node, 100) = static_cast<unsigned>(base + frame);
        }
        F(node, 28) = F(e, 796);
        F(node, 88) = (F(e, 228) + centered_random() * F(e, 232)) * 0.5f;
        F(node, 92) = ((F(e, 236) + centered_random() * F(e, 240)) * 0.5f - F(node, 88)) * F(node, 20);

        A[0] = 0.0f;
        A[1] = 0.0f;
        A[2] = 0.0f;
        B[0] = F(e, 848);
        B[1] = F(e, 852);
        B[2] = F(e, 856);
        if (W(e, 888) != 0) {
            D[0] = B[0];
            D[1] = B[1];
            D[2] = B[2];
            float hpr[3];
            random_hpr(hpr, e + 860);
            RotateHPR__10VIMatrix44RC7VIVect3(matrix, hpr);
            Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, D, B);
        }
        if ((W(e, 884) | W(e, 880)) != 0) offset_block(matrix, A, B, C, D, e + 824, e + 836);
        if ((W(e, 876) | W(e, 872)) != 0) offset_block(matrix, A, B, C, D, e + 800, e + 812);

        float transformed[3];
        __ml__FRC7VIVect3RC10VIMatrix33(transformed, B, e + 40);
        B[0] = transformed[0];
        B[1] = transformed[1];
        B[2] = transformed[2];
        __ml__FRC7VIVect3RC10VIMatrix33(transformed, A, e + 40);
        A[0] = transformed[0];
        A[1] = transformed[1];
        A[2] = transformed[2];
        position[0] = position[0] + A[0];
        position[1] = position[1] + A[1];
        position[2] = position[2] + A[2];

        float speed = F(e, 208) + centered_random() * F(e, 212);
        B[0] = B[0] * speed;
        B[1] = B[1] * speed;
        B[2] = B[2] * speed;
        float* velocity = reinterpret_cast<float*>(node + 44);
        velocity[0] = B[0];
        velocity[1] = B[1];
        velocity[2] = B[2];

        float inherit = F(e, 788);
        if (inherit != 0.0f) {
            D[0] = F(current, 0);
            D[1] = F(current, 4);
            D[2] = F(current, 8);
            D[0] = D[0] - F(last, 0);
            D[1] = D[1] - F(last, 4);
            D[2] = D[2] - F(last, 8);
            D[0] = D[0] * inverse_dt;
            D[1] = D[1] * inverse_dt;
            D[2] = D[2] * inverse_dt;
            D[0] = D[0] * inherit;
            D[1] = D[1] * inherit;
            D[2] = D[2] * inherit;
            velocity[0] = velocity[0] + D[0];
            velocity[1] = velocity[1] + D[1];
            velocity[2] = velocity[2] + D[2];
        }
    }
    _$_10VIMatrix44(matrix, 2);
    return 0;
}
}
