// Host check only; not a build input.
#include "getstringwidth.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>

const char _6VIFont$sm_FontName[8] = "ArialUI";
static int hash_calls, find_calls, inner_calls, find_result, found_value, hash_name_ok;
static unsigned find_id_seen;
static int type_seen;
static int g_raster, g_dict;
extern "C" {
unsigned VIHashResourceID(const char* name) { ++hash_calls; hash_name_ok = std::strcmp(name, "ArialUI") == 0; return 0xabcd1234u; }
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* d, unsigned id, int type, int* out) {
    ++find_calls; find_id_seen = id; type_seen = type; *out = found_value; return find_result && d == &g_dict;
}
int GetStringWidth__8VIRasteriPCUs(void* raster, int font, const unsigned short* text) { ++inner_calls; return (raster == &g_raster ? 1000 : 0) + font * 10 + text[0]; }
}

int main() {
    bool ok = true;
    const unsigned short text[2] = {7, 0};
    for (int null_dict = 0; null_dict < 2; ++null_dict) {
        for (int found : {0, 1}) {
            hash_calls = find_calls = inner_calls = 0;
            find_result = found; found_value = 4;
            const int width = GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(nullptr, &g_raster, null_dict ? nullptr : &g_dict, text);
            const int want = (!null_dict && found) ? 1000 + 40 + 7 : 0;
            ok = ok && width == want && hash_calls == !null_dict && find_calls == !null_dict && inner_calls == (!null_dict && found);
            if (!null_dict) ok = ok && hash_name_ok && find_id_seen == 0xabcd1234u && type_seen == 12;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
