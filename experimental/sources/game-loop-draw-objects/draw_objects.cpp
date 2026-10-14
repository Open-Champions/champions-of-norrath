// Observed drawObjectsTask at 0x0013f930, 1856 bytes.
// A zero signed byte objectDrawEnable returns at once. Otherwise a work stack
// starts with one record: clip word 0x8000 and the cell rectangle (0,0)-(119,119).
// Each popped record that does not already hold 0x803f is turned into a box:
// x from (x0 << 8) - 15456 to (x1 << 8) - 15008, y from (y0 << 8) - 15456 to
// (y1 << 8) - 15008, z from worldCameraPos+8 minus 840.0 to worldCameraPos+8.
// worldClipCubeToFrustum replaces the clip word, and zero drops the record.
// A rectangle larger than one cell is split: when height is less than width
// at x midpoint (x0 + x1) >> 1, otherwise at the y midpoint. The upper half
// is pushed second and so is visited first. A one-cell rectangle walks
// objectGrid[y * 120 + x] and then ghostObjectGrid at the same index, each
// through a ListSafeIterator with objectIgnoreFlags. Objects whose flags
// (byte 12 of the object) include 2 are skipped. A record with 0x803f clears
// modelClippingOn and draws without a per-object test. Any other clip word
// tests a box built from object+16, +20, +24: x and y widened by the byte at
// +40, z extended upward by the byte at +41. Zero skips the object. Before
// each draw the display list room, GLISTSIZE minus ((glistp - gstart) >> 3),
// must not be below 3000; below that the whole task returns, leaving
// modelClippingOn as it was. objectGrid objects set modelClippingOn from
// worldClipCubeToClipFrustum (0 for 0x803f, else 1); ghostObjectGrid objects
// do not touch it. The draw call is the slot at vtable+40 (vtable at object+72)
// with the sign-extended halfword adjustment and no other argument. When the
// stack empties, modelClippingOn becomes 1, alwaysDrawList is drawn the same
// way with no flag test or box, and modelClippingOn becomes 0.
// Unknown: what the 0x8000 start word and the 840.0 depth mean.
extern "C" void __16ListSafeIteratorP8ListHeadi(unsigned char* iterator, unsigned char* head, int mask);
extern "C" void next__16ListSafeIterator(unsigned char* iterator);
extern "C" void _$_16ListSafeIterator(unsigned char* iterator, int flags);
extern "C" int worldClipCubeToFrustum__FPA1_fi(float* box, int mask);
extern "C" int worldClipCubeToClipFrustum__FPA1_fi(float* box, int mask);
extern "C" signed char objectDrawEnable;
extern "C" int objectIgnoreFlags;
extern "C" unsigned char modelClippingOn;
extern "C" unsigned char objectGrid[];
extern "C" unsigned char ghostObjectGrid[];
extern "C" unsigned char alwaysDrawList[];
extern "C" float worldCameraPos[];
extern "C" int GLISTSIZE;
extern "C" unsigned char* glistp;
extern "C" unsigned char* gstart;

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_draw_objects_base;
extern "C" void (*norrath_draw_objects_slots[])(unsigned char*);
#endif

namespace {
unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

unsigned load_u16(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8);
}

float load_f32(const unsigned char* address) {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = load_u32(address);
    return value.as_float;
}

unsigned char* from_word(unsigned word) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_draw_objects_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

bool is_null(const unsigned char* pointer) {
#if defined(NORRATH_HOST_CONTRACT)
    return pointer == norrath_draw_objects_base;
#else
    return pointer == nullptr;
#endif
}

unsigned char* pointer_at(const unsigned char* address) {
    return from_word(load_u32(address));
}

void draw_object(unsigned char* object) {
    unsigned char* table = pointer_at(object + 72);
    int adjustment = static_cast<short>(load_u16(table + 40));
    unsigned code = load_u32(table + 44);
    unsigned char* self = object + adjustment;
#if defined(NORRATH_HOST_CONTRACT)
    norrath_draw_objects_slots[code](self);
#else
    auto function = reinterpret_cast<void (*)(unsigned char*)>(static_cast<__UINTPTR_TYPE__>(code));
    function(self);
#endif
}

bool display_list_low() {
    int used = static_cast<int>((glistp - gstart)) >> 3;
    return GLISTSIZE - used < 3000;
}

struct Record {
    int clip;
    short x0;
    short y0;
    short x1;
    short y1;
};

// Which kind of walk: whole cell unclipped, clipped with the model flag, or
// clipped without it.
enum Walk { WalkInside, WalkClipped, WalkGhost };

