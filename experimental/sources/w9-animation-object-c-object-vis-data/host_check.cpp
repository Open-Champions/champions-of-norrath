// Host contract for the objectVis storage unit. Not an EE plan input.
#include <cstdio>
#include "object_vis_data.cpp"
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    expect(sizeof(objectVisPoints) == 48 && sizeof(ObjectVisPoint) == 12, "48 bytes of four 12-byte points");
    for (auto& p : objectVisPoints) expect(p.x == 0 && p.y == 0 && p.z == 0, "points start at zero");
    expect(objectVisCount == 0 && sizeof(objectVisCount) == 1, "count is a zero signed byte");
    expect(objectVisUseCamCenter == 1 && sizeof(objectVisUseCamCenter) == 1, "flag byte starts at 1");
    if (failures) return 1;
    std::printf("object vis storage check passed\n");
    return 0;
}
