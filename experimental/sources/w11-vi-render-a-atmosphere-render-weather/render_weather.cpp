// Observed: RenderWeather__12VIAtmosphereR7VISceneR8VICamera at 0x010681c8 is
// 1464 bytes and returns nothing. Nothing is drawn (and RenderSplashes is not
// called) when the word at 1228 of this is zero or the float at 72 equals 0.0f.
// Otherwise, with raster = the scene word at 19356:
//  1. SetModelView__8VIRasterRC10VIMatrix44(raster, camera+48).
//  2. n = (int)(float72 * (float)int8) clamped to at most the int at 8 and at
//     least 0; split = (int)((float)n * float128).
//  3. Plane__C9VIFrustumi(camera+304, 0) returns four floats P (a, b, c, d). A
//     local u = (P.a, 0, P.c) is normalized with Normalize__7VIVect3 and
//     k = 1.0f - ((P.a*u.x + P.b*u.y) + P.c*u.z).
//  4. If 0 < split < n the particles run in two groups: [0, split) with the
//     material word at 164 and then [split, n) with the word at 160; otherwise
//     all n particles run in one group with the material word at 160 when split
//     is zero and 164 when it is not. Each group calls SetMaterial__8VIRasteri
//     first (also when n is 0). For a group the base size s is float16 times
//     the float at 136 (the word-160 group, or the second of two groups) or 140
//     (the word-164 group, or the first of two groups); a = s - s*X*k and
//     b = s - s*Y*k, where for the one-group case (X, Y) are the floats at
//     (152, 144) for the word-160 group and (156, 148) for the word-164 group,
//     and for the two-group case the floats at (152, 144) for the first group
//     and (156, 148) for the second.
//  5. A group is drawn with billboards: records are 36 bytes at the pointer word
//     at 12. Billboard batches come from BeginBillboards__8VIRasterPP11VIBillboardi
//     (raster, &cursor, remaining), 48-byte entries, and EndBillboards__8VIRaster
//     closes a batch when its capacity is used up or the group ends. An entry gets
//     the record position plus the floats at 60, 64, 68, the word 0 at 12, a at
//     16 and b at 20, and the 16 bytes at record+12 at entry+32. The distance
//     ((x*P.a + y*P.b) + z*P.c) + P.d of the offset position is compared with
//     -1.5f: when -1.5f < distance, the words at 16 and 20 are set to zero.
//  6. RenderSplashes__12VIAtmosphereR7VISceneR8VICamera(this, scene, camera).
// Inferred: the records are rain/snow particles, P is a frustum plane and the
// two groups are two weather types split at a ratio; the quirk that the two-group
// case pairs the sizes (152,144) with the first group is as observed. Unknown:
// the member names and the plane's identity.
extern "C" {
void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
void* Plane__C9VIFrustumi(const void* frustum, int index);
int Normalize__7VIVect3(void* vector);
void SetMaterial__8VIRasteri(void* raster, int material);
int BeginBillboards__8VIRasterPP11VIBillboardi(void* raster, unsigned char** cursor, int count);
void EndBillboards__8VIRaster(void* raster);
void RenderSplashes__12VIAtmosphereR7VISceneR8VICamera(void* object, void* scene, void* camera);

static float float_at(const unsigned char* base, int offset) {
    return *reinterpret_cast<const float*>(base + offset);
}
static unsigned int word_at(const unsigned char* base, int offset) {
    return *reinterpret_cast<const unsigned int*>(base + offset);
}

static void draw_group(unsigned char* object, void* raster, const float* plane, unsigned char*& cursor,
                       int first, int count, int remaining, float width, float height) {
    int offset = first * 36;
    int capacity = 0;
    if (count <= 0) return;
    for (int left = count; left != 0; --left) {
        if (cursor == 0) capacity = BeginBillboards__8VIRasterPP11VIBillboardi(raster, &cursor, remaining);
        const unsigned char* particle = reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(word_at(object, 12))) + offset;
        unsigned char* entry = cursor;
        float x = float_at(particle, 0) + float_at(object, 60);
        float y = float_at(particle, 4) + float_at(object, 64);
        float z = float_at(particle, 8) + float_at(object, 68);
        *reinterpret_cast<float*>(entry + 0) = x;
        *reinterpret_cast<float*>(entry + 4) = y;
        *reinterpret_cast<float*>(entry + 8) = z;
        *reinterpret_cast<float*>(entry + 16) = width;
        *reinterpret_cast<float*>(entry + 20) = height;
        float distance = x * plane[0];
        distance = distance + y * plane[1];
        distance = distance + z * plane[2];
        distance = distance + plane[3];
        offset += 36;
        if (__builtin_bit_cast(float, 0xbfc00000u) < distance) {
            *reinterpret_cast<int*>(entry + 16) = 0;
            *reinterpret_cast<int*>(entry + 20) = 0;
        }
        *reinterpret_cast<int*>(entry + 12) = 0;
        --capacity;
        --remaining;
        for (int i = 0; i < 16; ++i) entry[32 + i] = particle[12 + i];
        cursor = entry + 48;
        if (capacity == 0) {
            EndBillboards__8VIRaster(raster);
            cursor = 0;
        }
    }
}

