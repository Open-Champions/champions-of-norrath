// Host contract for lightUpdatePosition__FP5Light. Not an EE plan input.
// Pointer words are 32-bit, so every object lives in one static image.
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_floor_light_grid_view[32400] = {0};
extern "C" unsigned char norrath_light_off_grid_head_view[8] = {0};
extern "C" void lightUpdatePosition__FP5Light(void* light);

static unsigned char lights[4][64];
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" void* norrath_host_pointer(unsigned address) {
    const unsigned long base = reinterpret_cast<unsigned long>(lights);
    return reinterpret_cast<void*>((base & ~0xffffffffUL) | address);
}
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
static unsigned word(const void* base, unsigned offset) {
    unsigned v;
    std::memcpy(&v, static_cast<const unsigned char*>(base) + offset, 4);
    return v;
}
static void setword(void* base, unsigned offset, unsigned v) { std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 4); }
static void place(unsigned char* light, float x, float y) {
    std::memset(light, 0, 64);
    std::memcpy(light + 48, &x, 4);
    std::memcpy(light + 52, &y, 4);
}
static unsigned cell(int gx, int gy) { return static_cast<unsigned>((gy * 90 + gx) * 4); }

int main() {
    // A light that is not in a list (previous-link zero) is left alone.
    place(lights[0], 0.0f, 0.0f);
    lightUpdatePosition__FP5Light(lights[0]);
    unsigned char* head = norrath_floor_light_grid_view + cell(45, 45);
    expect(word(head, 0) == 0u && word(lights[0], 4) == 0u, "unlinked light is not touched");
    // Link two lights into the cell by hand: lights[1] first, then lights[0].
    setword(head, 0, low(lights[1]));
    place(lights[1], 100.9f, 100.9f);
    setword(lights[1], 0, low(lights[0])); setword(lights[1], 4, low(head));
    setword(lights[0], 4, low(lights[1]));
    lightUpdatePosition__FP5Light(lights[1]);
    expect(word(head, 0) == low(lights[1]) && word(lights[1], 0) == low(lights[0]) && word(lights[0], 4) == low(lights[1]), "same cell: unlinked and put back in front");
    // Move lights[1] to another cell: it unlinks from the old list first.
    float nx = 768.0f, ny = 512.0f;
    std::memcpy(lights[1] + 48, &nx, 4); std::memcpy(lights[1] + 52, &ny, 4);
    lightUpdatePosition__FP5Light(lights[1]);
    expect(word(head, 0) == low(lights[0]) && word(lights[0], 4) == low(head), "old list repaired");
    expect(word(norrath_floor_light_grid_view + cell(48, 47), 0) == low(lights[1]), "new cell");
    // The light's own next is not cleared before reinsertion; with an empty destination its
    // next becomes zero because the old first is read from the head.
    place(lights[2], 0.0f, 0.0f);
    setword(lights[2], 0, low(lights[3]));
    setword(lights[2], 4, low(lights[3]) + 8);
    place(lights[3], 0.0f, 0.0f);
    lightUpdatePosition__FP5Light(lights[2]);
    expect(word(lights[3], 8) == low(lights[3]), "previous word receives next");
    expect(word(lights[2], 4) == low(head), "previous-link is the head");
    // Off the grid.
    place(lights[2], -11521.0f, 0.0f);
    setword(lights[2], 4, low(lights[2]) + 8);
    lightUpdatePosition__FP5Light(lights[2]);
    expect(word(norrath_light_off_grid_head_view, 0) == low(lights[2]) && word(lights[2], 4) == low(norrath_light_off_grid_head_view), "off-grid head");
    if (failures) return 1;
    std::printf("lightUpdatePosition contract passed\n");
    return 0;
}
