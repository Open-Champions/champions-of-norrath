#include "texture_release_cache_buffer.cpp"

#include <cstdio>

signed char textureUncompBufferLocked, needToClearCacheBuffer;

int main() {
    textureUncompBufferLocked = 3;
    needToClearCacheBuffer = 0;
    textureReleaseCacheBuffer__Fv();
    if (textureUncompBufferLocked != 0 || needToClearCacheBuffer != 1) { std::puts("FAIL"); return 1; }
    textureUncompBufferLocked = 0;
    needToClearCacheBuffer = 1;
    textureReleaseCacheBuffer__Fv();
    if (textureUncompBufferLocked != 0 || needToClearCacheBuffer != 1) { std::puts("FAIL again"); return 1; }
    std::puts("ok");
    return 0;
}
