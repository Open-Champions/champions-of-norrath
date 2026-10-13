// Observed: CalcOutdoorsIntensity__7VISceneR8VICamera at 0x01143940 is 408 bytes. It builds a
// pick segment from the camera position (the three floats at camera offset 28) straight down
// 15.0 units: both ends start at the position and the end's second component has 15.0f
// subtracted. The pick runs with a local scene filter whose word at offset 16 is the address of
// the vtable symbol _vt$21VISceneOutdoorsFilter:
//   __13VIPickSegment(segment); __13VISceneFilter(filter);
//   Init__13VIPickSegmentRC7VIVect3T1(segment, start, end);
//   Pick__7VISceneR13VIPickSegmentR13VISceneFilter(scene, segment, filter).
// With a zero result the function returns zero and nothing in the scene changes. Otherwise it
// reads the 64-bit word at scene offset 6840 (a previous time) and the one at 1152 (the current
// time):
//   * previous nonzero: limit = (float)(int)(current - previous) * a * b, with a the float of
//     bits 0x3a83126e and b the float of bits 0x3e4ccccc; the float at offset 12 of the pointer
//     returned by GetHitColor__13VIPickSegment(segment) minus the scene float at 6848 is clamped
//     to [-limit, limit] (max first, then min) and added to the float at 6848;
//   * previous zero: the float at 6848 is set to the float at offset 12 of the GetHitColor result.
// In both cases the 64-bit word at 6840 then takes the current time word at 1152 (re-read after
// the GetHitColor call). The result is zero.
// Inferred: the float at 6848 is the current outdoors intensity (the hit color's fourth
// component), eased toward the ground sample at a rate proportional to elapsed time; 1152 is a
// time and a is close to 1/1000.
// Unknown: whether the fourth color component is alpha or something else for this pick, the
// time unit, and the sizes of the segment and filter objects (96 and 32 bytes are the frame slots
// seen).
extern "C" {
struct VIScene;
struct VICamera;

extern unsigned char norrath_outdoors_filter_vtable[] __asm__("_vt$21VISceneOutdoorsFilter");
void __13VIPickSegment(void* segment);
void __13VISceneFilter(void* filter);
void Init__13VIPickSegmentRC7VIVect3T1(void* segment, const float* start, const float* end);
int Pick__7VISceneR13VIPickSegmentR13VISceneFilter(VIScene* scene, void* segment, void* filter);
const float* GetHitColor__13VIPickSegment(void* segment);

int CalcOutdoorsIntensity__7VISceneR8VICamera(VIScene& scene, VICamera& camera) {
    char* world = reinterpret_cast<char*>(&scene);
    const float* position = reinterpret_cast<const float*>(reinterpret_cast<char*>(&camera) + 28);
    float start[3] = {0.0f, 0.0f, 0.0f};
    float end[3] = {0.0f, 0.0f, 0.0f};
    alignas(8) char segment[96];
    alignas(8) char filter[32];
    __13VIPickSegment(segment);
    __13VISceneFilter(filter);
    float x = position[0];
    float y = position[1];
    float z = position[2];
    start[2] = z;
    start[1] = y;
    start[0] = x;
    end[0] = x;
    *reinterpret_cast<void**>(filter + 16) = norrath_outdoors_filter_vtable;
    end[1] = start[1];
    end[2] = start[2];
    end[1] = end[1] - 15.0f;
    Init__13VIPickSegmentRC7VIVect3T1(segment, start, end);
    if (Pick__7VISceneR13VIPickSegmentR13VISceneFilter(&scene, segment, filter) == 0) return 0;

    long long* previous = reinterpret_cast<long long*>(world + 6840);
    long long* current = reinterpret_cast<long long*>(world + 1152);
    float* intensity = reinterpret_cast<float*>(world + 6848);
    if (*previous != 0) {
        int elapsed = static_cast<int>(*current - *previous);
        float limit = static_cast<float>(elapsed) * __builtin_bit_cast(float, 0x3a83126eu);
        limit = limit * __builtin_bit_cast(float, 0x3e4ccccc);
        float delta = GetHitColor__13VIPickSegment(segment)[3] - *intensity;
        float negative = -limit;
        delta = delta > negative ? delta : negative;
        delta = delta < limit ? delta : limit;
        *previous = *current;
        *intensity = *intensity + delta;
    } else {
        float hit = GetHitColor__13VIPickSegment(segment)[3];
        *intensity = hit;
        *previous = *current;
    }
    return 0;
}
}
