#define NORRATH_HOST_CONTRACT 1
#include "raster_sprite.cpp"
#include <string.h>

static int calls;
static void* seenSelf; static void* seenCamera; static void* seenScene; static int seenWord; static unsigned seenFlags;
static int fake(void* self, void* camera, void* scene, int word, unsigned flags) {
    ++calls; seenSelf = self; seenCamera = camera; seenScene = scene; seenWord = word; seenFlags = flags;
    return 1234;
}

int main() {
    alignas(16) static unsigned char scene[20000];
    alignas(16) static unsigned char records[12 * 4];
    alignas(16) static unsigned char object[128];
    alignas(16) static unsigned char table[64];
    unsigned char camera[8];
    // Table entry at +24: adjustment 40 and the function pointer at +4 within the entry.
    short adjust = 40;
    memcpy(table + 24, &adjust, 2);
    int (*fn)(void*, void*, void*, int, unsigned) = fake;
    memcpy(table + 28, &fn, sizeof fn);
    unsigned char* tablePtr = table;
    memcpy(object + 60, &tablePtr, sizeof tablePtr);
    unsigned char* objectPtr = object;
    memcpy(records + 12 * 2, &objectPtr, sizeof objectPtr);
    unsigned char* recordsPtr = records;
    memcpy(scene + 1004, &recordsPtr, sizeof recordsPtr);
    // Null scene word: -1 and no call.
    memset(scene + 19356, 0, 4);
    if (RasterSprite__7VISceneR8VICameraiUi(scene, camera, 2, 9) != -1 || calls != 0) return 1;
    int word = 0x55aa;
    memcpy(scene + 19356, &word, 4);
    if (RasterSprite__7VISceneR8VICameraiUi(scene, camera, 2, 0xfedc) != 1234 || calls != 1) return 2;
    if (seenSelf != object + 40 || seenCamera != camera || seenScene != scene || seenWord != 0x55aa || seenFlags != 0xfedc) return 3;
    return 0;
}
