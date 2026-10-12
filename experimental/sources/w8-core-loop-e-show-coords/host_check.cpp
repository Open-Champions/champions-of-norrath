// Host check for showCoords__Fv. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#define sprintf norrath_host_sprintf
#include "show_coords.cpp"
#undef sprintf

extern "C" {
int validLocalPlayer = 0;
int nmPlayers = 2;
int megatileSeed = 0x1234abcd;
int megatileRot = 3;
void* smallFont = (void*)0x1;
unsigned char* world;
unsigned char player[23984 * 3];
unsigned char pad[128];
}
static unsigned char arena[4096];
extern "C" void* norrath_host_pointer(unsigned address) { return arena + address; }

static std::string trace;
extern "C" int norrath_host_sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f);
    const int n = std::vsnprintf(d, 256, f, ap);
    va_end(ap);
    return n;
}
extern "C" void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b) {
    trace += "S" + std::to_string(font == smallFont) + std::to_string(a) + ":" + std::to_string(color) + ":" + std::to_string(b) + ";";
}
extern "C" void fontPrintCentered__FiiPci(int x, int y, char* text, int flags) {
    trace += "P" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(flags) + "[" + text + "];";
}
extern "C" void fontEnd__Fv() { trace += "E;"; }
static Point3 mega_in;
extern "C" void getMegatileOriginalPosition__FG6Point3(Point3* result, const Point3* position) {
    trace += "M;";
    mega_in = *position;
    result->x = 1000.5f; result->y = -2000.5f; result->z = 9.0f;
}
static void w32(unsigned char* p, unsigned v) { for (int i = 0; i < 4; ++i) p[i] = (unsigned char)(v >> (8 * i)); }
static void wf(unsigned char* p, float f) { std::memcpy(p, &f, 4); }

int main() {
    unsigned char world_storage[256] = {0};
    world = world_storage;
    // Entity at arena offset 0x40; position (12.5, -3.5, 7).
    wf(arena + 0x40 + 16, 12.5f); wf(arena + 0x40 + 20, -3.5f); wf(arena + 0x40 + 24, 7.0f);
    w32(player + 23984 + 21872, 0x40);

    // Negative local player: nothing.
    validLocalPlayer = -1; showCoords__Fv();
    if (!trace.empty()) return 1;
    w32(player + 2 * 23984 + 21872, 0x40);   // a valid entity, but the slot is not below nmPlayers
    validLocalPlayer = 2; showCoords__Fv();
    if (!trace.empty()) return 2;
    w32(player + 2 * 23984 + 21872, 0);
    // Null entity word.
    validLocalPlayer = 0; showCoords__Fv();
    if (!trace.empty()) return 3;
    validLocalPlayer = 1;
    // Bit 2 of pad+96 suppresses the overlay.
    w32(pad + 96, 2); showCoords__Fv();
    if (!trace.empty()) return 4;
    w32(pad + 96, 0xfffffffd);
    // No megatile word: two lines. 12.5 rounds up to 13, -3.5 rounds away to -4.
    showCoords__Fv();
    if (trace != "S1" "7:2151694400:-1;P320,10,10000[x:13 y:-4];P320,26,10000[Build: Dec 30 2003,  18:58:39\n];E;") return 5;
    // Megatile word set: the original position is requested for the copied position.
    trace.clear(); w32(world + 112, 1);
    showCoords__Fv();
    if (trace != "S17:2151694400:-1;M;P320,10,10000[x:13 y:-4 -- Mega 1001, -2001];P320,26,10000[Build: Dec 30 2003,  18:58:39\n];"
                 "P320,40,10000[Megatile seed 1234abcd:3\n];E;") return 6;
    if (mega_in.x != 12.5f || mega_in.y != -3.5f || mega_in.z != 7.0f) return 7;
    // Zero rounds to zero, a small positive value rounds to zero, a small negative to zero.
    trace.clear(); w32(world + 112, 0);
    wf(arena + 0x40 + 16, 0.0f); wf(arena + 0x40 + 20, -0.25f);
    showCoords__Fv();
    if (trace.find("[x:0 y:0]") == std::string::npos) return 8;
    return 0;
}
