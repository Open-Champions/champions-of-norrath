// Host contract for objectSaveGetFromStringPool__Fi. Not an EE plan input.
#include <climits>
#include <cstdio>
extern "C" char* poolOffset;
char* poolOffset;
extern "C" char* objectSaveGetFromStringPool__Fi(int);
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    static char pool[64];
    poolOffset = pool;
    expect(objectSaveGetFromStringPool__Fi(0) == pool, "index 0 is the base");
    expect(objectSaveGetFromStringPool__Fi(17) == pool + 17, "index is added to the base");
    expect(objectSaveGetFromStringPool__Fi(-1) == nullptr && objectSaveGetFromStringPool__Fi(INT_MIN) == nullptr, "negative gives null");
    poolOffset = nullptr;
    expect(objectSaveGetFromStringPool__Fi(0) == nullptr, "zero base and index 0 is null");
    expect(objectSaveGetFromStringPool__Fi(5) == reinterpret_cast<char*>(5), "no pool-end check");
    if (failures) return 1;
    std::printf("objectSaveGetFromStringPool contract passed\n");
    return 0;
}
