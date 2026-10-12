// Observed AdvanceDialogSwoop__6CameraiPf at 0x0026cde8, 392 bytes, pinned
// SLUS_205.65. Arguments: a camera object, a signed step count, and an
// optional float output. Returns 1 when the swoop is already finished.
// Camera fields used (byte offsets, observed): floats at 0, 4, 8 and 12, 16, 20
// (start and end positions), float 168 (progress), float 172 (a rate), signed
// halfwords 104, 106 (written) and 118, 120, 122, 124 (read).
// Observed: a positive count with progress exactly 1.0, or a negative count
// with progress exactly 0.0, returns 1 and changes nothing. Otherwise with
// step = 1.0/(float)count, progress becomes progress + (step - step*rate*0.75),
// is capped at 1.0 after being stored, and raised to 0.0 if negative. The ease
// s = evalHermite(progress, 0, 1, 0, 0) drives two lerpShortRot calls: the
// halfword at 106 gets lerpShortRot(h124, h122, s*s*s, -1, -1) and the one at 104
// gets lerpShortRot(h118, h120, s, -1, -1). The position is start + (end - start)*s
// per axis and is passed to BuildMatrix(camera, x, y, z, 0) with a null point pointer. The output, if non-null, receives s. Returns 0.
// A count of zero is not examined. Inferred: the meaning of the fields.
// Not executed on hardware.
extern "C" {
float evalHermite__Ffffff(float t, float a, float b, float c, float d);
short lerpShortRot__Fssfff(short from, short to, float amount, float a, float b);
void BuildMatrix__6CamerafffP6Point3(void* camera, float x, float y, float z, void* point);
}

static float floatAt(const unsigned char* base, int offset) { return *reinterpret_cast<const float*>(base + offset); }
static short halfAt(const unsigned char* base, int offset) { return *reinterpret_cast<const short*>(base + offset); }

extern "C" int AdvanceDialogSwoop__6CameraiPf(unsigned char* camera, int count, float* out) {
    float* progress = reinterpret_cast<float*>(camera + 168);
    if (count > 0) {
        if (*progress == 1.0f) return 1;
    }
    if (count < 0) {
        if (*progress == 0.0f) return 1;
    }
    float step = 1.0f / static_cast<float>(count);
    float damp = step * floatAt(camera, 172) * 0.75f;
    float next = *progress + (step - damp);
    *progress = next;
    if (1.0f < next) *progress = 1.0f;
    float now = *progress;
    if (now < 0.0f) {
        *progress = 0.0f;
        now = 0.0f;
    }
    float s = evalHermite__Ffffff(now, 0.0f, 1.0f, 0.0f, 0.0f);
    *reinterpret_cast<short*>(camera + 106) = lerpShortRot__Fssfff(halfAt(camera, 124), halfAt(camera, 122), s * s * s, -1.0f, -1.0f);
    short second = lerpShortRot__Fssfff(halfAt(camera, 118), halfAt(camera, 120), s, -1.0f, -1.0f);
    float x0 = floatAt(camera, 0), y0 = floatAt(camera, 4), z0 = floatAt(camera, 8);
    float x = x0 + (floatAt(camera, 12) - x0) * s;
    float y = y0 + (floatAt(camera, 16) - y0) * s;
    float z = z0 + (floatAt(camera, 20) - z0) * s;
    *reinterpret_cast<short*>(camera + 104) = second;
    BuildMatrix__6CamerafffP6Point3(camera, x, y, z, nullptr);
    if (out != nullptr) *out = s;
    return 0;
}
