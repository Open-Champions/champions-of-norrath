// Host check only; not a build input.
#include "drawcenteredstring.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>

const char _6VIFont$sm_FontName[8] = "ArialUI";
static int hash_calls, find_calls, draw_calls, find_result, g_raster, g_dict, g_self[4];
static int seen_x, seen_y, seen_font, seen_color_ok, seen_flags;
static const char* seen_text;
extern "C" {
unsigned VIHashResourceID(const char* name) { ++hash_calls; return std::strcmp(name, "ArialUI") == 0 ? 0x55u : 0u; }
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* d, unsigned id, int type, int* out) {
    ++find_calls; *out = 9; return find_result && d == &g_dict && id == 0x55u && type == 12;
}
int DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(void* raster, int font, const int* pos, const void* color,
                                                                 const char* text, unsigned flags) {
    ++draw_calls; seen_x = pos[0]; seen_y = pos[1]; seen_font = font; seen_color_ok = color == g_self && raster == &g_raster;
    seen_text = text; seen_flags = static_cast<int>(flags);
    return 77;
}
}

int main() {
    bool ok = true;
    const char* text = "hi";
    for (int null_dict = 0; null_dict < 2; ++null_dict) {
        for (int found : {0, 1}) {
            hash_calls = find_calls = draw_calls = 0;
            find_result = found;
            const int r = DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCciiUi(g_self, &g_raster, null_dict ? nullptr : &g_dict, text, -30, 45, 3u);
            const int drawn = !null_dict && found;
            ok = ok && r == 0 && hash_calls == !null_dict && find_calls == !null_dict && draw_calls == drawn;
            if (drawn) ok = ok && seen_x == -30 && seen_y == 45 && seen_font == 9 && seen_color_ok && seen_text == text && seen_flags == 3;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
