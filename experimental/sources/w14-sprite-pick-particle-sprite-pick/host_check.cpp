// Host check only; not a build input.
#include "pick.cpp"
#include <cstring>
#include <vector>
#include <string>
static std::vector<std::string> events;
static int uiValue, uiCalls, pickResult, pickUi; static unsigned char *pickScene, *pickSegment, *pickFilter;
alignas(16) static float modelView[16];
alignas(16) static float viewSeen[16], dtorSeen[16]; static int dtorFlags, dtorCalls; static VIMatrixLocal* dtorPtr; static VIMatrixLocal* firstMatrix;
static unsigned char *copyRasterArg, *setRaster; static VIMatrixLocal* setMatrix;
extern "C" int GetParticleUISprite__7VIScene(unsigned char*) { ++uiCalls; events.push_back("ui"); return uiValue; }
extern "C" unsigned char* GetModelView__C8VIRaster(unsigned char* r) { copyRasterArg = r; events.push_back("view"); return reinterpret_cast<unsigned char*>(modelView); }
extern "C" void sceVu0CopyMatrix(void* d, const void* s) { firstMatrix = static_cast<VIMatrixLocal*>(d); events.push_back("copy"); std::memcpy(d, s, 64); }
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(unsigned char* r, VIMatrixLocal* m) { setRaster = r; setMatrix = m; events.push_back("set"); std::memcpy(viewSeen, m->m, 64); }
extern "C" int PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(unsigned char* s, unsigned char* g, unsigned char* f, int ui) { pickScene = s; pickSegment = g; pickFilter = f; pickUi = ui; events.push_back("pick"); return pickResult; }
extern "C" void _$_10VIMatrix44(VIMatrixLocal* m, int flags) { ++dtorCalls; dtorPtr = m; dtorFlags = flags; std::memcpy(dtorSeen, m->m, 64); events.push_back("dtor"); }
int main() {
    alignas(8) unsigned char self[64], seg[8], filt[8], scene[8], raster[8];
    // row scale factors: rows shorter than, equal to and longer than the limit
    const float scales[][3] = {{1, 1, 1}, {3, 3, 3}, {0.5f, 5, 9}, {20, 0.1f, 4}, {2.3f, 2.3f, 2.3f}, {0, 0, 0}};
    for (int k = 0; k < 2; ++k) for (int s = 0; s < 6; ++s) for (int r = 0; r < 3; ++r) {
        for (int i = 0; i < 16; ++i) modelView[i] = 0.0f;
        for (int row = 0; row < 3; ++row) {
            modelView[4 * row + 0] = 0.6f * scales[s][row];
            modelView[4 * row + 1] = -0.64f * scales[s][row];
            modelView[4 * row + 2] = 0.48f * scales[s][row] + row;
            modelView[4 * row + 3] = 7.0f + row;
        }
        modelView[12] = 3.0f; modelView[13] = 4.0f; modelView[14] = 5.0f; modelView[15] = 1.0f;
        events.clear(); uiCalls = 0; dtorCalls = 0; uiValue = k ? 5 : -1; pickResult = r * 11 - 3; pickUi = -99;
        int got = Pick__16VIParticleSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(self, seg, filt, scene, raster);
        if (k == 0) { if (got != 0 || events.size() != 1 || uiCalls != 1) return 1; continue; }
        if (got != pickResult || uiCalls != 2 || pickUi != 5) return 2;
        std::string order; for (auto& e : events) order += e + ",";
        if (order != "ui,view,copy,set,ui,pick,dtor,") return 3;
        if (copyRasterArg != raster || setRaster != raster || pickScene != scene || pickSegment != seg || pickFilter != filt) return 4;
        if (setMatrix != firstMatrix || dtorPtr != firstMatrix || dtorFlags != 2 || dtorCalls != 1) return 5;
        for (int i = 0; i < 16; ++i) {
            float expect = modelView[i];
            int row = i / 4, col = i % 4;
            if (row < 3 && col < 3) {
                const float* q = modelView + 4 * row;
                float inv = 1.0f / __builtin_sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2]);
                if (inv < 0.25f) expect = expect * inv * 4.0f;
            }
            if (viewSeen[i] != expect || dtorSeen[i] != expect) return 6;
        }
    }
    return 0;
}
