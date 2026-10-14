// Host check only; not a build input.
#include "pick.cpp"
static unsigned char *gR, *gG, *gScene; static int gA, gB, gN, which, calls, nextBuffer, nextCalls, ret;
extern "C" int GetNextColorBuffer__7VIScene(unsigned char* s) { gScene = s; ++nextCalls; return nextBuffer; }
extern "C" int PickPrimBuffer__8VIRasterR13VIPickSegmentii(unsigned char* r, unsigned char* g, int a, int b) { gR = r; gG = g; gA = a; gB = b; which = 2; ++calls; return ret; }
extern "C" int PickPrimBuffer__8VIRasterR13VIPickSegmentiii(unsigned char* r, unsigned char* g, int a, int n, int b) { gR = r; gG = g; gA = a; gN = n; gB = b; which = 3; ++calls; return ret; }
int main() {
    alignas(8) unsigned char self[96], seg[8], filt[8], scene[8], raster[8];
    for (int i = 0; i < 96; ++i) self[i] = static_cast<unsigned char>(i * 3 + 1);
    const int buffers[] = {-1, -300, 0, 5, 77};
    for (int k = 0; k < 5; ++k) for (int r = 0; r < 3; ++r) {
        *reinterpret_cast<int*>(self + 64) = 0x1000 + k * 77 - 5;
        *reinterpret_cast<int*>(self + 72) = -0x2000 - k * 31;
        calls = 0; nextCalls = 0; which = 0; nextBuffer = buffers[k]; ret = r * 9 - 4;
        if (Pick__14VISimpleSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(self, seg, filt, scene, raster) != ret) return 1;
        if (nextCalls != 1 || gScene != scene || calls != 1) return 2;
        if (gR != raster || gG != seg || gA != 0x1000 + k * 77 - 5 || gB != -0x2000 - k * 31) return 3;
        if (buffers[k] < 0 ? which != 2 : (which != 3 || gN != buffers[k])) return 4;
    }
    return 0;
}
