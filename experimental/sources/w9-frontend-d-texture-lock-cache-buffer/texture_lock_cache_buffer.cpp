// Observed: textureLockCacheBuffer first stores 1 in textureUncompBufferLocked
// when it is zero. If the byte, read as signed, is below 3 the function
// returns null and changes nothing else. Otherwise it zeroes all 1280 eight-byte
// uncompMap records (clearing the word at +0x14 of any record the first word
// points to), zeroes uncompSpin and texDecodeStarted, and, when
// needToClearCacheBuffer is set, clears that byte and fills 0x500000 bytes at
// uncompCacheBuffer with zero. It returns uncompCacheBuffer.
// Inferred: none. The function never raises the lock byte above 1 itself.
// Unknown: who sets the lock byte to 3 or more, the type behind the record's
// first word, and what the word at +0x14 means.

struct UncompBlock {
    unsigned char unknown0[0x14];
    int field_14;
};

struct UncompRecord {
    UncompBlock* block;
    unsigned unknown4;
};

extern "C" signed char textureUncompBufferLocked;
extern "C" signed char needToClearCacheBuffer;
extern "C" UncompRecord uncompMap[1280];
extern "C" int uncompSpin;
extern "C" int texDecodeStarted;
extern "C" void* uncompCacheBuffer;
extern "C" void* memset(void* destination, int value, __SIZE_TYPE__ length);

extern "C" void* textureLockCacheBuffer__Fv() {
    if (textureUncompBufferLocked == 0) {
        textureUncompBufferLocked = 1;
    }
    if (textureUncompBufferLocked < 3) {
        return 0;
    }
    for (int i = 1279; i >= 0; --i) {
        if (uncompMap[i].block != 0) {
            uncompMap[i].block->field_14 = 0;
        }
        uncompMap[i].block = 0;
    }
    uncompSpin = 0;
    texDecodeStarted = 0;
    if (needToClearCacheBuffer != 0) {
        needToClearCacheBuffer = 0;
        memset(uncompCacheBuffer, 0, 0x500000);
    }
    return uncompCacheBuffer;
}
