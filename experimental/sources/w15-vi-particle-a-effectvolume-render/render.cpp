// Observed: Render__14VIEffectVolumeR8VICameraR10VIMatrix44 at 0x01147368 is 1528 bytes. Arguments: the object (V), a camera (C) and
// a matrix reference that is only passed on (M). Three 4x4 float matrices A, B and D are zero-filled on the stack and destroyed on return
// (_$_10VIMatrix44 with flag 2, in the order D, B, A).
//  1. If the word at V+72 is non-zero it calls AllocParticles__14VIEffectVolume(V) and clears the word.
//  2. sceVu0CopyMatrix(A, C+48), then Transpose__10VIMatrix44RC10VIMatrix44(B, A). B[12], B[13], B[14] become the negated dot products
//     of the elements (B[3], B[7], B[11]) with the columns (B[0], B[4], B[8]), (B[1], B[5], B[9]) and (B[2], B[6], B[10]) respectively, each
//     sum formed left to right; then B[3], B[7] and B[11] are set to 0.0f. sceVu0CopyMatrix(A, GetModelView__C8VIRaster(raster)) replaces A,
//     with raster the word at _14VIEffectVolume$mScene+19356, and Mul__10VIMatrix44RC10VIMatrix44T1(D, A, B) runs. The vector
//     (C+28 floats) minus D[12..14] is passed to Magnitude__C7VIVect3 and gives the distance L.
//  3. With r the float at V+48: L < r*0.75f stores 1.0f at V+96; L > r stores 0.0f at V+96 and skips to the destructors (nothing below
//     runs); otherwise V+96 gets 1.0f - (L - r*0.75f) / (r*0.25f) bounded to [0, 1] with max.s then min.s.
//  4. With a = float at V+52 and b = float at V+56, d = |a - b| taken through the original's soft-double routines (fptodp, dpcmp, dpsub,
//     dptofp). If d <= 0.25f the float at V+92 is 0.0f when a < b else 1.0f. If d >= 23.75f it is 1.0f when a < b else 0.0f. Otherwise
//     the float at scene+8160 is S, and: when b < a, S gains 24.0f if S < b and b gains 24.0f. If a <= S <= b the result is 1.0f. Else
//     with s = 1/12 (0x3daaaaaa): S loses 24.0f when b + s < S, otherwise gains 24.0f when S < a - s; then the result is
//     ((S - a) + s) * 12.0f if a - s < S <= a, else 0.0f if S < b, else ((b + s) - S) * 12.0f if S < b + s, else 0.0f. The result is bounded
//     to [0, 1] with max.s then min.s and stored at V+92.
//  5. The word at V+36 selects RenderFallingLeaves (1), RenderLightShafts (2), RenderGroundFog (3) or RenderDustStorm (4) of
//     __14VIEffectVolumeR8VICameraR10VIMatrix44 with (V, C, M); other values call nothing. It returns 0.
// Inferred: V+48 is a fade radius, V+96 a distance fade (1 inside three quarters of the radius, 0 beyond it); V+52/V+56 are a start
// and end time of day in hours on a 24-hour clock with a twelfth-hour (5-minute) ramp, V+92 the matching fade, and scene+8160 the current
// time of day. Matrix A first holds the camera matrix and then the model-view matrix.
// Unknown: the meaning of the matrix at C+48 beyond its use here, and the matrix byte layout of VIMatrix44 beyond 16 floats.
extern "C" {
extern unsigned char* _14VIEffectVolume$mScene;
void AllocParticles__14VIEffectVolume(void* self);
void sceVu0CopyMatrix(void* destination, const void* source);
void* Transpose__10VIMatrix44RC10VIMatrix44(void* destination, const void* source);
const void* GetModelView__C8VIRaster(void* raster);
void* Mul__10VIMatrix44RC10VIMatrix44T1(void* destination, const void* left, const void* right);
float Magnitude__C7VIVect3(const void* vector);
unsigned long long fptodp(float value);
int dpcmp(unsigned long long left, unsigned long long right);
unsigned long long dpsub(unsigned long long left, unsigned long long right);
float dptofp(unsigned long long value);
void _$_10VIMatrix44(void* matrix, int flags);
void RenderFallingLeaves__14VIEffectVolumeR8VICameraR10VIMatrix44(void* self, void* camera, void* matrix);
void RenderLightShafts__14VIEffectVolumeR8VICameraR10VIMatrix44(void* self, void* camera, void* matrix);
void RenderGroundFog__14VIEffectVolumeR8VICameraR10VIMatrix44(void* self, void* camera, void* matrix);
void RenderDustStorm__14VIEffectVolumeR8VICameraR10VIMatrix44(void* self, void* camera, void* matrix);

#if defined(__mips__)
static float lower_bound(float a, float b) {
    float result;
    __asm__ volatile ("max.s %0, %1, %2" : "=f" (result) : "f" (a), "f" (b));
    return result;
}
static float upper_bound(float a, float b) {
    float result;
    __asm__ volatile ("min.s %0, %1, %2" : "=f" (result) : "f" (a), "f" (b));
    return result;
}
#else
static float lower_bound(float a, float b) { return a > b ? a : b; }
static float upper_bound(float a, float b) { return a < b ? a : b; }
#endif

static void clear_matrix(float* m) {
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
}

int Render__14VIEffectVolumeR8VICameraR10VIMatrix44(unsigned char* self, unsigned char* camera, void* matrix) {
    alignas(16) float a[16];
    alignas(16) float b[16];
    alignas(16) float d[16];
    float offset[3];
    float* const fade_distance = reinterpret_cast<float*>(self + 96);
    float* const fade_time = reinterpret_cast<float*>(self + 92);
    if (*reinterpret_cast<unsigned*>(self + 72) != 0) {
        AllocParticles__14VIEffectVolume(self);
        *reinterpret_cast<unsigned*>(self + 72) = 0;
    }
    clear_matrix(a);
    clear_matrix(b);
    clear_matrix(d);
    sceVu0CopyMatrix(a, camera + 48);
    Transpose__10VIMatrix44RC10VIMatrix44(b, a);
    b[12] = -(b[3] * b[0] + b[7] * b[4] + b[11] * b[8]);
    b[13] = -(b[3] * b[1] + b[7] * b[5] + b[11] * b[9]);
    b[14] = -(b[3] * b[2] + b[7] * b[6] + b[11] * b[10]);
    b[11] = 0.0f;
    b[7] = 0.0f;
    b[3] = 0.0f;
    sceVu0CopyMatrix(a, GetModelView__C8VIRaster(*reinterpret_cast<void**>(_14VIEffectVolume$mScene + 19356)));
    Mul__10VIMatrix44RC10VIMatrix44T1(d, a, b);
    const float* position = reinterpret_cast<const float*>(camera + 28);
    offset[0] = position[0] - d[12];
    offset[1] = position[1] - d[13];
    offset[2] = position[2] - d[14];
    const float distance = Magnitude__C7VIVect3(offset);
    const float radius = *reinterpret_cast<float*>(self + 48);
    const float inner = radius * 0.75f;
    bool done = false;
    if (distance < inner) {
        *fade_distance = 1.0f;
    } else if (radius < distance) {
        *fade_distance = 0.0f;
        done = true;
    } else {
        *fade_distance = upper_bound(lower_bound(1.0f - (distance - inner) / (radius * 0.25f), 0.0f), 1.0f);
    }
    if (!done) {
        const float start = *reinterpret_cast<float*>(self + 52);
        float end = *reinterpret_cast<float*>(self + 56);
        unsigned long long span = fptodp(start - end);
        if (dpcmp(span, 0) < 0) span = dpsub(0, span);
        const float width = dptofp(span);
        const float ramp = 0x1.555554p-4f;
        if (width <= 0.25f) {
            *fade_time = start < end ? 0.0f : 1.0f;
        } else if (23.75f <= width) {
            *fade_time = start < end ? 1.0f : 0.0f;
        } else {
            float now = *reinterpret_cast<float*>(_14VIEffectVolume$mScene + 8160);
            if (end < start) {
                if (now < end) now += 24.0f;
                end += 24.0f;
            }
            if (start <= now && now <= end) {
                *fade_time = 1.0f;
            } else {
                if (end + ramp < now) now -= 24.0f;
                else if (now < start - ramp) now += 24.0f;
                if (start - ramp < now && now <= start) {
                    *fade_time = ((now - start) + ramp) * 12.0f;
                } else if (!(end <= now)) {
                    *fade_time = 0.0f;
                } else if (!(now < end + ramp)) {
                    *fade_time = 0.0f;
                } else {
                    *fade_time = ((end + ramp) - now) * 12.0f;
                }
                *fade_time = upper_bound(lower_bound(*fade_time, 0.0f), 1.0f);
            }
        }
        switch (*reinterpret_cast<unsigned*>(self + 36)) {
        case 1: RenderFallingLeaves__14VIEffectVolumeR8VICameraR10VIMatrix44(self, camera, matrix); break;
        case 2: RenderLightShafts__14VIEffectVolumeR8VICameraR10VIMatrix44(self, camera, matrix); break;
        case 3: RenderGroundFog__14VIEffectVolumeR8VICameraR10VIMatrix44(self, camera, matrix); break;
        case 4: RenderDustStorm__14VIEffectVolumeR8VICameraR10VIMatrix44(self, camera, matrix); break;
        default: break;
        }
    }
    _$_10VIMatrix44(d, 2);
    _$_10VIMatrix44(b, 2);
    _$_10VIMatrix44(a, 2);
    return 0;
}
}
