// Host contract for drawObjectsTask__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" void drawObjectsTask__Fv();
extern "C" signed char objectDrawEnable = 0;
extern "C" unsigned objectIgnoreFlags = 0;
extern "C" unsigned char objectGrid[120 * 120 * 4] = {};
extern "C" unsigned char ghostObjectGrid[120 * 120 * 4] = {};
extern "C" unsigned char alwaysDrawList[4] = {};
extern "C" unsigned char modelClippingOn = 0;
extern "C" unsigned glistp = 0;
extern "C" unsigned gstart = 0;
extern "C" unsigned GLISTSIZE = 35000;
extern "C" unsigned char worldCameraPos[12] = {};
extern "C" unsigned long long norrath_particles_host_base = 0;

namespace {
alignas(16) unsigned char arena[0x800];
struct Clip {
    float cube[6];
    int mode;
    bool frustum;
};
std::vector<Clip> clips;
std::vector<void*> lists;
std::vector<int> masks;
std::vector<int> dtors;
std::vector<unsigned char*> draws;
int clip_frustum_result = 1;
int object_frustum_result = 6;

unsigned read_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}

void write_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

void write_float(unsigned char* slot, float value) {
    const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
    slot[0] = bytes[0];
    slot[1] = bytes[1];
    slot[2] = bytes[2];
    slot[3] = bytes[3];
}

unsigned char* from_word(unsigned word) {
    return arena + word;
}

bool near(float value, float expect) {
    const float delta = value - expect;
    return delta < 0.01f && delta > -0.01f;
}

void reset() {
    std::memset(arena, 0, sizeof arena);
    std::memset(objectGrid, 0, sizeof objectGrid);
    std::memset(ghostObjectGrid, 0, sizeof ghostObjectGrid);
    std::memset(alwaysDrawList, 0, sizeof alwaysDrawList);
    std::memset(worldCameraPos, 0, sizeof worldCameraPos);
    write_float(worldCameraPos + 8, 1000.f);
    norrath_particles_host_base = reinterpret_cast<unsigned long long>(arena);
    clips.clear();
    lists.clear();
    masks.clear();
    dtors.clear();
    draws.clear();
    objectIgnoreFlags = 0x15u;
    modelClippingOn = 9;
    glistp = 0;
    gstart = 0;
    GLISTSIZE = 35000;
    clip_frustum_result = 1;
    object_frustum_result = 6;
}
}

extern "C" int worldClipCubeToFrustum__FPA1_fi(float* cube, int mode) {
    Clip call{};
    for (int index = 0; index < 6; ++index) call.cube[index] = cube[index];
    call.mode = mode;
    call.frustum = true;
    clips.push_back(call);
    if (cube[0] > -20.f && cube[0] < 20.f) return object_frustum_result;
    const int x0 = static_cast<int>((cube[0] + 15456.f) / 256.f + 0.5f);
    const int x1 = static_cast<int>((cube[1] + 15008.f) / 256.f + 0.5f);
    const int y0 = static_cast<int>((cube[2] + 15456.f) / 256.f + 0.5f);
    const int y1 = static_cast<int>((cube[3] + 15008.f) / 256.f + 0.5f);
    if (x0 <= 119 && x1 >= 119 && y0 <= 119 && y1 >= 119) return 4;
    return 0;
}

extern "C" int worldClipCubeToClipFrustum__FPA1_fi(float* cube, int mode) {
    Clip call{};
    for (int index = 0; index < 6; ++index) call.cube[index] = cube[index];
    call.mode = mode;
    call.frustum = false;
    clips.push_back(call);
    return clip_frustum_result;
}

extern "C" void __16ListSafeIteratorP8ListHeadi(void* iterator, void* list, int mask) {
    lists.push_back(list);
    masks.push_back(mask);
    write_word(static_cast<unsigned char*>(iterator) + 4, read_word(static_cast<unsigned char*>(list)));
}

extern "C" void next__16ListSafeIterator(void* iterator) {
    auto* raw = static_cast<unsigned char*>(iterator);
    const unsigned current = read_word(raw + 4);
    const unsigned next = current == 0u ? 0u : read_word(from_word(current));
    write_word(raw + 4, next);
}

extern "C" void _$_16ListSafeIterator(void*, int flag) {
    dtors.push_back(flag);
}

extern "C" void norrath_draw_objects_host_call(unsigned, unsigned char* object) {
    draws.push_back(object);
}

