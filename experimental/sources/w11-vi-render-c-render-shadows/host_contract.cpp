#include "render_shadows.cpp"

#include <cstring>

alignas(4) static unsigned char object[7200];
static int calls;
static int seen[8];
static VIScene* seen_scene[8];
static int shrink_to = -1;

extern "C" int RenderShadow__7VIScenei(VIScene* scene, int entry) {
    if (calls < 8) {
        seen[calls] = entry;
        seen_scene[calls] = scene;
    }
    ++calls;
    if (shrink_to >= 0) *reinterpret_cast<int*>(object + 6792) = shrink_to;  // count is re-read
    return 77;
}

static void fill(int count) {
    std::memset(object, 0, sizeof object);
    for (int i = 0; i < 6; ++i) *reinterpret_cast<int*>(object + 6280 + 4 * i) = 100 + i;
    *reinterpret_cast<int*>(object + 6792) = count;
    calls = 0;
}

int main() {
    VIScene* scene = reinterpret_cast<VIScene*>(object);
    fill(0);
    if (RenderShadows__7VIScene(scene) != 0 || calls != 0) return 1;
    fill(-3);
    if (RenderShadows__7VIScene(scene) != 0 || calls != 0) return 2;
    fill(4);
    if (RenderShadows__7VIScene(scene) != 0 || calls != 4) return 3;
    for (int i = 0; i < 4; ++i)
        if (seen[i] != 100 + i || seen_scene[i] != scene) return 4;
    fill(5);
    shrink_to = 2;
    if (RenderShadows__7VIScene(scene) != 0 || calls != 2) return 5;  // count falls to 2 after the first call
    return 0;
}
