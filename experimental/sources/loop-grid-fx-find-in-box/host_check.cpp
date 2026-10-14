// Host contract for objectFindInBox__FG6Point3T0PP10GameObjecti. Not an EE plan input.
// Words that hold addresses point into one static arena.
#include <cstdio>
#include <cstring>

struct Point3 { float x, y, z; };
extern "C" unsigned char objectGrid[57600] = {0};
extern "C" unsigned char norrath_grid_ignore_flags_view[4] = {0};
extern "C" void** objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 minimum, Point3 maximum, void** output, int capacity);

static unsigned char arena[8192];   // nodes at 64*i, objects at 4096 + 64*i
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
static void set_word(void* base, unsigned offset, unsigned v) { std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 4); }
static void set_float(void* base, unsigned offset, float v) { std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 4); }
static unsigned char* node(int i) { return arena + 64 * i; }
static unsigned char* object(int i) { return arena + 4096 + 64 * i; }
static unsigned char* cell(int x, int y) { return objectGrid + (y * 120 + x) * 4; }

// Put object i alone in its own node, at the front of the cell.
static void place(int i, int x, int y, float px, float py, float pz, float radius, float height) {
    unsigned char* o = object(i);
    std::memset(o, 0, 64);
    set_float(o, 16, px); set_float(o, 20, py); set_float(o, 24, pz); set_float(o, 28, radius); set_float(o, 32, height);
    set_word(node(i), 12, low(o));
    set_word(node(i), 0, 0u);
    unsigned head;
    std::memcpy(&head, cell(x, y), 4);
    set_word(node(i), 0, head);
    set_word(cell(x, y), 0, low(node(i)));
}
static unsigned out[16];
static void** run(Point3 a, Point3 b, int capacity) {
    std::memset(out, 0x5a, sizeof out);
    return objectFindInBox__FG6Point3T0PP10GameObjecti(a, b, reinterpret_cast<void**>(out), capacity);
}
static unsigned obj_word(int i) { return low(object(i)); }

int main() {
    // Cell (60, 60) is the cell containing 0..255 on each axis.
    place(1, 60, 60, 10.0f, 10.0f, 0.0f, 1.0f, 2.0f);
    place(2, 60, 60, 100.0f, 100.0f, 0.0f, 1.0f, 2.0f);
    place(3, 61, 60, 300.0f, 10.0f, 0.0f, 1.0f, 2.0f);
    void** end = run({0, 0, -5}, {255, 255, 5}, 8);
    expect(out[0] == obj_word(2) && out[1] == obj_word(1), "the cell list is walked front to back");
    expect(out[2] == 0u && end == reinterpret_cast<void**>(out + 2), "terminator and its address returned");
    // A wider box reaches the neighbouring cell, scanned after cell (60, 60).
    end = run({0, 0, -5}, {400, 255, 5}, 8);
    expect(out[0] == obj_word(2) && out[1] == obj_word(1) && out[2] == obj_word(3) && out[3] == 0u, "cells run low to high in x");
    // Rows run low to high.
    place(4, 60, 61, 10.0f, 300.0f, 0.0f, 1.0f, 2.0f);
    end = run({0, 0, -5}, {255, 400, 5}, 8);
    expect(out[2] == obj_word(4) && out[3] == 0u, "second row comes after the first");
    // Empty range.
    end = run({255, 0, 0}, {-255, 0, 0}, 8);
    expect(out[0] == 0u && end == reinterpret_cast<void**>(out), "empty box stores only the terminator");
    // Capacity: at most capacity - 1 objects.
    end = run({0, 0, -5}, {400, 400, 5}, 3);
    expect(out[0] == obj_word(2) && out[1] == obj_word(1) && out[2] == 0u, "capacity 3 keeps two");
    expect(end == reinterpret_cast<void**>(out + 2), "capacity stop returns the terminator address");

    // Box tests on the object (60, 60) id 1 at (10, 10, 0), radius 1, height 2.
    run({12.5f, 0, -5}, {255, 255, 5}, 8);
    expect(out[0] == obj_word(2) && out[1] == 0u, "x + r < min.x skips object 1");
    run({11.0f, 0, -5}, {255, 255, 5}, 8);
    expect(out[0] == obj_word(2) && out[1] == obj_word(1), "x + r == min.x does not skip");
    run({0, 0, -5}, {50.0f, 255, 5}, 8);
    expect(out[0] == obj_word(1) && out[1] == 0u, "max.x < x - r skips object 2");
    run({0, 11.5f, -5}, {255, 255, 5}, 8);
    expect(out[0] == obj_word(2) && out[1] == 0u, "y + r < min.y skips object 1");
    run({0, 0, -5}, {255, 50.0f, 5}, 8);
    expect(out[0] == obj_word(1) && out[1] == 0u, "max.y < y - r skips object 2");
    run({0, 0, 0.5f}, {255, 255, 5}, 8);
    expect(out[0] == obj_word(2) && out[1] == obj_word(1), "z + height >= min.z keeps both");
    run({0, 0, 2.5f}, {255, 255, 5}, 8);
    expect(out[0] == 0u, "z + height < min.z skips");
    run({0, 0, -5}, {255, 255, -0.5f}, 8);
    expect(out[0] == 0u, "max.z < z skips");

    // Ignore mask.
    set_word(object(2), 12, 0x8u);
    set_word(norrath_grid_ignore_flags_view, 0, 0x8u);
    run({0, 0, -5}, {255, 255, 5}, 8);
    expect(out[0] == obj_word(1) && out[1] == 0u, "an object sharing a bit with the mask is skipped");
    set_word(norrath_grid_ignore_flags_view, 0, 0x10u);
    run({0, 0, -5}, {255, 255, 5}, 8);
    expect(out[0] == obj_word(2) && out[1] == obj_word(1), "no shared bit means it is kept");

    // Null object word is skipped.
    set_word(node(1), 12, 0u);
    run({0, 0, -5}, {255, 255, 5}, 8);
    expect(out[0] == obj_word(2) && out[1] == 0u, "a node with no object is skipped");

    // Grid clamps: a very large box is capped to the 120 by 120 grid, a very negative one starts at cell zero.
    run({-100000, -100000, -5}, {100000, 100000, 5}, 8);
    expect(out[0] != 0x5a5a5a5au, "huge box does not leave the grid");
    if (failures != 0) return 1;
    std::printf("find-in-box contract passed\n");
    return 0;
}
