// Observed: SetPretranslations__6VIZoneR8VIRasterR8VICamera at 0x01115828 is 204 bytes.
// Arguments: the zone (this), a raster and a camera. The zone holds a signed count at byte offset 84
// and a pointer at +88 to that many 12-byte vectors (three floats). The camera's float triple at
// +28 is the eye position. For each index i from 0 to count-1 the routine stores
// (v[i].x - eye.x, v[i].y - eye.y, v[i].z - eye.z), each float subtracted separately, into
// the 12-byte slot i of the array at zone+208 (no bounds check against any capacity). The original
// also keeps the latest result in a stack temporary that nothing reads again. It then calls
// SetPreTranslations__8VIRasteriP7VIVect3RC7VIVect3(raster, count, zone+208, camera+28), with count
// re-read from the zone, and returns -1 if that call returns a negative value and 0 otherwise.
// With a non-positive count the loop is skipped and the call is still made.
// Inferred: per-zone point offsets relative to the camera, handed to the rasterizer as
// pretranslations. Unknown: member names and the capacity of the array at +208.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int SetPreTranslations__8VIRasteriP7VIVect3RC7VIVect3(unsigned char* raster, int count, unsigned char* out, const unsigned char* eye);

int SetPretranslations__6VIZoneR8VIRasterR8VICamera(unsigned char* self, unsigned char* raster, unsigned char* camera) {
    const unsigned char* eye = camera + 28;
    float* out = reinterpret_cast<float*>(self + 208);
    for (int i = 0; i < *reinterpret_cast<const int*>(self + 84); ++i) {
        const float* v = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(self + 88)) + 12 * i);
        const float* e = reinterpret_cast<const float*>(eye);
        const float x = v[0] - e[0];
        const float y = v[1] - e[1];
        const float z = v[2] - e[2];
        out[3 * i + 0] = x;
        out[3 * i + 1] = y;
        out[3 * i + 2] = z;
    }
    return SetPreTranslations__8VIRasteriP7VIVect3RC7VIVect3(raster, *reinterpret_cast<const int*>(self + 84), reinterpret_cast<unsigned char*>(self + 208), eye) < 0 ? -1 : 0;
}
}
