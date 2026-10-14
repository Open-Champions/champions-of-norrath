// Host check only; not a build input.
#include "pick.cpp"
#include <cstdio>
int main() {
    unsigned char a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    bool ok = Pick__8VISpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(a, a + 1, a + 2, a + 3, a + 4) == 0
        && Pick__8VISpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(0, 0, 0, 0, 0) == 0 && a[0] == 1 && a[7] == 8;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
