#include "texture_prepare_to_lock_cache_buffer.cpp"

#include <cstdio>

signed char textureUncompBufferLocked;

int main() {
    const signed char before[] = {0, 1, 2, 3, 100, -1};
    for (signed char start : before) {
        textureUncompBufferLocked = start;
        texturePrepareToLockCacheBuffer__Fv();
        if (textureUncompBufferLocked != 1) { std::printf("FAIL from %d\n", start); return 1; }
    }
    std::puts("ok");
    return 0;
}
