// drawObjectsTask__Fv at 0x0013f930, 1852 bytes, read from the pinned
// SLUS-20565 image. It is registered as a task; main .text has no direct
// call. Fresh compilation is not an original-byte match. No object has been
// drawn on the EE.
//
// A zero objectDrawEnable byte returns immediately. Otherwise the task walks
// a 120 by 120 cell range, starting at mode 0x8000 and corners (0, 0) and
// (119, 119). A mode other than 0x803f is replaced by worldClipCubeToFrustum
// of the cell cube. A zero result discards the cell. A cell that still spans
// more than one column or row is split on its longer axis, or on Y when the
// spans are equal. The high half is visited first.
//
// A one-cell result walks objectGrid, then ghostObjectGrid. Bit 2 of the
// word at object+12 skips that object. The other objects are clipped, and a
// display-list remainder below 3000 destroys the iterator and returns. The
// remainder is GLISTSIZE minus the arithmetic shift of glistp - gstart.
// Mode 0x803f skips the cubes and clears modelClippingOn before the object
// grid. Other modes call worldClipCubeToClipFrustum and store 0 or 1 in
// modelClippingOn. The draw is the halfword at vtable+40 added to the
// object, and the word at vtable+44. alwaysDrawList is walked last with
// modelClippingOn set to 1, and that byte is cleared if the walk finishes.
extern "C" {
extern signed char objectDrawEnable;
extern unsigned objectIgnoreFlags;
extern unsigned char objectGrid[];
extern unsigned char ghostObjectGrid[];
extern unsigned char alwaysDrawList[];
extern unsigned char modelClippingOn;
extern unsigned glistp;
extern unsigned gstart;
extern unsigned GLISTSIZE;
extern unsigned char worldCameraPos[];

int worldClipCubeToFrustum__FPA1_fi(float* cube, int mode);
int worldClipCubeToClipFrustum__FPA1_fi(float* cube, int mode);
void __16ListSafeIteratorP8ListHeadi(void* iterator, void* list, int mask);
void next__16ListSafeIterator(void* iterator);
void _$_16ListSafeIterator(void* iterator, int flag);

#if defined(NORRATH_PARTICLES_HOST)
extern unsigned long long norrath_particles_host_base;
extern void norrath_draw_objects_host_call(unsigned function, unsigned char* object);
#endif

static unsigned read_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}

static unsigned short read_half(const unsigned char* slot) {
    return static_cast<unsigned short>(slot[0] | (slot[1] << 8));
}

static unsigned char* from_word(unsigned word) {
    if (word == 0u) return nullptr;
#if defined(NORRATH_PARTICLES_HOST)
    return reinterpret_cast<unsigned char*>(norrath_particles_host_base + word);
#else
    return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(word));
#endif
}

static float bits_float(unsigned bits) {
    unsigned char bytes[4] = {
        static_cast<unsigned char>(bits), static_cast<unsigned char>(bits >> 8),
        static_cast<unsigned char>(bits >> 16), static_cast<unsigned char>(bits >> 24)};
    float value = 0.f;
    auto* raw = reinterpret_cast<unsigned char*>(&value);
    raw[0] = bytes[0];
    raw[1] = bytes[1];
    raw[2] = bytes[2];
    raw[3] = bytes[3];
    return value;
}

static float load_float(const unsigned char* slot) {
    return bits_float(read_word(slot));
}

static bool display_low() {
    const unsigned difference = glistp - gstart;
    const int slots = static_cast<int>(difference) >> 3;
    const int remain = static_cast<int>(GLISTSIZE - static_cast<unsigned>(slots));
    return remain < 3000;
}

static void call_draw(unsigned char* object) {
    unsigned char* vtable = from_word(read_word(object + 72));
    const int adjust = static_cast<short>(read_half(vtable + 40));
    const unsigned function = read_word(vtable + 44);
    unsigned char* target = object + adjust;
#if defined(NORRATH_PARTICLES_HOST)
    norrath_draw_objects_host_call(function, target);
#else
    auto* draw = reinterpret_cast<void (*)(unsigned char*)>(static_cast<unsigned long>(function));
    draw(target);
#endif
}

static void build_region_cube(short x0, short y0, short x1, short y1, float* cube) {
    const float camera_z = load_float(worldCameraPos + 8);
    const float lift = bits_float(0x44520000u);
    cube[0] = static_cast<float>((static_cast<int>(x0) << 8) - 15456);
    cube[1] = static_cast<float>((static_cast<int>(x1) << 8) - 15008);
    cube[2] = static_cast<float>((static_cast<int>(y0) << 8) - 15456);
    cube[3] = static_cast<float>((static_cast<int>(y1) << 8) - 15008);
    cube[4] = camera_z - lift;
    cube[5] = camera_z;
}

