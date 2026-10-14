// Host contract for runObjectsTask. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "run_objects.cpp"

#include <cstring>

extern "C" {
unsigned char* norrath_run_objects_base = nullptr;
void (*norrath_run_objects_with_arg[2])(unsigned char*, int) = {};
void (*norrath_run_objects_only[2])(unsigned char*) = {};
unsigned char deleteList[4];
unsigned char runList[4];
unsigned char mediumRunList[15 * 4];
unsigned char slowRunList[60 * 4];
unsigned char ghostObjectGrid[120 * 120 * 4];
unsigned char worldViewCenter[12];
int objectIgnoreFlags = 0;
int lastNmBlocksFree = 0;
int frameNm = 0;
signed char netNetworkMode = 0;
}

static int free_result = 0;
static int paused_result = 0;
static int free_calls = 0;
static int arg_calls = 0;
static int only_calls = 0;
static int arg_self[4];
static int arg_value[4];
static int only_self[4];
static unsigned char* constructed_head[8];
static int constructed_mask[8];
static int constructs = 0;
static int destroy_flags = 0;

static void write_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

static unsigned read_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

static void store_f32(unsigned char* address, float value) {
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    write_u32(address, bits);
}

extern "C" void __16ListSafeIteratorP8ListHeadi(unsigned char* iterator, unsigned char* head, int mask) {
    if (constructs < 8) {
        constructed_head[constructs] = head;
        constructed_mask[constructs] = mask;
    }
    ++constructs;
    write_u32(iterator + 4, read_u32(head));
}
extern "C" void next__16ListSafeIterator(unsigned char* iterator) {
    unsigned current = read_u32(iterator + 4);
    write_u32(iterator + 4, read_u32(norrath_run_objects_base + current));
}
extern "C" void _$_16ListSafeIterator(unsigned char*, int flags) { destroy_flags = flags; }
extern "C" int blockGetNmFree__Fv() {
    ++free_calls;
    return free_result;
}
extern "C" int gameIsPaused__Fv() { return paused_result; }

static void on_arg(unsigned char* self, int argument) {
    if (arg_calls < 4) {
        arg_self[arg_calls] = static_cast<int>(self - norrath_run_objects_base);
        arg_value[arg_calls] = argument;
    }
    ++arg_calls;
}
static void on_only(unsigned char* self) {
    if (only_calls < 4) only_self[only_calls] = static_cast<int>(self - norrath_run_objects_base);
    ++only_calls;
}

static void reset_counts() {
    arg_calls = 0;
    only_calls = 0;
    free_calls = 0;
    constructs = 0;
    destroy_flags = 0;
}

int main() {
    alignas(16) unsigned char heap[0x800];
    std::memset(heap, 0, sizeof heap);
    norrath_run_objects_base = heap;
    norrath_run_objects_with_arg[0] = on_arg;
    norrath_run_objects_only[1] = on_only;
    write_u32(deleteList, 0x100u);
    write_u32(heap + 0x100, 0x140u);
    write_u32(heap + 0x10c, 0x200u);
    write_u32(heap + 0x140, 0u);
    write_u32(heap + 0x14c, 0u);
    write_u32(heap + 0x200 + 72, 0x400u);
    write_u32(heap + 0x408, 4u);
    write_u32(heap + 0x40c, 0u);
    write_u32(runList, 0x180u);
    write_u32(heap + 0x180, 0x1c0u);
    write_u32(heap + 0x18c, 0x300u);
    write_u32(heap + 0x1c0, 0u);
    write_u32(heap + 0x1cc, 0x380u);
    write_u32(heap + 0x300 + 12, 0u);
    write_u32(heap + 0x300 + 72, 0x480u);
    write_u32(heap + 0x380 + 12, 0x8000u);
    write_u32(heap + 0x380 + 72, 0x480u);
    write_u32(heap + 0x480 + 32, 0u);
    write_u32(heap + 0x480 + 36, 1u);
    write_u32(heap + 0x500, 0x540u);
    write_u32(heap + 0x50c, 0x600u);
    write_u32(heap + 0x540, 0u);
    write_u32(heap + 0x54c, 0x680u);
    write_u32(heap + 0x600 + 12, 0x1000u);
    write_u32(heap + 0x600 + 72, 0x400u);
    store_f32(heap + 0x600 + 16, 0.0f);
    write_u32(heap + 0x680 + 12, 0x1000u);
    write_u32(heap + 0x680 + 72, 0x400u);
    store_f32(heap + 0x680 + 16, 1000.0f);
    write_u32(worldViewCenter, 0u);
    write_u32(worldViewCenter + 4, 0u);
    write_u32(worldViewCenter + 8, 0u);
    std::memset(mediumRunList, 0, sizeof mediumRunList);
    std::memset(slowRunList, 0, sizeof slowRunList);
    std::memset(ghostObjectGrid, 0, sizeof ghostObjectGrid);
    write_u32(ghostObjectGrid + 4, 0x500u);
    objectIgnoreFlags = 0x55;

    frameNm = 1;
    lastNmBlocksFree = 100;
    paused_result = 1;
    netNetworkMode = 0;
    reset_counts();
    runObjectsTask__Fv();
    if (free_calls != 0 || arg_calls != 1 || only_calls != 0 || constructs != 1) return 1;
    if (arg_self[0] != 0x204 || arg_value[0] != 3) return 2;
    if (constructed_head[0] != deleteList || constructed_mask[0] != 0x55 || destroy_flags != 2) return 3;
    if (lastNmBlocksFree != 100) return 4;

    frameNm = 0;
    free_result = 10;
    paused_result = 0;
    reset_counts();
    runObjectsTask__Fv();
    if (free_calls != 1 || lastNmBlocksFree != 10) return 5;
    if (arg_calls != 2 || arg_self[1] != 0x684 || arg_value[1] != 3) return 6;
    if (only_calls != 1 || only_self[0] != 0x300) return 7;
    if (constructs != 4) return 8;
    if (constructed_head[1] != runList || constructed_head[2] != mediumRunList) return 9;
    if (constructed_head[3] != slowRunList) return 10;

    netNetworkMode = 1;
    reset_counts();
    paused_result = 1;
    lastNmBlocksFree = 10;
    runObjectsTask__Fv();
    if (arg_calls != 2 || arg_self[1] != 0x604) return 11;
    if (constructs != 1) return 12;
    return 0;
}
