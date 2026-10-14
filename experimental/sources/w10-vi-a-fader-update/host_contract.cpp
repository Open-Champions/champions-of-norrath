#include "update.cpp"

#include <cstring>
#include <sys/mman.h>

struct Event { int kind; int a; int b; float f[4]; };
static Event events[32];
static int event_count;
static long long now_value;
static void* raster_seen;

static Event& add(int kind) { Event& e = events[event_count++]; e = Event(); e.kind = kind; return e; }

extern "C" long long GetTime__7VITimer() { add(1); return now_value; }
extern "C" void SetMaterial__8VIRasteri(void* r, int index) { raster_seen = r; add(2).a = index; }
extern "C" void Begin2D__8VIRaster(void* r) { if (r != raster_seen) add(99); add(3); }
extern "C" void BeginTriStrip__8VIRaster(void* r) { if (r != raster_seen) add(99); add(4); }
extern "C" void Color__8VIRasterRC10VIColor32F(void* r, const float* c) {
    if (r != raster_seen) add(99);
    Event& e = add(5);
    std::memcpy(e.f, c, sizeof e.f);
}
extern "C" void Vertex__8VIRasterRC8VIVect2I(void* r, const int* p) {
    if (r != raster_seen) add(99);
    Event& e = add(6);
    e.a = p[0];
    e.b = p[1];
}
extern "C" void EndTriStrip__8VIRaster(void* r) { if (r != raster_seen) add(99); add(7); }
extern "C" void End2D__8VIRaster(void* r) { if (r != raster_seen) add(99); add(8); }

// The raster pointer word is 32 bits wide, so the fake raster is mapped below
// 4 GiB. Build with: c++ -std=c++17 -Wl,-pagezero_size,0x10000 ...
int main() {
    unsigned char* raster = static_cast<unsigned char*>(mmap(
        reinterpret_cast<void*>(0x20000000), 0x10000, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0));
    if (raster != reinterpret_cast<unsigned char*>(0x20000000)) return 100;
    std::memset(raster, 0, 0x4600);
    int width = 640, height = 448;
    std::memcpy(raster + 0x4554, &width, 4);
    std::memcpy(raster + 0x4558, &height, 4);

    unsigned char object[32];
    std::memset(object, 0, sizeof object);
    unsigned raster_word = static_cast<unsigned>(reinterpret_cast<unsigned long>(raster));
    auto setu = [&](int off, unsigned v) { std::memcpy(object + off, &v, 4); };
    auto setf = [&](int off, float v) { std::memcpy(object + off, &v, 4); };
    auto getf = [&](int off) { float v; std::memcpy(&v, object + off, 4); return v; };
    auto getll = [&](int off) { long long v; std::memcpy(&v, object + off, 8); return v; };
    auto setll = [&](int off, long long v) { std::memcpy(object + off, &v, 8); };

    // Fade up: rate 2/s, 250 ms elapsed from 0.25 to 0.75 (clamp target 1.0).
    setu(0, raster_word);
    setf(8, 0.25f); setf(12, 1.0f); setf(16, 2.0f);
    setu(20, 9);
    setll(24, 1000);
    now_value = 1250;
    Update__7VIFader(object);
    float level = getf(8);
    if (level != 0.25f + 2.0f * (static_cast<float>(250) * 0.001f)) return 1;
    if (getll(24) != 1250) return 2;
    static const int kinds[] = {1, 2, 3, 4, 5, 6, 6, 6, 6, 7, 8};
    if (event_count != 11) return 3;
    for (int i = 0; i < 11; ++i) if (events[i].kind != kinds[i]) return 4;
    if (events[1].a != 9) return 5;
    if (events[4].f[0] != 0 || events[4].f[1] != 0 || events[4].f[2] != 0 || events[4].f[3] != level) return 6;
    if (events[5].a != 0 || events[5].b != 448) return 7;
    if (events[6].a != 0 || events[6].b != -1) return 8;
    if (events[7].a != 641 || events[7].b != 448) return 9;
    if (events[8].a != 641 || events[8].b != -1) return 10;

    // Clamp above: 10 seconds elapsed.
    event_count = 0;
    now_value = 11250;
    Update__7VIFader(object);
    if (getf(8) != 1.0f) return 11;

    // Fade down to the target of zero: no drawing once the level is not above 0.
    event_count = 0;
    setf(8, 0.5f); setf(12, 0.0f); setf(16, -1.0f);
    setll(24, 0);
    now_value = 2000;
    Update__7VIFader(object);
    if (getf(8) != 0.0f) return 12;
    if (event_count != 1 || events[0].kind != 1) return 13;

    // Zero rate counts as the non-positive branch: level is raised to the target.
    event_count = 0;
    setf(8, 0.0f); setf(12, 0.5f); setf(16, 0.0f);
    setll(24, 2000);
    now_value = 2000;
    Update__7VIFader(object);
    if (getf(8) != 0.5f) return 14;
    if (event_count != 11) return 15;
    return 0;
}
