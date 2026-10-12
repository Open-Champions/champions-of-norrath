// Observed: textureReleaseCacheBuffer stores zero in textureUncompBufferLocked
// and the byte value 1 in needToClearCacheBuffer, then returns.
// Inferred: none.
// Unknown: who observes needToClearCacheBuffer besides textureLockCacheBuffer.

extern "C" signed char textureUncompBufferLocked;
extern "C" signed char needToClearCacheBuffer;

extern "C" void textureReleaseCacheBuffer__Fv() {
    textureUncompBufferLocked = 0;
    needToClearCacheBuffer = 1;
}
