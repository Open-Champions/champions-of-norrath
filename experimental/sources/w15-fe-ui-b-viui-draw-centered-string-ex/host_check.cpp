#include "draw_centered_string_ex.cpp"
#include <cstring>
#include <string>
#include <vector>

struct Event { std::string kind; float c[4]; int x, y; unsigned flags, raster, dictionary; };
static std::vector<Event> events;
static const void* last_font;
extern "C" void SetTextColor__6VIFontRC10VIColor32F(void* font, const VIColor32F& color) {
    Event e{"color", {color.v[0], color.v[1], color.v[2], color.v[3]}, 0, 0, 0, 0, 0};
    events.push_back(e);
    last_font = font;
}
extern "C" void DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCUsiiUi(void* font, unsigned raster, unsigned dictionary,
                                                                              const unsigned short*, int x, int y, unsigned flags) {
    Event e{"draw", {0, 0, 0, 0}, x, y, flags, raster, dictionary};
    events.push_back(e);
    if (font != last_font) events.push_back({"badfont", {0, 0, 0, 0}, 0, 0, 0, 0, 0});
}

int main() {
    alignas(16) unsigned char ui[128] = {};
    const unsigned raster = 0x1111, dictionary = 0x2222;
    std::memcpy(ui + 72, &raster, 4);
    std::memcpy(ui + 80, &dictionary, 4);
    const float old_color[4] = {0.9f, 0.8f, 0.7f, 0.6f};
    std::memcpy(ui + 56, old_color, 16);
    VIColor32F wanted = {{0.1f, 0.2f, 0.3f, 0.4f}};
    const unsigned short text[2] = {'x', 0};
    if (DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii(reinterpret_cast<char*>(ui), text, 10, 20, wanted, 5u, 1) != 0) return 1;
    if (events.size() != 5) return 2;
    if (events[0].kind != "color" || events[0].c[0] != 0 || events[0].c[1] != 0 || events[0].c[2] != 0 || events[0].c[3] != 0.4f) return 3;
    if (events[1].kind != "draw" || events[1].x != 12 || events[1].y != 22 || events[1].flags != 5u ||
        events[1].raster != raster || events[1].dictionary != dictionary) return 4;
    if (events[2].kind != "color" || events[2].c[0] != 0.1f || events[2].c[3] != 0.4f) return 5;
    if (events[3].kind != "draw" || events[3].x != 10 || events[3].y != 20) return 6;
    if (events[4].kind != "color" || events[4].c[0] != 0.9f || events[4].c[3] != 0.6f) return 7;
    events.clear();
    if (DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii(reinterpret_cast<char*>(ui), text, -1, 2, wanted, 0u, 0) != 0) return 8;
    if (events.size() != 3 || events[0].kind != "color" || events[1].x != -1 || events[2].c[0] != 0.9f) return 9;
    return 0;
}
