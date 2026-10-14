// Host check only; not a build input. Both callees are stubbed with scripted results.
#include "render_by_volume.cpp"
#include <cstdio>
static int pre, render, preCalls, renderCalls, seenFlags;
static unsigned seenRaster, seenCamera;
static void* seenRecord;
extern "C" int SetPretranslations__6VIZoneR8VIRasterR8VICamera(void*, unsigned raster, unsigned camera) { ++preCalls; seenRaster = raster; seenCamera = camera; return pre; }
extern "C" int RenderByVolume__6VIZoneiR15VISceneRendVars(void*, int flags, void* record) { ++renderCalls; seenFlags = flags; seenRecord = record; return render; }
int main() {
    unsigned char zone[128] = {}, record[64] = {};
    *reinterpret_cast<unsigned*>(record + 12) = 0x1111; *reinterpret_cast<unsigned*>(record + 16) = 0x2222;
    *reinterpret_cast<int*>(zone + 68) = 5;
    pre = 0; render = 3;
    bool ok = RenderByVolume__6VIZoneR15VISceneRendVarsi(zone, record, 9) == 0 && *reinterpret_cast<int*>(record + 48) == 9 && seenRaster == 0x2222 && seenCamera == 0x1111 &&
              seenFlags == 0 && seenRecord == record && preCalls == 1 && renderCalls == 1;
    *reinterpret_cast<int*>(zone + 68) = 0;
    ok = ok && RenderByVolume__6VIZoneR15VISceneRendVarsi(zone, record, 4) == 0 && seenFlags == 0x40000000 && *reinterpret_cast<int*>(record + 48) == 4;
    render = -1;
    ok = ok && RenderByVolume__6VIZoneR15VISceneRendVarsi(zone, record, 4) == -1;
    pre = -2; renderCalls = 0;
    ok = ok && RenderByVolume__6VIZoneR15VISceneRendVarsi(zone, record, 7) == -1 && renderCalls == 0 && *reinterpret_cast<int*>(record + 48) == 7;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
