// Host check only; not a build input.
#include "release.cpp"
static unsigned char *gD, *gR; static int calls, ret;
extern "C" int Remove__12VIDictionaryR10VIResource(unsigned char* d, unsigned char* r) { gD = d; gR = r; ++calls; return ret; }
int main() {
    alignas(8) unsigned char self[96], dict[8], other[8];
    for (int i = 0; i < 96; ++i) self[i] = static_cast<unsigned char>(i * 5 + 3);
    const int rets[] = {0, 1, -1, -9, 77};
    for (int z = 0; z < 2; ++z)
        for (int k = 0; k < 5; ++k) {
            *reinterpret_cast<int*>(self) = z ? 0 : 0x1234 + k;
            calls = 0; ret = rets[k];
            int want = (!z && rets[k] < 0) ? -1 : 0;
            if (Release__12VIPointLightR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(self, other, dict, other, other, other) != want) return 1;
            if (z ? calls != 0 : (calls != 1 || gD != dict || gR != self)) return 2;
        }
    return 0;
}
