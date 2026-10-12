// Observed: texturePrepareToLockCacheBuffer stores the byte value 1 in
// textureUncompBufferLocked and returns.
// Inferred: none.
// Unknown: the meaning of the values above 1; textureLockCacheBuffer only
// treats values of 3 and more as a request to clear the cache.

extern "C" signed char textureUncompBufferLocked;

extern "C" void texturePrepareToLockCacheBuffer__Fv() {
    textureUncompBufferLocked = 1;
}
