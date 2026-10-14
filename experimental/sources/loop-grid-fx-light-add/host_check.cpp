// Host contract for norrath_light_add. Not an EE plan input.
// Pointer words are 32-bit, so every object lives in one static image.
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_floor_light_grid_view[32400] = {0};
extern "C" unsigned char norrath_light_off_grid_head_view[8];
extern "C" void norrath_light_add(void* light);

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
static void place(unsigned char* light, float x, float y) {
    std::memset(light, 0, 64);
    std::memcpy(light + 48, &x, 4);
    std::memcpy(light + 52, &y, 4);
}
static void move_to(unsigned char* light, float x, float y) {
    std::memcpy(light + 48, &x, 4);
    std::memcpy(light + 52, &y, 4);
}
static unsigned cell(int gx, int gy) { return static_cast<unsigned>((gy * 90 + gx) * 4); }

int main() {
    std::memset(norrath_floor_light_grid_view, 0, sizeof norrath_floor_light_grid_view);
    std::memset(norrath_light_off_grid_head_view, 0, sizeof norrath_light_off_grid_head_view);

    // x = y = 0 lands in cell (45, 45).
    place(lights[0], 0.0f, 0.0f);
    norrath_light_add(lights[0]);
    unsigned char* head = norrath_floor_light_grid_view + cell(45, 45);
    expect(word(head, 0) == low(lights[0]), "head takes the light");
    expect(word(lights[0], 0) == 0u, "next is the old first, zero here");
    expect(word(lights[0], 4) == low(head), "previous-link is the head word");

    // A second light in the same cell is pushed in front, and the old first links back.
    place(lights[1], 100.9f, 100.9f);   // truncates to 100 and 100, still cell (45, 45)
    norrath_light_add(lights[1]);
    expect(word(head, 0) == low(lights[1]), "second light is the new first");
    expect(word(lights[1], 0) == low(lights[0]), "its next is the old first");
    expect(word(lights[0], 4) == low(lights[1]), "old first points back at it");

    // Moving a light first unlinks it from its list. Re-add the front light elsewhere.
    move_to(lights[1], 256.0f * 3.0f, 256.0f * 2.0f);
    norrath_light_add(lights[1]);
    expect(word(head, 0) == low(lights[0]), "unlink writes next into the previous word");
    expect(word(lights[0], 4) == low(head), "unlink writes previous into next+4");
    unsigned char* moved = norrath_floor_light_grid_view + cell(48, 47);
    expect(word(moved, 0) == low(lights[1]), "x + 768 and y + 512 move the cell by 3 and 2");

    // Cell edges. -11520 is the first cell; one less leaves the grid.
    place(lights[2], -11520.0f, -11520.0f);
    norrath_light_add(lights[2]);
    expect(word(norrath_floor_light_grid_view + cell(0, 0), 0) == low(lights[2]), "-11520 is cell 0");
    place(lights[3], -11520.5f, 0.0f);   // truncates toward zero to -11520: still cell 0
    norrath_light_add(lights[3]);
    expect(word(norrath_floor_light_grid_view + cell(0, 45), 0) == low(lights[3]), "fractions truncate toward zero");
    move_to(lights[3], -11521.0f, 0.0f);
    norrath_light_add(lights[3]);
    expect(word(norrath_light_off_grid_head_view, 0) == low(lights[3]), "x below the grid goes off the grid");
    move_to(lights[3], 11519.0f, 11520.0f);   // y cell 90 is out of range
    norrath_light_add(lights[3]);
    expect(word(norrath_light_off_grid_head_view, 0) == low(lights[3]), "y cell 90 goes off the grid");
    expect(word(lights[3], 4) == low(norrath_light_off_grid_head_view), "off-grid previous-link");
    move_to(lights[3], 11519.0f, 11519.0f);   // last cell, 89 / 89
    norrath_light_add(lights[3]);
    expect(word(norrath_floor_light_grid_view + cell(89, 89), 0) == low(lights[3]), "last cell");
    expect(word(norrath_light_off_grid_head_view, 0) == 0u, "leaving the off-grid list clears its head");
    if (failures != 0) return 1;
    std::printf("light-add contract passed\n");
    return 0;
}