void RenderWeather__12VIAtmosphereR7VISceneR8VICamera(void* object_, void* scene_, void* camera_) {
    unsigned char* object = static_cast<unsigned char*>(object_);
    unsigned char* scene = static_cast<unsigned char*>(scene_);
    unsigned char* camera = static_cast<unsigned char*>(camera_);
    if (word_at(object, 1228) == 0) return;
    if (float_at(object, 72) == 0.0f) return;
    void* raster = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word_at(scene, 19356)));
    SetModelView__8VIRasterRC10VIMatrix44(raster, camera + 48);
    int capacity = static_cast<int>(word_at(object, 8));
    int n = static_cast<int>(float_at(object, 72) * static_cast<float>(capacity));
    if (!(n < capacity)) n = capacity;
    if (n < 0) n = 0;
    int split = static_cast<int>(static_cast<float>(n) * float_at(object, 128));
    const float* source = static_cast<const float*>(Plane__C9VIFrustumi(camera + 304, 0));
    float plane[4] = {source[0], source[1], source[2], source[3]};
    float down[3] = {plane[0], 0.0f, plane[2]};
    Normalize__7VIVect3(down);
    float dot = plane[0] * down[0];
    dot = dot + plane[1] * down[1];
    dot = dot + plane[2] * down[2];
    float k = 1.0f - dot;
    unsigned char* cursor = 0;
    if (split > 0 && split < n) {
        for (int phase = 0; phase < 2; ++phase) {
            int slot = (1 - phase) * 4;
            SetMaterial__8VIRasteri(raster, static_cast<int>(word_at(object, 160 + slot)));
            float base = float_at(object, 136 + slot) * float_at(object, 16);
            float x = float_at(object, 152 + 4 * phase);
            float y = float_at(object, 144 + 4 * phase);
            float height = base - base * x * k;
            float width = base - base * y * k;
            int first = phase == 0 ? 0 : split;
            int count = phase == 0 ? split : n - split;
            draw_group(object, raster, plane, cursor, first, count, count, width, height);
            if (cursor != 0) {
                EndBillboards__8VIRaster(raster);
                cursor = 0;
            }
        }
    } else {
        int upper = split != 0;
        SetMaterial__8VIRasteri(raster, static_cast<int>(word_at(object, upper ? 164 : 160)));
        float base = float_at(object, upper ? 140 : 136) * float_at(object, 16);
        float x = float_at(object, upper ? 156 : 152);
        float y = float_at(object, upper ? 148 : 144);
        float height = base - base * x * k;
        float width = base - base * y * k;
        draw_group(object, raster, plane, cursor, 0, n, n, width, height);
        if (cursor != 0) {
            EndBillboards__8VIRaster(raster);
            cursor = 0;
        }
    }
    RenderSplashes__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
}
}
