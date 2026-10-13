#include "splash_fx_init.cpp"

#include <cstring>

static int hash_calls;
static const char* hash_name_seen;
static unsigned hash_result = 0xcafe1234u;
static int find_calls;
static VIDictionary* find_dictionary;
static unsigned find_id;
static int* find_out;
static int find_result;
static int find_value;
static int reset_calls;
static int bird_at_reset;

extern "C" unsigned VIHashResourceID(const char* name) {
    ++hash_calls;
    hash_name_seen = name;
    return hash_result;
}

extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(VIDictionary* dictionary, unsigned id,
                                                        VIResourceType* type, int* out) {
    ++find_calls;
    find_dictionary = dictionary;
    find_id = id;
    find_out = out;
    if (type == 0) return 99;
    if (*out != -1) return 98;  // already -1 when Find runs
    *out = find_value;
    return find_result;
}

extern "C" void VISplashFxReset__Fv() {
    ++reset_calls;
    bird_at_reset = norrath_splash_bird_tex;
}

int main() {
    VIRaster* raster = reinterpret_cast<VIRaster*>(0x1000);
    VIDictionary* dictionary = reinterpret_cast<VIDictionary*>(0x2000);

    norrath_splash_bird_tex = 5;
    find_result = 1;
    find_value = 42;
    if (VISplashFxInit__FP8VIRasterP12VIDictionary(raster, dictionary) != 0) return 1;
    if (hash_calls != 1 || std::strcmp(hash_name_seen, "Bird_Animation.tga") != 0) return 2;
    if (find_calls != 1 || find_dictionary != dictionary || find_id != 0xcafe1234u ||
        find_out != &norrath_splash_bird_tex)
        return 3;
    if (norrath_splash_raster != raster || norrath_splash_dictionary != dictionary) return 4;
    if (reset_calls != 1 || bird_at_reset != 42 || norrath_splash_bird_tex != 42) return 5;

    find_result = 0;
    find_value = 7;
    if (VISplashFxInit__FP8VIRasterP12VIDictionary(0, 0) != -1) return 6;
    if (reset_calls != 1) return 7;
    if (norrath_splash_raster != 0 || norrath_splash_dictionary != 0) return 8;
    return 0;
}
