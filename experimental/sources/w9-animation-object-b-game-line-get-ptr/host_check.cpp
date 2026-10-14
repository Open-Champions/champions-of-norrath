// Host contract for gameLineGetPtr__Fi. Not an EE plan input.
#include <cstdio>
extern "C" {
void* gameLines[8];
void* gameLineGetPtr__Fi(int);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    int slots[8];
    for (int i = 0; i < 8; ++i) gameLines[i] = &slots[i];
    gameLines[5] = nullptr;
    expect(gameLineGetPtr__Fi(0) == &slots[0], "index 0");
    expect(gameLineGetPtr__Fi(7) == &slots[7], "last index");
    expect(gameLineGetPtr__Fi(5) == nullptr, "a null entry is returned as is");
    expect(gameLineGetPtr__Fi(3) == &slots[3], "middle index");
    if (failures) return 1;
    std::printf("gameLineGetPtr contract passed\n");
    return 0;
}
