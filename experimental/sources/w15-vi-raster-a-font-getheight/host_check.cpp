// Host check only; not a build input.
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "getheight.cpp"
#include <cstdio>
#include <cstring>

const char _6VIFont$sm_FontName[8] = "ArialUI";
static int hash_calls, find_calls, point_calls, find_result, g_raster, g_dict, seen_font;
extern "C" {
unsigned VIHashResourceID(const char* name) { ++hash_calls; return std::strcmp(name, "ArialUI") == 0 ? 0x55u : 0u; }
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* d, unsigned id, int type, int* out) {
    ++find_calls; *out = 9; return find_result && d == &g_dict && id == 0x55u && type == 12;
}
int GetPointSize__8VIRasteri(void* raster, int font) { ++point_calls; seen_font = font; return raster == &g_raster ? 77 : -1; }
}

int main() {
    bool ok = true;
    for (int null_dict = 0; null_dict < 2; ++null_dict) {
        for (int found = 0; found < 2; ++found) {
            hash_calls = find_calls = point_calls = 0;
            find_result = found;
            const int r = GetHeight__6VIFontP8VIRasterP12VIDictionary(nullptr, &g_raster, null_dict ? nullptr : &g_dict);
            const int used = !null_dict && found;
            ok = ok && r == (used ? 77 : 0) && hash_calls == !null_dict && find_calls == !null_dict && point_calls == used;
            if (used) ok = ok && seen_font == 9;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
