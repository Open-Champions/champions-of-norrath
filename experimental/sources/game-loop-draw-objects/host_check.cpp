// Host contract for drawObjectsTask. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "draw_objects.cpp"

#include <cmath>
#include <cstring>

extern "C" {
unsigned char* norrath_draw_objects_base = nullptr;
void (*norrath_draw_objects_slots[4])(unsigned char*) = {};
signed char objectDrawEnable = 1;
int objectIgnoreFlags = 0x55;
unsigned char modelClippingOn = 9;
unsigned char objectGrid[120 * 120 * 4];
unsigned char ghostObjectGrid[120 * 120 * 4];
unsigned char alwaysDrawList[4];
float worldCameraPos[3] = {0.0f, 0.0f, 100.0f};
int GLISTSIZE = 100000;
unsigned char* glistp = nullptr;
unsigned char* gstart = nullptr;
}

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

static int destroys = 0;
static int constructs = 0;
static int grid_calls = 0;
static int object_clip_calls = 0;
static int model_clip_calls = 0;
static int draws = 0;
static int drawn_object[16];
static int draw_flag[16];
static int object_mask[16];
static int object_mask_calls = 0;
static float last_object_box[6];
static int clip_arg = 0;
static int always_constructs = 0;

// Cells: A is partial and holds list objects, B is entirely inside.
static const int ax = 5, ay = 7, bx = 60, by = 60;

extern "C" void __16ListSafeIteratorP8ListHeadi(unsigned char* iterator, unsigned char* head, int mask) {
    if (mask != 0x55) constructs += 1000;
    if (head == alwaysDrawList) ++always_constructs;
    ++constructs;
    write_u32(iterator + 4, read_u32(head));
}
extern "C" void next__16ListSafeIterator(unsigned char* iterator) {
    unsigned current = read_u32(iterator + 4);
    write_u32(iterator + 4, read_u32(norrath_draw_objects_base + current));
}
extern "C" void _$_16ListSafeIterator(unsigned char*, int flags) {
    if (flags == 2) ++destroys;
}

extern "C" int worldClipCubeToFrustum__FPA1_fi(float* box, int mask) {
    if (box[5] == 100.0f && box[4] == -740.0f) {
        ++grid_calls;
        int x0 = static_cast<int>(std::lround((box[0] + 15456.0f) / 256.0f));
        int x1 = static_cast<int>(std::lround((box[1] + 15008.0f) / 256.0f));
        int y0 = static_cast<int>(std::lround((box[2] + 15456.0f) / 256.0f));
        int y1 = static_cast<int>(std::lround((box[3] + 15008.0f) / 256.0f));
        bool has_a = x0 <= ax && ax <= x1 && y0 <= ay && ay <= y1;
        bool has_b = x0 <= bx && bx <= x1 && y0 <= by && by <= y1;
        if (x0 == bx && x1 == bx && y0 == by && y1 == by) return 0x803f;
        if (has_a || has_b) return 0x8001;
        return 0;
    }
    ++object_clip_calls;
    std::memcpy(last_object_box, box, sizeof last_object_box);
    if (object_mask_calls < 16) object_mask[object_mask_calls] = mask;
    ++object_mask_calls;
    // Objects with z == 3000 are outside.
    return box[4] == 3000.0f ? 0 : 7;
}
extern "C" int worldClipCubeToClipFrustum__FPA1_fi(float* box, int mask) {
    ++model_clip_calls;
    clip_arg = mask;
    return box[4] == 2000.0f ? 0x803f : 3;
}

static void on_draw(unsigned char* self) {
    if (draws < 16) {
        drawn_object[draws] = static_cast<int>(self - norrath_draw_objects_base);
        draw_flag[draws] = modelClippingOn;
    }
    ++draws;
}

static void make_object(unsigned char* heap, unsigned at, unsigned flags, float x, float y, float z,
                        unsigned radius, unsigned up, unsigned table, int adjust) {
    write_u32(heap + at + 12, flags);
    store_f32(heap + at + 16, x);
    store_f32(heap + at + 20, y);
    store_f32(heap + at + 24, z);
    heap[at + 40] = static_cast<unsigned char>(radius);
    heap[at + 41] = static_cast<unsigned char>(up);
    write_u32(heap + at + 72, table);
    heap[table + 40] = static_cast<unsigned char>(adjust);
    heap[table + 41] = static_cast<unsigned char>(adjust >> 8);
    write_u32(heap + table + 44, 0);
}

static void make_node(unsigned char* heap, unsigned at, unsigned next, unsigned object) {
    write_u32(heap + at, next);
    write_u32(heap + at + 12, object);
}