static void build_object_cube(const unsigned char* object, float* cube) {
    const float radius = static_cast<float>(object[40]);
    const float x = load_float(object + 16);
    const float y = load_float(object + 20);
    const float z = load_float(object + 24);
    cube[0] = x - radius;
    cube[1] = x + radius;
    cube[2] = y - radius;
    cube[3] = y + radius;
    cube[4] = z;
    cube[5] = z + static_cast<float>(object[41]);
}

// kind 0 clips twice, kind 1 clips once, kind 2 and 3 do not clip.
// kind 3 ignores the skip bit.
static bool walk_list(unsigned char* list, unsigned mode, int kind) {
    unsigned char iterator[16] = {};
    __16ListSafeIteratorP8ListHeadi(iterator, list, static_cast<int>(objectIgnoreFlags));
    unsigned current = read_word(iterator + 4);
    while (current != 0u) {
        unsigned char* node = from_word(current);
        unsigned char* object = from_word(read_word(node + 12));
        const bool skipped = kind != 3 && (read_word(object + 12) & 2u) != 0u;
        if (!skipped) {
            if (kind < 2) {
                float cube[6];
                build_object_cube(object, cube);
                const int clipped = worldClipCubeToFrustum__FPA1_fi(cube, static_cast<int>(mode));
                if (clipped != 0) {
                    if (display_low()) {
                        _$_16ListSafeIterator(iterator, 2);
                        return true;
                    }
                    if (kind == 0) {
                        const int second = worldClipCubeToClipFrustum__FPA1_fi(cube, clipped);
                        modelClippingOn = second == 0x803f ? 0u : 1u;
                    }
                    call_draw(object);
                }
            } else {
                if (display_low()) {
                    _$_16ListSafeIterator(iterator, 2);
                    return true;
                }
                call_draw(object);
            }
        }
        next__16ListSafeIterator(iterator);
        current = read_word(iterator + 4);
    }
    _$_16ListSafeIterator(iterator, 2);
    return false;
}

struct Cell {
    unsigned mode;
    short x0;
    short y0;
    short x1;
    short y1;
};

static unsigned char* cell_at(unsigned char* grid, short x, short y) {
    const unsigned index = static_cast<unsigned>(y) * 120u + static_cast<unsigned>(x);
    return grid + index * 4u;
}

void drawObjectsTask__Fv() {
    if (objectDrawEnable == 0) return;
    Cell stack[256];
    int count = 0;
    stack[count++] = Cell{0x8000u, 0, 0, 119, 119};
    while (count > 0) {
        const Cell cell = stack[--count];
        unsigned mode = cell.mode;
        if (mode != 0x803fu) {
            float cube[6];
            build_region_cube(cell.x0, cell.y0, cell.x1, cell.y1, cube);
            mode = static_cast<unsigned>(worldClipCubeToFrustum__FPA1_fi(cube, static_cast<int>(mode)));
            if (mode == 0u) continue;
        }
        if (cell.x0 < cell.x1 || cell.y0 < cell.y1) {
            if ((cell.y1 - cell.y0) < (cell.x1 - cell.x0)) {
                const short mid = static_cast<short>((static_cast<int>(cell.x0) + cell.x1) >> 1);
                stack[count++] = Cell{mode, cell.x0, cell.y0, mid, cell.y1};
                stack[count++] = Cell{mode, static_cast<short>(mid + 1), cell.y0, cell.x1, cell.y1};
            } else {
                const short mid = static_cast<short>((static_cast<int>(cell.y0) + cell.y1) >> 1);
                stack[count++] = Cell{mode, cell.x0, cell.y0, cell.x1, mid};
                stack[count++] = Cell{mode, cell.x0, static_cast<short>(mid + 1), cell.x1, cell.y1};
            }
            continue;
        }
        unsigned char* object_cell = cell_at(objectGrid, cell.x0, cell.y0);
        unsigned char* ghost_cell = cell_at(ghostObjectGrid, cell.x0, cell.y0);
        if (mode == 0x803fu) {
            modelClippingOn = 0;
            if (walk_list(object_cell, mode, 2)) return;
            if (walk_list(ghost_cell, mode, 2)) return;
        } else {
            if (walk_list(object_cell, mode, 0)) return;
            if (walk_list(ghost_cell, mode, 1)) return;
        }
    }
    modelClippingOn = 1;
    if (walk_list(alwaysDrawList, 0u, 3)) return;
    modelClippingOn = 0;
}
}