// Returns false when the display list ran low.
bool walk_list(unsigned char* iterator, unsigned char* head, int clip, Walk kind) {
    __16ListSafeIteratorP8ListHeadi(iterator, head, objectIgnoreFlags);
    unsigned char* node = pointer_at(iterator + 4);
    while (!is_null(node)) {
        unsigned char* object = pointer_at(node + 12);
        if ((load_u32(object + 12) & 2u) == 0u) {
            int result = clip;
            float box[6];
            if (kind != WalkInside) {
                float x = load_f32(object + 16);
                float y = load_f32(object + 20);
                float z = load_f32(object + 24);
                float radius = static_cast<float>(object[40]);
                box[0] = x - radius;
                box[1] = x + radius;
                box[2] = y - radius;
                box[3] = y + radius;
                box[4] = z;
                box[5] = z + static_cast<float>(object[41]);
                result = worldClipCubeToFrustum__FPA1_fi(box, clip);
            }
            if (kind == WalkInside || result != 0) {
                if (display_list_low()) {
                    _$_16ListSafeIterator(iterator, 2);
                    return false;
                }
                if (kind == WalkClipped) {
                    modelClippingOn = worldClipCubeToClipFrustum__FPA1_fi(box, result) == 0x803f ? 0 : 1;
                }
                draw_object(object);
            }
        }
        next__16ListSafeIterator(iterator);
        node = pointer_at(iterator + 4);
    }
    _$_16ListSafeIterator(iterator, 2);
    return true;
}
}

extern "C" void drawObjectsTask__Fv() {
    unsigned char iterator[32];
    Record stack[64];
    int count;
    if (objectDrawEnable == 0) return;
    stack[0].clip = 0x8000;
    stack[0].x0 = 0;
    stack[0].y0 = 0;
    stack[0].x1 = 119;
    stack[0].y1 = 119;
    count = 1;
    do {
        --count;
        int clip = stack[count].clip;
        int x0 = stack[count].x0;
        int y0 = stack[count].y0;
        int x1 = stack[count].x1;
        int y1 = stack[count].y1;
        if (clip != 0x803f) {
            float box[6];
            float top = worldCameraPos[2];
            box[0] = static_cast<float>((x0 << 8) - 15456);
            box[1] = static_cast<float>((x1 << 8) - 15008);
            box[2] = static_cast<float>((y0 << 8) - 15456);
            box[3] = static_cast<float>((y1 << 8) - 15008);
            box[4] = top - 840.0f;
            box[5] = top;
            clip = worldClipCubeToFrustum__FPA1_fi(box, clip);
            if (clip == 0) continue;
        }
        if (x0 < x1 || y0 < y1) {
            if (y1 - y0 < x1 - x0) {
                int middle = (x0 + x1) >> 1;
                stack[count].clip = clip;
                stack[count].x0 = static_cast<short>(x0);
                stack[count].y0 = static_cast<short>(y0);
                stack[count].x1 = static_cast<short>(middle);
                stack[count].y1 = static_cast<short>(y1);
                stack[count + 1].clip = clip;
                stack[count + 1].x0 = static_cast<short>(middle + 1);
                stack[count + 1].y0 = static_cast<short>(y0);
                stack[count + 1].x1 = static_cast<short>(x1);
                stack[count + 1].y1 = static_cast<short>(y1);
            } else {
                int middle = (y0 + y1) >> 1;
                stack[count].clip = clip;
                stack[count].x0 = static_cast<short>(x0);
                stack[count].y0 = static_cast<short>(y0);
                stack[count].x1 = static_cast<short>(x1);
                stack[count].y1 = static_cast<short>(middle);
                stack[count + 1].clip = clip;
                stack[count + 1].x0 = static_cast<short>(x0);
                stack[count + 1].y0 = static_cast<short>(middle + 1);
                stack[count + 1].x1 = static_cast<short>(x1);
                stack[count + 1].y1 = static_cast<short>(y1);
            }
            count += 2;
            continue;
        }
        unsigned offset = static_cast<unsigned>(y0 * 120 + x0) * 4u;
        if (clip == 0x803f) {
            modelClippingOn = 0;
            if (!walk_list(iterator, objectGrid + offset, clip, WalkInside)) return;
            if (!walk_list(iterator, ghostObjectGrid + offset, clip, WalkInside)) return;
        } else {
            if (!walk_list(iterator, objectGrid + offset, clip, WalkClipped)) return;
            if (!walk_list(iterator, ghostObjectGrid + offset, clip, WalkGhost)) return;
        }
    } while (count > 0);
    modelClippingOn = 1;
    __16ListSafeIteratorP8ListHeadi(iterator, alwaysDrawList, objectIgnoreFlags);
    for (unsigned char* node = pointer_at(iterator + 4); !is_null(node); node = pointer_at(iterator + 4)) {
        if (display_list_low()) {
            _$_16ListSafeIterator(iterator, 2);
            return;
        }
        draw_object(pointer_at(node + 12));
        next__16ListSafeIterator(iterator);
    }
    _$_16ListSafeIterator(iterator, 2);
    modelClippingOn = 0;
}
