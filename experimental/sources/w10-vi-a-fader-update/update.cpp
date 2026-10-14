// Observed: Update__7VIFader at 0x0117caa8 is 372 bytes and returns nothing.
// 1. now = GetTime__7VITimer(); elapsed = (float)(now - the 64-bit word at
//    offset 24) * 0.001f (constant 0x3a83126e); offset 24 = now.
// 2. level (float, offset 8) += rate (float, offset 16) * elapsed, stored back.
// 3. Clamp: if rate > 0 the level becomes min(target, level), otherwise
//    max(level, target), where target is the float at offset 12. Stored.
// 4. If the level is above zero it draws a fullscreen translucent black quad
//    on the VIRaster whose pointer is the word at offset 0: SetMaterial(raster,
//    material index at offset 20), Begin2D, BeginTriStrip, Color with a
//    VIColor32F {0, 0, 0, level}, four Vertex calls with VIVect2I points
//    (0, H), (0, -1), (W + 1, H), (W + 1, -1), then EndTriStrip and End2D.
//    W and H are the words at raster offsets 0x4554 and 0x4558.
// Not byte-identical: min/max are written as compares and the draw structures
// are plain arrays.
// Unknown: what W and H are named in VIRaster; whether the -1 row is meant as
// a bottom edge, and the units of the rate.
extern "C" {
long long GetTime__7VITimer();
void SetMaterial__8VIRasteri(void* raster, int index);
void Begin2D__8VIRaster(void* raster);
void BeginTriStrip__8VIRaster(void* raster);
void Color__8VIRasterRC10VIColor32F(void* raster, const float* color);
void Vertex__8VIRasterRC8VIVect2I(void* raster, const int* point);
void EndTriStrip__8VIRaster(void* raster);
void End2D__8VIRaster(void* raster);

void Update__7VIFader(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    long long now = GetTime__7VITimer();
    long long* last = reinterpret_cast<long long*>(bytes + 24);
    float elapsed = static_cast<float>(now - *last) * 0.001f;
    *last = now;

    float* level = reinterpret_cast<float*>(bytes + 8);
    float target = *reinterpret_cast<float*>(bytes + 12);
    float rate = *reinterpret_cast<float*>(bytes + 16);
    float value = *level + rate * elapsed;
    *level = value;
    if (rate > 0.0f) {
        if (target < value) value = target;
    } else {
        if (value < target) value = target;
    }
    *level = value;

    if (!(0.0f < *level)) return;
    void* raster = reinterpret_cast<void*>(static_cast<unsigned long>(*reinterpret_cast<unsigned*>(bytes)));
    unsigned char* raster_bytes = static_cast<unsigned char*>(raster);
    SetMaterial__8VIRasteri(raster, *reinterpret_cast<int*>(bytes + 20));
    Begin2D__8VIRaster(raster);
    BeginTriStrip__8VIRaster(raster);
    float color[4] = {0.0f, 0.0f, 0.0f, *level};
    Color__8VIRasterRC10VIColor32F(raster, color);
    int height = *reinterpret_cast<int*>(raster_bytes + 0x4558);
    int top[2] = {0, height};
    Vertex__8VIRasterRC8VIVect2I(raster, top);
    int bottom[2] = {0, -1};
    Vertex__8VIRasterRC8VIVect2I(raster, bottom);
    int width = *reinterpret_cast<int*>(raster_bytes + 0x4554);
    int third[2] = {width + 1, *reinterpret_cast<int*>(raster_bytes + 0x4558)};
    Vertex__8VIRasterRC8VIVect2I(raster, third);
    int fourth[2] = {*reinterpret_cast<int*>(raster_bytes + 0x4554) + 1, -1};
    Vertex__8VIRasterRC8VIVect2I(raster, fourth);
    EndTriStrip__8VIRaster(raster);
    End2D__8VIRaster(raster);
}
}
