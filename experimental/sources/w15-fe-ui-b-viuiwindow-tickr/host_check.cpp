#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "tickr.cpp"
#include <string>
#include <vector>

static std::string trace;
static int blit_x, blit_y, blit_texture, blit_blend;
static unsigned blit_ui, blit_flags;
static float blit_alpha;
static int pulse_calls;
extern "C" int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, unsigned raster, unsigned dictionary) {
    trace += "H" + std::to_string(reinterpret_cast<unsigned char*>(font) - arena) + "," + std::to_string(raster) + "," + std::to_string(dictionary) + ";";
    return 17;
}
extern "C" float TickPulse__C10VIUIWindow(const void*) { ++pulse_calls; return 0.25f; }
extern "C" int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(unsigned ui, int texture, VIVect2I at, int blend, float alpha, unsigned flags) {
    blit_ui = ui; blit_texture = texture; blit_x = at.x; blit_y = at.y; blit_blend = blend; blit_alpha = alpha; blit_flags = flags;
    trace += "B;";
    return 1;
}

int main() {
    int window[16] = {};
    window[0] = static_cast<int>(arena_base + 256);
    const unsigned raster = 7, dictionary = 9;
    std::memcpy(arena + 256 + 72, &raster, 4);
    std::memcpy(arena + 256 + 80, &dictionary, 4);
    window[3] = 300; window[4] = 200; window[5] = 100; window[6] = 60; window[7] = 12;
    TickR__10VIUIWindowiib(window, 5, 3, true);
    // Q = (300 - 50 + 5 - 16, 200 - 30 + 12 + (17 - 4) * 3)
    if (trace != "H312,7,9;B;" || pulse_calls != 1) return 1;
    if (blit_ui != arena_base + 256 || blit_texture != 89 || blit_blend != 3 || blit_flags != 0u) return 2;
    if (blit_x != 239 || blit_y != 221 || blit_alpha != 0.25f) return 3;
    TickR__10VIUIWindowiib(window, -5, 0, false);
    if (pulse_calls != 1 || blit_alpha != 1.0f || blit_x != 229 || blit_y != 182) return 4;
    return 0;
}
