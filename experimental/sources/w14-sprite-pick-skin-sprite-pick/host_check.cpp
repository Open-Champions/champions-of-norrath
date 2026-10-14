// Host check only; not a build input.
#include "pick.cpp"
static unsigned char *gR, *gG; static int gA, gB, calls, ret;
extern "C" int PickPrimBuffer__8VIRasterR13VIPickSegmentii(unsigned char* r, unsigned char* g, int a, int b) { gR = r; gG = g; gA = a; gB = b; ++calls; return ret; }
int main() {
    alignas(8) unsigned char self[96], seg[8], filt[8], scene[8], raster[8];
    for (int i = 0; i < 96; ++i) self[i] = static_cast<unsigned char>(i * 3 + 1);
    const int rets[] = {0, 1, -7, 1234};
    for (int k = 0; k < 4; ++k) {
        *reinterpret_cast<int*>(self + 68) = 0x1000 + k * 77 - 5;
        *reinterpret_cast<int*>(self + 72) = -0x2000 - k * 31;
        calls = 0; ret = rets[k];
        if (Pick__12VISkinSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(self, seg, filt, scene, raster) != rets[k]) return 1;
        if (calls != 1 || gR != raster || gG != seg || gA != 0x1000 + k * 77 - 5 || gB != -0x2000 - k * 31) return 2;
    }
    return 0;
}
