// Host contract for objectUpdateInGrid__FP10GameObject. Not an EE plan input.
// Pointer words are 32-bit, so every object lives in one static image.
#include <cstdio>
#include <cstring>

extern "C" unsigned char objectGrid[57600] = {0};
extern "C" unsigned char ghostObjectGrid[57600] = {0};
extern "C" unsigned char offTheGrid[8] = {0};
extern "C" void objectUpdateInGrid__FP10GameObject(void* object);

static unsigned char arena[1024];   // object at 0, own node at 128, cell nodes at 256, 320, 384
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" void* norrath_host_pointer(unsigned address) {
    const unsigned long base = reinterpret_cast<unsigned long>(arena);
    return reinterpret_cast<void*>((base & ~0xffffffffUL) | address);
}
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
static unsigned word(const void* base, unsigned offset) {
    unsigned v;
    std::memcpy(&v, static_cast<const unsigned char*>(base) + offset, 4);
    return v;
}
static void set_word(void* base, unsigned offset, unsigned v) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 4);
}
static void set_pos(float x, float y) {
    std::memcpy(arena + 16, &x, 4);
    std::memcpy(arena + 20, &y, 4);
}
static unsigned char* cell_head(unsigned char* grid, int gx, int gy) { return grid + (gy * 120 + gx) * 4; }

// Put the object's cell node (arena+256) alone at the front of a head word.
static void seat(unsigned char* head) {
    std::memset(arena, 0, sizeof arena);
    set_word(arena, 8, low(arena + 128));          // object+8: own node
    set_word(arena + 128, 8, low(arena + 256));    // own node +8: cell node
    set_word(arena + 256, 4, low(head));           // cell node previous: head word
    set_word(head, 0, low(arena + 256));
}

int main() {
    unsigned char* old_head = cell_head(objectGrid, 60, 60);
    seat(old_head);
    set_word(arena + 256, 0, low(arena + 320));    // a follower after the cell node
    set_word(arena + 320, 4, low(arena + 256));
    // Move to cell (62, 61): x = 2 * 256, y = 1 * 256.
    set_pos(512.0f, 256.0f);
    objectUpdateInGrid__FP10GameObject(arena);
    expect(word(old_head, 0) == low(arena + 320), "old head takes the follower");
    expect(word(arena + 320, 4) == low(old_head), "follower points back at the old head");
    unsigned char* head = cell_head(objectGrid, 62, 61);
    expect(word(head, 0) == low(arena + 256), "new head takes the cell node");
    expect(word(arena + 256, 4) == low(head), "cell node previous is the new head");
    expect(word(arena + 256, 0) == 0u, "no old first in the new cell");

    // A second node already in the new cell is pushed back and linked.
    seat(old_head);
    set_word(old_head, 0, low(arena + 256));
    set_word(arena + 384, 4, low(cell_head(objectGrid, 60, 61)));
    set_word(cell_head(objectGrid, 60, 61), 0, low(arena + 384));
    set_pos(-512.0f, 256.0f);   // cell (58, 61)
    set_word(cell_head(objectGrid, 58, 61), 0, low(arena + 384));
    set_word(arena + 384, 4, low(cell_head(objectGrid, 58, 61)));
    set_word(cell_head(objectGrid, 60, 61), 0, 0u);
    objectUpdateInGrid__FP10GameObject(arena);
    head = cell_head(objectGrid, 58, 61);
    expect(word(head, 0) == low(arena + 256), "pushed in front of the existing node");
    expect(word(arena + 256, 0) == low(arena + 384), "next is the old first");
    expect(word(arena + 384, 4) == low(arena + 256), "old first points back at the node");

    // Bit 0x800 of the flags selects the ghost grid.
    seat(old_head);
    set_word(arena, 12, 0x800u);
    set_pos(0.0f, 0.0f);
    objectUpdateInGrid__FP10GameObject(arena);
    expect(word(cell_head(ghostObjectGrid, 60, 60), 0) == low(arena + 256), "ghost grid used when bit 0x800 is set");
    expect(word(cell_head(objectGrid, 60, 60), 0) == 0u, "normal grid cell left empty");

    // Without the bit, and with bit 0x2 set, the normal grid is still used.
    seat(old_head);
    set_word(arena, 12, 0x2u);
    objectUpdateInGrid__FP10GameObject(arena);
    expect(word(cell_head(objectGrid, 60, 60), 0) == low(arena + 256), "flag bit 2 is not consulted");

    // Edges: -15360 is cell 0; 119 is the last cell; outside goes to offTheGrid.
    seat(old_head);
    set_pos(-15360.9f, -15360.9f);   // truncates to -15360
    objectUpdateInGrid__FP10GameObject(arena);
    expect(word(cell_head(objectGrid, 0, 0), 0) == low(arena + 256), "truncated -15360 is cell 0");
    seat(cell_head(objectGrid, 0, 0));
    set_pos(-15361.0f, 0.0f);
    objectUpdateInGrid__FP10GameObject(arena);
    expect(word(offTheGrid, 0) == low(arena + 256), "x below the grid goes to offTheGrid");
    expect(word(arena + 256, 4) == low(offTheGrid), "off-grid previous is the head word");
    seat(offTheGrid);
    set_pos(0.0f, 15360.0f);   // gy = 120
    objectUpdateInGrid__FP10GameObject(arena);
    expect(word(offTheGrid, 0) == low(arena + 256), "y cell 120 goes to offTheGrid");
    seat(offTheGrid);
    set_pos(15359.0f, 15359.0f);
    objectUpdateInGrid__FP10GameObject(arena);
    expect(word(cell_head(objectGrid, 119, 119), 0) == low(arena + 256), "last cell");
    if (failures != 0) return 1;
    std::printf("update-in-grid contract passed\n");
    return 0;
}