namespace {
bool expect_root_cube() {
    const Clip& call = clips[0];
    return call.mode == 0x8000 && near(call.cube[0], -15456.f) && near(call.cube[1], 15456.f) &&
           near(call.cube[2], -15456.f) && near(call.cube[3], 15456.f) && near(call.cube[4], 160.f) &&
           near(call.cube[5], 1000.f);
}

void place_object() {
    write_word(arena + 0x200, 0);
    write_word(arena + 0x200 + 12, 0x400);
    write_word(arena + 0x400 + 12, 0);
    write_float(arena + 0x400 + 16, 3.f);
    write_float(arena + 0x400 + 20, 4.f);
    write_float(arena + 0x400 + 24, 5.f);
    arena[0x400 + 40] = 2;
    arena[0x400 + 41] = 6;
    write_word(arena + 0x400 + 72, 0x500);
    arena[0x500 + 40] = 8;
    arena[0x500 + 41] = 0;
    write_word(arena + 0x500 + 44, 1u);
    write_word(objectGrid + (119 * 120 + 119) * 4, 0x200);
}
}

int main() {
    reset();
    objectDrawEnable = 0;
    drawObjectsTask__Fv();
    if (!clips.empty() || !lists.empty() || modelClippingOn != 9) {
        std::fprintf(stderr, "fail: disabled\n");
        return 1;
    }

    reset();
    objectDrawEnable = static_cast<signed char>(0x80);
    drawObjectsTask__Fv();
    if (clips.empty() || !expect_root_cube()) {
        std::fprintf(stderr, "fail: negative enable\n");
        return 1;
    }

    reset();
    objectDrawEnable = 1;
    drawObjectsTask__Fv();
    if (clips.size() < 2 || !expect_root_cube()) {
        std::fprintf(stderr, "fail: root cube\n");
        return 1;
    }
    if (clips[1].mode != 4 || !near(clips[1].cube[0], -15456.f) || !near(clips[1].cube[2], -96.f) ||
        !near(clips[1].cube[3], 15456.f)) {
        std::fprintf(stderr, "fail: high child %f %f\n", clips[1].cube[0], clips[1].cube[2]);
        return 1;
    }

    reset();
    objectDrawEnable = 1;
    place_object();
    drawObjectsTask__Fv();
    if (draws.size() != 1 || draws[0] != arena + 0x400 + 8) {
        std::fprintf(stderr, "fail: draw target\n");
        return 1;
    }
    bool saw_object = false;
    for (const Clip& call : clips) {
        if (call.frustum && near(call.cube[0], 1.f) && near(call.cube[1], 5.f) && near(call.cube[2], 2.f) &&
            near(call.cube[3], 6.f) && near(call.cube[4], 5.f) && near(call.cube[5], 11.f) && call.mode == 4) {
            saw_object = true;
        }
    }
    if (!saw_object) {
        std::fprintf(stderr, "fail: object cube\n");
        return 1;
    }
    bool saw_second = false;
    for (const Clip& call : clips) {
        if (!call.frustum && call.mode == 6 && near(call.cube[0], 1.f)) saw_second = true;
    }
    if (!saw_second) {
        std::fprintf(stderr, "fail: clip frustum\n");
        return 1;
    }
    const auto leaf = objectGrid + (119 * 120 + 119) * 4;
    if (lists.empty() || lists[0] != leaf || masks[0] != 0x15) {
        std::fprintf(stderr, "fail: leaf iterator\n");
        return 1;
    }
    if (modelClippingOn != 0) {
        std::fprintf(stderr, "fail: clipping byte %u\n", modelClippingOn);
        return 1;
    }
    for (int flag : dtors) {
        if (flag != 2) {
            std::fprintf(stderr, "fail: destructor\n");
            return 1;
        }
    }

    reset();
    objectDrawEnable = 1;
    place_object();
    GLISTSIZE = 100;
    modelClippingOn = 9;
    drawObjectsTask__Fv();
    if (!draws.empty() || modelClippingOn != 9) {
        std::fprintf(stderr, "fail: low display list\n");
        return 1;
    }

    reset();
    objectDrawEnable = 1;
    place_object();
    write_word(arena + 0x400 + 12, 2u);
    drawObjectsTask__Fv();
    if (!draws.empty()) {
        std::fprintf(stderr, "fail: skipped flag\n");
        return 1;
    }
    return 0;
}
