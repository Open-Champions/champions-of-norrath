// Host check only; not a build input.
#include "pick.cpp"
#include <cstring>
#include <vector>
#include <string>
static std::vector<std::string> events;
static int uiValue, uiCalls, pickResult, pickUi; static unsigned char *pickScene, *pickSegment, *pickFilter;
alignas(16) static float modelView[16];
alignas(16) static float viewSeen[16], dtorSeen[16]; static int dtorFlags, dtorCalls; static VIMatrixLocal* dtorPtr; static VIMatrixLocal* firstMatrix;
static unsigned char* clampSelf; static unsigned char* copyRasterArg;
extern "C" int GetStreamAudioUISprite__7VIScene(unsigned char*) { ++uiCalls; events.push_back("ui"); return uiValue; }
extern "C" unsigned char* GetModelView__C8VIRaster(unsigned char* r) { copyRasterArg = r; events.push_back("view"); return reinterpret_cast<unsigned char*>(modelView); }
extern "C" void sceVu0CopyMatrix(void* d, const void* s) { firstMatrix = static_cast<VIMatrixLocal*>(d); events.push_back("copy"); std::memcpy(d, s, 64); }
extern "C" void ClampScale__19VIStreamAudioSpriteR10VIMatrix44(unsigned char* s, VIMatrixLocal* m) {{ clampSelf = s; events.push_back("clamp"); if (m != firstMatrix) events.push_back("BADCLAMP"); for (int i = 0; i < 16; ++i) m->m[i] = m->m[i] * 0.5f + 1.0f; }}
static unsigned char* setRaster; static VIMatrixLocal* setMatrix;
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(unsigned char* r, VIMatrixLocal* m) { setRaster = r; setMatrix = m; events.push_back("set"); std::memcpy(viewSeen, m->m, 64); }
extern "C" int PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(unsigned char* s, unsigned char* g, unsigned char* f, int ui) { pickScene = s; pickSegment = g; pickFilter = f; pickUi = ui; events.push_back("pick"); return pickResult; }
extern "C" void _$_10VIMatrix44(VIMatrixLocal* m, int flags) { ++dtorCalls; dtorPtr = m; dtorFlags = flags; std::memcpy(dtorSeen, m->m, 64); events.push_back("dtor"); }
int main() {
    alignas(8) unsigned char self[64], seg[8], filt[8], scene[8], raster[8];
    for (int k = 0; k < 2; ++k) for (int r = 0; r < 3; ++r) {
        for (int i = 0; i < 16; ++i) modelView[i] = 1.0f + i * 0.75f - k;
        events.clear(); uiCalls = 0; dtorCalls = 0; uiValue = k ? 5 : -1; pickResult = r * 11 - 3; pickUi = -99;
        int got = Pick__19VIStreamAudioSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(self, seg, filt, scene, raster);
        if (k == 0) { if (got != 0 || events.size() != 1 || uiCalls != 1) return 1; continue; }
        if (got != pickResult || uiCalls != 2 || pickUi != 5) return 2;
        std::string order; for (auto& e : events) order += e + ",";
        if (order != "ui,view,copy,clamp,set,ui,pick,dtor,") return 3;
        if (clampSelf != self || copyRasterArg != raster || setRaster != raster || pickScene != scene || pickSegment != seg || pickFilter != filt) return 4;
        if (setMatrix != firstMatrix || dtorPtr != firstMatrix || dtorFlags != 2 || dtorCalls != 1) return 5;
        for (int i = 0; i < 16; ++i) {
            float expect = modelView[i]; expect = expect * 0.5f + 1.0f;
            if (viewSeen[i] != expect || dtorSeen[i] != expect) return 6;
        }
    }
    return 0;
}
