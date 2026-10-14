// Host contract for the w10-world-c-texture-load-state storage. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "texture_load_state.cpp"

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

int main() {
    expect(loadCompleteFlag == 0 && loadPage == 0 && loadKillListSize == 0 && loadSegmentsLoaded == 0, "zero-initialized words");
    expect(needToClearCacheBuffer == 1, "needToClearCacheBuffer starts at 1");
    expect(reallyLoad_873 == 0, "reallyLoad starts at 0");
    expect(sizeof(loadCompleteFlag) == 1 && sizeof(loadPage) == 2 && sizeof(needToClearCacheBuffer) == 1 && sizeof(reallyLoad_873) == 1, "byte widths");
    bool zero = true;
    for (signed char c : loadKillList) zero &= c == 0;
    expect(zero && sizeof(loadKillList) == 16, "kill list: sixteen zero bytes");
    if (failures) return 1;
    std::printf("w10-world-c-texture-load-state contract passed\n");
    return 0;
}