int main() {
    alignas(16) static unsigned char heap[0x4000];
    std::memset(heap, 0, sizeof heap);
    norrath_draw_objects_base = heap;
    norrath_draw_objects_slots[0] = on_draw;
    gstart = heap;
    glistp = heap;

    unsigned table = 0x3000;
    // Cell B: one object, never box-tested.
    make_object(heap, 0x400, 0, 1, 1, 50, 2, 3, table, 0);
    make_node(heap, 0x100, 0, 0x400);
    write_u32(objectGrid + (by * 120 + bx) * 4, 0x100);
    // Cell A list: flagged object, inside object, outside object, partial object.
    make_object(heap, 0x500, 2, 1, 1, 1000, 1, 1, table, 0);
    make_object(heap, 0x600, 0, 10, 20, 2000, 4, 6, table + 0x100, 16);
    make_object(heap, 0x700, 0, 1, 1, 3000, 1, 1, table, 0);
    make_object(heap, 0x800, 0, 1, 1, 1500, 1, 1, table, 0);
    make_node(heap, 0x110, 0x120, 0x500);
    make_node(heap, 0x120, 0x130, 0x600);
    make_node(heap, 0x130, 0x140, 0x700);
    make_node(heap, 0x140, 0, 0x800);
    write_u32(objectGrid + (ay * 120 + ax) * 4, 0x110);
    // Ghost list at A, and another ghost list at B.
    make_object(heap, 0x900, 0, 1, 1, 1600, 1, 1, table, 0);
    make_node(heap, 0x150, 0, 0x900);
    write_u32(ghostObjectGrid + (ay * 120 + ax) * 4, 0x150);
    make_object(heap, 0xa00, 2, 1, 1, 0, 1, 1, table, 0);
    make_object(heap, 0xb00, 0, 1, 1, 0, 1, 1, table, 0);
    make_node(heap, 0x160, 0x170, 0xa00);
    make_node(heap, 0x170, 0, 0xb00);
    write_u32(ghostObjectGrid + (by * 120 + bx) * 4, 0x160);
    // Always-draw list.
    make_object(heap, 0xc00, 2, 0, 0, 0, 0, 0, table, 0);
    make_node(heap, 0x180, 0, 0xc00);
    write_u32(alwaysDrawList, 0x180);

    objectDrawEnable = 0;
    drawObjectsTask__Fv();
    if (grid_calls != 0 || draws != 0 || constructs != 0 || modelClippingOn != 9) return 1;

    objectDrawEnable = 1;
    GLISTSIZE = 3000;
    drawObjectsTask__Fv();
    if (constructs >= 1000) return 2;
    if (always_constructs != 1) return 3;
    if (destroys != constructs) return 4;
    // Cell B (upper half) first, then cell A, then the always-draw list.
    if (draws != 6) return 5;
    if (drawn_object[0] != 0x400 || draw_flag[0] != 0) return 6;
    if (drawn_object[1] != 0xb00 || draw_flag[1] != 0) return 7;
    if (drawn_object[2] != 0x600 + 16 || draw_flag[2] != 0) return 8;
    if (drawn_object[3] != 0x800 || draw_flag[3] != 1) return 9;
    if (drawn_object[4] != 0x900 || draw_flag[4] != 1) return 10;
    if (drawn_object[5] != 0xc00 || draw_flag[5] != 1) return 11;
    if (modelClippingOn != 0) return 12;
    if (model_clip_calls != 2 || clip_arg != 7) return 13;
    if (object_clip_calls != 4) return 14;
    for (int i = 0; i < 4; ++i) {
        if (object_mask[i] != 0x8001) return 15;
    }
    const float expected[6] = {0.0f, 2.0f, 0.0f, 2.0f, 1600.0f, 1601.0f};
    for (int i = 0; i < 6; ++i) {
        if (last_object_box[i] != expected[i]) return 16;
    }

    // A display list one entry short of 3000 draws nothing. Cell B clears the
    // flag before its first room check, so that clear is the last write.
    draws = 0;
    destroys = 0;
    constructs = 0;
    always_constructs = 0;
    modelClippingOn = 9;
    GLISTSIZE = 2999;
    drawObjectsTask__Fv();
    if (draws != 0 || modelClippingOn != 0) return 17;
    if (destroys != constructs || constructs != 1) return 18;

    // One 8-byte display list unit used: 3001 - 1 is 3000, enough; 3000 - 1 is not.
    GLISTSIZE = 3001;
    glistp = heap + 8;
    drawObjectsTask__Fv();
    if (draws != 6) return 19;
    GLISTSIZE = 3000;
    draws = 0;
    modelClippingOn = 9;
    drawObjectsTask__Fv();
    if (draws != 0 || modelClippingOn != 0) return 20;
    return 0;
}
