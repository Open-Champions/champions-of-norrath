// Observed: Render__19VIRadialFloraSystemR8VICamera at 0x010d4658 is 656 bytes. The system holds a
// raster-like pointer at word 1 (offset 4), the nine-word swatch table at offsets 32 to 64, a
// draw-entry count at 68 and 12-byte draw entries from offset 72. It keeps two zeroed 64-byte
// matrices on its stack and:
//   1. stores zero at 68 and sets the first matrix to identity (Identity__10VIMatrix44);
//   2. for each of the nine table slots i with a nonzero swatch pointer, calls
//      Cull__C9VIFrustumRC6VIBBoxRC10VIMatrix44(camera+304, swatch, identity) and, if that result
//      is zero, DrawSwatch__19VIRadialFloraSystemiR8VICamera(system, i, camera) (which appends
//      draw entries);
//   3. calls qsort(system+72, count, 12, QSortCompare__19VIRadialFloraSystemPCvT1);
//   4. walks the entries from the last to the first. For entry n it takes word 1 as a table
//      index and word 2 as an element index: swatch = table[word1]; the 96-byte elements start at
//      the pointer stored at swatch+48; Mul__10VIMatrix44RC10VIMatrix44T1(second, element, camera+48)
//      forms the second matrix from the element's leading matrix and the camera's matrix at 48,
//      then SetModelView__8VIRasterRC10VIMatrix44(word1 of the system, second) is called. The
//      entry's float at word 0 gives fade = 1.0 if it is not above 10.0f and otherwise
//      1.0f - (value - 10.0f) * the float with bits 0x3d088888; finally
//      DrawPrimBuffer__8VIRasteriifUi(word1 of the system, the element's words at 80 and 84, fade, 3);
//   5. destroys both matrices (flag 2) and returns zero.
// Inferred: word 1 of the system is the raster pointer (it receives SetModelView and
// DrawPrimBuffer); entries are sorted by their first float, ascending, and drawn farthest first
// when that float is a distance; 10.0 and about 30.0 (the reciprocal 0x3d088888) are the start
// and length of a distance fade.
// Unknown: the swatch and element layouts beyond the words used here, and the unit of the distance.
extern "C" {
struct VIRadialFloraSystem;
struct VICamera;
struct VIRaster;

struct VIMatrix44 {
    float m[16];
};

void Identity__10VIMatrix44(VIMatrix44* matrix);
int Cull__C9VIFrustumRC6VIBBoxRC10VIMatrix44(const void* frustum, const void* box, const VIMatrix44* matrix);
int DrawSwatch__19VIRadialFloraSystemiR8VICamera(VIRadialFloraSystem* system, int slot, VICamera& camera);
void qsort(void* base, unsigned count, unsigned size, int (*compare)(const void*, const void*));
int QSortCompare__19VIRadialFloraSystemPCvT1(const void* a, const void* b);
void Mul__10VIMatrix44RC10VIMatrix44T1(VIMatrix44* result, const void* a, const void* b);
int SetModelView__8VIRasterRC10VIMatrix44(VIRaster* raster, const VIMatrix44* matrix);
int DrawPrimBuffer__8VIRasteriifUi(VIRaster* raster, int a, int b, float fade, unsigned flags);
void _$_10VIMatrix44(VIMatrix44* matrix, int flags);

int Render__19VIRadialFloraSystemR8VICamera(VIRadialFloraSystem* system, VICamera& camera) {
    int* words = reinterpret_cast<int*>(system);
    char* cameraBytes = reinterpret_cast<char*>(&camera);
    VIMatrix44 view;
    VIMatrix44 model;
    for (int i = 0; i < 16; ++i) view.m[i] = 0.0f;
    for (int i = 0; i < 16; ++i) model.m[i] = 0.0f;
    words[17] = 0;
    Identity__10VIMatrix44(&view);
    for (int slot = 0; slot < 9; ++slot) {
        int swatch = words[8 + slot];
        if (swatch != 0 &&
            Cull__C9VIFrustumRC6VIBBoxRC10VIMatrix44(cameraBytes + 304, reinterpret_cast<const void*>(swatch), &view) == 0)
            DrawSwatch__19VIRadialFloraSystemiR8VICamera(system, slot, camera);
    }
    qsort(words + 18, static_cast<unsigned>(words[17]), 12, QSortCompare__19VIRadialFloraSystemPCvT1);
    for (int n = words[17] - 1; n >= 0; --n) {
        char* entry = reinterpret_cast<char*>(words + 18) + n * 12;
        char* swatch = reinterpret_cast<char*>(words[8 + *reinterpret_cast<int*>(entry + 4)]);
        char* detail = swatch + 44;
        Mul__10VIMatrix44RC10VIMatrix44T1(
            &model, *reinterpret_cast<char**>(detail + 4) + *reinterpret_cast<int*>(entry + 8) * 96, cameraBytes + 48);
        SetModelView__8VIRasterRC10VIMatrix44(reinterpret_cast<VIRaster*>(words[1]), &model);
        float distance = *reinterpret_cast<float*>(entry);
        float fade = 1.0f;
        if (10.0f < distance)
            fade = 1.0f - (distance - 10.0f) * __builtin_bit_cast(float, 0x3d088888u);
        char* element = *reinterpret_cast<char**>(detail + 4) + *reinterpret_cast<int*>(entry + 8) * 96;
        DrawPrimBuffer__8VIRasteriifUi(reinterpret_cast<VIRaster*>(words[1]), *reinterpret_cast<int*>(element + 80),
                                       *reinterpret_cast<int*>(element + 84), fade, 3);
    }
    _$_10VIMatrix44(&model, 2);
    _$_10VIMatrix44(&view, 2);
    return 0;
}
}
