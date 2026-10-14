#include <cstring>
#include "../level-anim-load-common/host_check.hpp"
// Private target address map: records at 0x100000 (arena), the grid view at 0x200000.
static unsigned char arena[0x4000];
extern "C" {
unsigned char lightGrid[90 * 90 * 4];
int interfaceMode;
unsigned char* norrath_host_pointer(unsigned a) { return a >= 0x200000u ? lightGrid + (a - 0x200000u) : arena + (a - 0x100000u); }
unsigned norrath_host_address(const unsigned char* p) {
    if (p >= lightGrid && p < lightGrid + sizeof lightGrid) return 0x200000u + unsigned(p - lightGrid);
    return 0x100000u + unsigned(p - arena);
}
void findLightsAtPoint__FG6Point3PP5Lighti(const float* point, unsigned char* out, int max);
}
static unsigned A(const unsigned char* p) { return norrath_host_address(p); }
static unsigned char* light(int k, float x, float y, float z, float r, int mode) {
    unsigned char* l = arena + 0x1000 + 0x100 * k;
    std::memset(l, 0, 0x100);
    sf_(l + 48, x); sf_(l + 52, y); sf_(l + 56, z); sf_(l + 28, r); l[8] = (unsigned char)mode;
    return l;
}
static void put(unsigned char* l, int col, int row) {
    unsigned char* cell = lightGrid + (row * 90 + col) * 4;
    sw_(l, hw(cell)); sw_(cell, A(l));
}
int main() {
    unsigned char* out = arena + 0x100;
    float p[3] = {0.0f, 0.0f, 0.0f};   // cell 45, 45; range columns (11220>>8)=43 .. (11820>>8)=46
    interfaceMode = 2;
    unsigned char* a = light(0, 100.0f, 0.0f, 0.0f, 150.0f, 2);
    unsigned char* b = light(1, 0.0f, 0.0f, 0.0f, 1.0f, 2);
    unsigned char* c = light(2, 0.0f, 400.0f, 0.0f, 150.0f, 2);   // out of reach on y
    unsigned char* d = light(3, 0.0f, 0.0f, 0.0f, 10.0f, 1);      // wrong mode byte
    unsigned char* e = light(4, 0.0f, 0.0f, 299.0f, 299.0f, 2);   // touches z exactly
    put(a, 45, 45); put(b, 43, 44); put(c, 46, 46); put(d, 45, 45); put(e, 46, 43);
    std::memset(out, 0xee, 64);
    findLightsAtPoint__FG6Point3PP5Lighti(p, out, 8);
    // Rows run low to high, columns low to high; within a cell the list order (newest first) holds.
    if (hw(out) != A(e) || hw(out + 4) != A(b) || hw(out + 8) != A(a) || hw(out + 12) != 0) {
        for (int i = 0; i < 4; ++i) std::printf("%08x ", hw(out + 4 * i));
        return 1;
    }
    // Capacity 3 leaves room for the terminator: two lights.
    findLightsAtPoint__FG6Point3PP5Lighti(p, out, 3);
    if (hw(out) != A(e) || hw(out + 4) != A(b) || hw(out + 8) != 0) return 2;
    findLightsAtPoint__FG6Point3PP5Lighti(p, out, 1);
    if (hw(out) != 0) return 3;
    // Lights that only touch (pos + radius == p) are kept; a cell outside the range is not scanned.
    unsigned char* f = light(5, 0.0f, 0.0f, -300.0f, 300.0f, 2);
    put(f, 45, 47);                // row 47 is above the range of 43 .. 46
    findLightsAtPoint__FG6Point3PP5Lighti(p, out, 8);
    if (hw(out + 12) != 0) return 4;
    unsigned char* g = light(6, 0.0f, 0.0f, -300.0f, 300.0f, 2);
    put(g, 44, 45);
    findLightsAtPoint__FG6Point3PP5Lighti(p, out, 8);
    if (hw(out + 8) != A(g) || hw(out + 12) != A(a)) return 5;
    // A point near the grid corner clamps the range to cell 0..; a far point visits nothing.
    float q[3] = {-11520.0f, -11520.0f, 0.0f};
    unsigned char* h = light(7, -11520.0f, -11520.0f, 0.0f, 5.0f, 2);
    put(h, 0, 0);
    findLightsAtPoint__FG6Point3PP5Lighti(q, out, 8);
    if (hw(out) != A(h) || hw(out + 4) != 0) return 6;
    float far[3] = {1e6f, 0.0f, 0.0f};
    findLightsAtPoint__FG6Point3PP5Lighti(far, out, 8);
    if (hw(out) != 0) return 7;
    return 0;
}
