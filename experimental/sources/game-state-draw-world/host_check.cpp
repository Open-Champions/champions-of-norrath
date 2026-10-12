// Host contract for gameDrawWorld. Not an EE plan input.
#include "draw_world.cpp"

#include <cstdio>
#include <cstring>

extern "C" int gameNoDraw = 0;
extern "C" unsigned char coreLoadingScreenRunning = 0;
extern "C" unsigned char hudNoDraw = 0;
extern "C" unsigned char* world = 0;

static int failures = 0;
static char order[16];
static int order_used = 0;
static unsigned char* drawn_world = 0;
static unsigned char world_bytes[4];
static int loading_after_world = -1;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static void mark(char step) {
    if (order_used < 15) order[order_used++] = step;
    order[order_used] = 0;
}

static void reset_order() {
    order_used = 0;
    order[0] = 0;
    drawn_world = 0;
}

extern "C" void worldSetViewCenter__Fv() { mark('V'); }
extern "C" void drawAutoMap__Fv() {
    mark('A');
    world = 0;
}
extern "C" void worldDrawWorld__FP12_worldHeader(unsigned char* header) {
    mark('W');
    drawn_world = header;
    coreLoadingScreenRunning = static_cast<unsigned char>(loading_after_world);
}
extern "C" void drawDome__Fv() { mark('D'); }
extern "C" void gameLineDebugDraw__Fv() { mark('L'); }

int main() {
    gameNoDraw = 1;
    world = world_bytes;
    reset_order();
    gameDrawWorld__Fv();
    expect(order[0] == 0, "gameNoDraw skips every call");

    gameNoDraw = -1;
    reset_order();
    gameDrawWorld__Fv();
    expect(order[0] == 0, "a negative gameNoDraw word also skips");

    gameNoDraw = 0;
    coreLoadingScreenRunning = 0;
    hudNoDraw = 0;
    world = world_bytes;
    loading_after_world = 0;
    reset_order();
    gameDrawWorld__Fv();
    expect(std::strcmp(order, "VAWDL") == 0, "clear state draws the map, world, dome, and lines");
    expect(drawn_world == 0, "worldDrawWorld sees the pointer after the map");

    world = world_bytes;
    coreLoadingScreenRunning = 1;
    hudNoDraw = 0;
    loading_after_world = 0;
    reset_order();
    gameDrawWorld__Fv();
    expect(std::strcmp(order, "VWDL") == 0, "a loading byte skips the map and is read again for the dome");
    expect(drawn_world == world_bytes, "worldDrawWorld still receives world");

    world = world_bytes;
    coreLoadingScreenRunning = 0;
    hudNoDraw = 1;
    loading_after_world = 0;
    reset_order();
    gameDrawWorld__Fv();
    expect(std::strcmp(order, "VWDL") == 0, "hudNoDraw skips only the map");

    world = 0;
    hudNoDraw = 0;
    coreLoadingScreenRunning = 0;
    loading_after_world = 0;
    reset_order();
    gameDrawWorld__Fv();
    expect(std::strcmp(order, "VWDL") == 0 && drawn_world == 0, "a null world skips the map and is still passed");

    world = world_bytes;
    coreLoadingScreenRunning = 0;
    loading_after_world = 1;
    reset_order();
    gameDrawWorld__Fv();
    expect(std::strcmp(order, "VAWL") == 0, "a loading byte set by worldDrawWorld skips the dome");

    if (failures != 0) return 1;
    std::puts("gameDrawWorld contract passed");
    return 0;
}
