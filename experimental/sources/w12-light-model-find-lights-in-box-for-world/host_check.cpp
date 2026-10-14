// Host contract for findLightsInBox_forWorld__FG3BoxPP5Lighti. Not an EE plan input.
// Build with -DNORRATH_HOST_ARENA. Private address map: records at 0x100000 (arena), grid at 0x200000.
#include <cstring>
#include "../level-anim-load-common/host_check.hpp"
static unsigned char arena[0x4000];
extern "C" {
unsigned char lightGrid[90 * 90 * 4];
int interfaceMode;
unsigned char* norrath_host_pointer(unsigned a) { return a >= 0x200000u ? lightGrid + (a - 0x200000u) : arena + (a - 0x100000u); }
unsigned norrath_host_address(const unsigned char* p) {
    if (p >= lightGrid && p < lightGrid + sizeof lightGrid) return 0x200000u + unsigned(p - lightGrid);
    return 0x100000u + unsigned(p - arena);
}
void findLightsInBox_forWorld__FG3BoxPP5Lighti(const float* box, unsigned char* out, int max);
}
static unsigned A(const unsigned char* p) { return norrath_host_address(p); }
static unsigned char* light(int k, float x, float y, float z, float r, int mode) {
    unsigned char* l = arena + 0x1000 + 0x100 * k;
    std::memset(l, 0, 0x100);
    sf_(l + 48, x); sf_(l + 52, y); sf_(l + 56, z); sf_(l + 36, r); l[8] = (unsigned char)mode;
    return l;
}
static void put(unsigned char* l, int col, int row) {
    unsigned char* cell = lightGrid + (row * 90 + col) * 4;
    sw_(l, hw(cell)); sw_(cell, A(l));
}
int main() {
    unsigned char* out = arena + 0x100;
    // box from (-50,-50,-50) to (50,50,50): columns (11170>>8)=43 .. (11870>>8)=46, same rows
    const float box[6] = {-50, -50, -50, 50, 50, 50};
    interfaceMode = 2;
    unsigned char* a = light(0, 0, 0, 0, 10, 2);
    unsigned char* b = light(1, 100, 0, 0, 50, 2);      // touches the high x face exactly
    unsigned char* c = light(2, 100, 0, 0, 49, 2);      // 1 short on x
    unsigned char* d = light(3, 0, 0, 0, 0, 2);         // radius 0 is skipped
    unsigned char* e = light(4, 0, 0, 0, 10, 1);        // wrong mode byte
    unsigned char* f = light(5, 0, 0, -70, 20, 2);      // touches the low z face exactly
    unsigned char* g = light(6, 0, 70, 0, 19, 2);       // 1 short on y
    put(a, 45, 45); put(b, 43, 44); put(c, 46, 46); put(d, 45, 45); put(e, 45, 45); put(f, 46, 43); put(g, 44, 44);
    std::memset(out, 0xee, 64);
    findLightsInBox_forWorld__FG3BoxPP5Lighti(box, out, 8);
    // rows low to high, then columns low to high: row 43 (f), row 44 (b, g skipped), row 45 (a), row 46 (c skipped)
    if (hw(out) != A(f) || hw(out + 4) != A(b) || hw(out + 8) != A(a) || hw(out + 12) != 0) {
        for (int i = 0; i < 5; ++i) std::printf("%08x ", hw(out + 4 * i));
        return 1;
    }
    findLightsInBox_forWorld__FG3BoxPP5Lighti(box, out, 3);
    if (hw(out) != A(f) || hw(out + 4) != A(b) || hw(out + 8) != 0) return 2;
    findLightsInBox_forWorld__FG3BoxPP5Lighti(box, out, 1);
    if (hw(out) != 0) return 3;
    // A cell outside the range is not scanned.
    unsigned char* h = light(7, 0, 0, 0, 10, 2);
    put(h, 45, 47);
    findLightsInBox_forWorld__FG3BoxPP5Lighti(box, out, 8);
    if (hw(out) != A(f) || hw(out + 4) != A(b) || hw(out + 8) != A(a) || hw(out + 12) != 0) return 4;
    // An empty row range (low box above high box on y) visits nothing and still terminates the list.
    const float inverted[6] = {-50, 5000, -50, 50, -5000, 50};
    std::memset(out, 0xee, 64);
    findLightsInBox_forWorld__FG3BoxPP5Lighti(inverted, out, 8);
    if (hw(out) != 0) return 5;
    // A box far outside the world clamps to the edge cells.
    const float wide[6] = {-100000, -100000, -50, 100000, 100000, 50};
    unsigned char* i0 = light(8, 0, 0, 0, 10, 2);
    put(i0, 0, 0);
    findLightsInBox_forWorld__FG3BoxPP5Lighti(wide, out, 8);
    bool found = false;
    for (int i = 0; i < 8 && hw(out + 4 * i); ++i) found = found || hw(out + 4 * i) == A(i0);
    if (!found) return 6;
    std::printf("findLightsInBox_forWorld contract passed\n");
    return 0;
}
