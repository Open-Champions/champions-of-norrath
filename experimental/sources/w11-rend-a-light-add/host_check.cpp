#include <cstring>
#include "../level-anim-load-common/host_check.hpp"
// Target addresses: lights in a private arena at 0x100000, the grid at 0x200000 and the
// overflow head word at 0x300000, so that a head-word address stored in a record maps back.
static unsigned char arena[0x1000];
extern "C" {
unsigned char lightGrid[90 * 90 * 4];
unsigned char offTheGrid[8];
unsigned char* norrath_host_pointer(unsigned address) {
    if (address >= 0x300000u) return offTheGrid + (address - 0x300000u);
    if (address >= 0x200000u) return lightGrid + (address - 0x200000u);
    return arena + (address - 0x100000u);
}
unsigned norrath_host_address(const unsigned char* p) {
    if (p >= offTheGrid && p < offTheGrid + 8) return 0x300000u + unsigned(p - offTheGrid);
    if (p >= lightGrid && p < lightGrid + sizeof lightGrid) return 0x200000u + unsigned(p - lightGrid);
    return 0x100000u + unsigned(p - arena);
}
void lightAdd__FP5Light(unsigned char* light);
}
static unsigned A(const unsigned char* p) { return norrath_host_address(p); }
static void place(unsigned char* l, float x, float y) {
    std::memset(l, 0, 64); sf_(l + 48, x); sf_(l + 52, y);
}
int main() {
    unsigned char* a = arena + 0x100;
    unsigned char* b = arena + 0x200;
    unsigned char* c = arena + 0x300;
    // gx = (int(x) + 11520) >> 8: x = 0 gives 45; y = 256 gives 46. Cell word 46 * 90 + 45.
    unsigned char* cell = lightGrid + (46 * 90 + 45) * 4;
    place(a, 0.0f, 256.0f);
    lightAdd__FP5Light(a);
    if (hw(cell) != A(a) || hw(a) != 0 || hw(a + 4) != A(cell)) return 1;
    // A second light in the same cell goes in front; the first one links back to it.
    place(b, 10.5f, 300.9f);
    lightAdd__FP5Light(b);
    if (hw(cell) != A(b) || hw(b) != A(a) || hw(a + 4) != A(b) || hw(b + 4) != A(cell)) return 2;
    // Moving the front light (x -11520.5 truncates to -11520, so gx 0, gy 45) unlinks it first.
    sf_(b + 48, -11520.5f); sf_(b + 52, 0.0f);
    lightAdd__FP5Light(b);
    unsigned char* cell2 = lightGrid + (45 * 90 + 0) * 4;
    if (hw(cell) != A(a) || hw(a + 4) != A(cell) || hw(cell2) != A(b) || hw(b) != 0) return 3;
    // Unlinking from the middle of a list: c goes in front of a, then a moves away.
    place(c, 0.0f, 256.0f);
    lightAdd__FP5Light(c);
    if (hw(cell) != A(c) || hw(c) != A(a) || hw(a + 4) != A(c)) return 4;
    sf_(a + 48, 90.0f * 256 - 11520.0f);   // gx 90: off the grid
    lightAdd__FP5Light(a);
    if (hw(c) != 0 || hw(offTheGrid) != A(a) || hw(a + 4) != A(offTheGrid) || hw(a) != 0) return 5;
    // Negative gy also goes off the grid, in front of the earlier light.
    place(b, 0.0f, -11521.0f);
    lightAdd__FP5Light(b);
    if (hw(offTheGrid) != A(b) || hw(b) != A(a) || hw(a + 4) != A(b)) return 6;
    return 0;
}
