// Observed Init__7VIWorldiiiifRC7VIVect3RC6VIBBox at 0x010faad8, 540 bytes. Arguments in order:
// world (this), integers n1, n2, n3, n4, float f, a three-float vector `vec`, and a box `box`
// of six floats (box + 0 and box + 12 are its two corners). Result -1 on any failure, else 0.
//   1. Init__t7VIArray1Z6VIZonei(this + 72, n1); negative gives -1.
//   2. For i = 0 .. n1 - 1: Init__6VIZone(*(this + 76) + 592 * i); negative gives -1 (the array
//      pointer is re-read for each element).
//   3. If n1 == 1: with zone = *(this + 76), the box corners are copied to zone + 44 / zone + 56 and
//      to *(zone + 96) + 4 / *(zone + 96) + 16 (three floats each).
//   4. Init__11VIWorldTreeii(this + 52, 0, 1); negative gives -1.
//   5. Stores in this order: this + 32 = f; this + 36 / 40 / 44 = vec; the word at *(this + 64) = 0;
//      this + 24 = n2; this + 28 = n3; this + 48 = n4; this + 0 / 4 / 8 = box corner 0;
//      this + 12 / 16 / 20 = box corner 1.
//   6. Init__t7VIArray1Z13VIWorldRegioni(this + 80, (n2 * n4) * (n4 * n3)); negative gives -1.
//   7. r = SetProfiles__7VIWorld(this); the result is -1 if r < 0, otherwise 0.
// The product in step 6 is reproduced as the original multiplies it (two products multiplied),
// 32-bit wrapping. Unknown: the meaning of every argument and member.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
struct VIWorld;
struct VIVect3 { float x, y, z; };
struct VIBBox { float min[3]; float max[3]; };

int Init__t7VIArray1Z6VIZonei(void* array, int count);
int Init__6VIZone(void* zone);
int Init__11VIWorldTreeii(void* tree, int a, int b);
int Init__t7VIArray1Z13VIWorldRegioni(void* array, int count);
int SetProfiles__7VIWorld(VIWorld* world);

static unsigned word_at(const char* address) { return *reinterpret_cast<const unsigned*>(address); }

int Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(VIWorld* world, int n1, int n2, int n3, int n4, float f,
                                           const VIVect3* vec, const VIBBox* box) {
    char* w = reinterpret_cast<char*>(world);
    if (Init__t7VIArray1Z6VIZonei(w + 72, n1) < 0) return -1;
    for (int i = 0; i < n1; ++i)
        if (Init__6VIZone(NORRATH_TRANSLATE(word_at(w + 76)) + 592 * i) < 0) return -1;
    if (n1 == 1) {
        char* zone = NORRATH_TRANSLATE(word_at(w + 76));
        float* a = reinterpret_cast<float*>(zone + 44);
        float* b = reinterpret_cast<float*>(zone + 56);
        float* entry = reinterpret_cast<float*>(NORRATH_TRANSLATE(word_at(zone + 96)));
        a[0] = box->min[0];
        a[1] = box->min[1];
        a[2] = box->min[2];
        b[0] = box->max[0];
        b[1] = box->max[1];
        b[2] = box->max[2];
        entry[1] = box->min[0];
        entry[2] = box->min[1];
        entry[3] = box->min[2];
        entry[4] = box->max[0];
        entry[5] = box->max[1];
        entry[6] = box->max[2];
    }
    if (Init__11VIWorldTreeii(w + 52, 0, 1) < 0) return -1;
    *reinterpret_cast<float*>(w + 32) = f;
    float* out = reinterpret_cast<float*>(w + 36);
    out[0] = vec->x;
    out[1] = vec->y;
    out[2] = vec->z;
    *reinterpret_cast<unsigned*>(NORRATH_TRANSLATE(word_at(w + 64))) = 0;
    *reinterpret_cast<int*>(w + 24) = n2;
    *reinterpret_cast<int*>(w + 28) = n3;
    *reinterpret_cast<int*>(w + 48) = n4;
    float* corner = reinterpret_cast<float*>(w);
    corner[0] = box->min[0];
    corner[1] = box->min[1];
    corner[2] = box->min[2];
    corner[3] = box->max[0];
    corner[4] = box->max[1];
    corner[5] = box->max[2];
    int regions = static_cast<int>(static_cast<unsigned>(n2) * static_cast<unsigned>(n4) *
                                   (static_cast<unsigned>(n4) * static_cast<unsigned>(n3)));
    if (Init__t7VIArray1Z13VIWorldRegioni(w + 80, regions) < 0) return -1;
    return SetProfiles__7VIWorld(world) < 0 ? -1 : 0;
}
}
