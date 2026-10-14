#include "texture_lock_cache_buffer.cpp"

#include <cstdio>
#include <initializer_list>
#include <cstring>

signed char textureUncompBufferLocked, needToClearCacheBuffer;
UncompRecord uncompMap[1280];
int uncompSpin, texDecodeStarted;
void* uncompCacheBuffer;
static int memset_calls;
static void* memset_dest;
static int memset_value;
static unsigned memset_len;
extern "C" void* memset(void* d, int v, __SIZE_TYPE__ n) {
    ++memset_calls; memset_dest = d; memset_value = v; memset_len = n;
    return d;
}

static UncompBlock blocks[3];
static char cache[4];

static void reset(signed char lock, signed char clear) {
    textureUncompBufferLocked = lock; needToClearCacheBuffer = clear;
    for (int i = 0; i < 1280; ++i) { uncompMap[i].block = 0; uncompMap[i].unknown4 = 0x55; }
    uncompMap[0].block = &blocks[0]; uncompMap[700].block = &blocks[1]; uncompMap[1279].block = &blocks[2];
    for (auto& b : blocks) b.field_14 = 9;
    uncompSpin = 4; texDecodeStarted = 5; memset_calls = 0; uncompCacheBuffer = cache;
}
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); return 1; } } while (0)

int main() {
    for (signed char lock : {(signed char)0, (signed char)1, (signed char)2, (signed char)-1, (signed char)-128}) {
        reset(lock, 1);
        void* r = textureLockCacheBuffer__Fv();
        CHECK(r == 0);
        CHECK(textureUncompBufferLocked == (lock == 0 ? 1 : lock));
        CHECK(uncompMap[0].block == &blocks[0] && uncompMap[700].block && blocks[0].field_14 == 9);
        CHECK(uncompSpin == 4 && texDecodeStarted == 5 && needToClearCacheBuffer == 1 && memset_calls == 0);
    }
    for (signed char lock : {(signed char)3, (signed char)4, (signed char)127}) {
        reset(lock, 0);
        void* r = textureLockCacheBuffer__Fv();
        CHECK(r == cache);
        CHECK(textureUncompBufferLocked == lock);
        for (int i = 0; i < 1280; ++i) CHECK(uncompMap[i].block == 0 && uncompMap[i].unknown4 == 0x55);
        for (auto& b : blocks) CHECK(b.field_14 == 0);
        CHECK(uncompSpin == 0 && texDecodeStarted == 0 && memset_calls == 0);
        reset(lock, 1);
        r = textureLockCacheBuffer__Fv();
        CHECK(r == cache && needToClearCacheBuffer == 0);
        CHECK(memset_calls == 1 && memset_dest == cache && memset_value == 0 && memset_len == 0x500000);
    }
    std::puts("ok");
    return 0;
}
