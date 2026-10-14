// Host check only; not a build input.
#include "raster.cpp"
#include <cstdio>
int main() {
    unsigned char a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    bool ok = Raster__8VISpriteR8VICameraR7VISceneR8VIRasterUi(a, a + 1, a + 2, a + 3, 0xffffffffu) == 0
        && Raster__8VISpriteR8VICameraR7VISceneR8VIRasterUi(0, 0, 0, 0, 0) == 0 && a[0] == 1 && a[7] == 8;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
