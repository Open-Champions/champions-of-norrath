// Host contract for modelGetNmShadowPoints__FP12_modelHeader. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" const void* modelGetNmShadowPoints__FP12_modelHeader(const unsigned char*);
int main() {
    int failures = 0;
    alignas(16) unsigned char buffer[16 * 260] = {};
    const int counts[] = {0, 1, 3, 17, 128, 255};
    for (int count : counts) {
        std::memset(buffer, 0, sizeof buffer);
        buffer[36] = static_cast<unsigned char>(count);
        static int target[4];
        const void* wantPtr = target + count % 4;
        std::memcpy(buffer + count * 16 + 4, &wantPtr, sizeof wantPtr);
        const void* decoy = buffer;
        // a decoy at the header's own +4 must be ignored unless count is 0 (then it is the same slot)
        if (count != 0) std::memcpy(buffer + 4, &decoy, sizeof decoy);
        const void* got = modelGetNmShadowPoints__FP12_modelHeader(buffer);
        if (got != wantPtr) { std::fprintf(stderr, "fail: count %d\n", count); ++failures; }
    }
    if (failures) return 1;
    std::printf("modelGetNmShadowPoints contract passed\n");
    return 0;
}
