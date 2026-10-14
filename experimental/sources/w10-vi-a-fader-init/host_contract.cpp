#include "init.cpp"

#include <cstring>

static int log_text[16];
static int log_count;
static int create_result;
static int create_count_arg;
static void* material_object = reinterpret_cast<void*>(0x7700);
static int material_index_arg;

extern "C" int CreateMaterial__8VIRasteri(void* raster, int count) {
    (void)raster;
    create_count_arg = count;
    log_text[log_count++] = 1;
    return create_result;
}
extern "C" void* Material__8VIRasteri(void* raster, int index) {
    (void)raster;
    material_index_arg = index;
    log_text[log_count++] = 2;
    return material_object;
}
extern "C" void InitSolidFill__10VIMaterial(void* material) {
    if (material != material_object) log_text[log_count++] = 99;
    log_text[log_count++] = 3;
}
static int layer_arg = -1, mode_arg = -1;
extern "C" void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void* material, int layer, int mode) {
    if (material != material_object) log_text[log_count++] = 99;
    layer_arg = layer;
    mode_arg = mode;
    log_text[log_count++] = 4;
}
extern "C" long long GetTime__7VITimer() {
    log_text[log_count++] = 5;
    return 0x0000000400000009ll;
}

int main() {
    unsigned char object[40];
    std::memset(object, 0xa5, sizeof object);
    int raster_dummy;
    create_result = -5;
    int result = Init__7VIFaderP8VIRasterP12VIDictionaryf(object, &raster_dummy, nullptr, 0.5f);
    if (result != -1 || log_count != 1 || create_count_arg != 1) return 1;
    int stored_index;
    std::memcpy(&stored_index, object + 20, 4);
    float level;
    std::memcpy(&level, object + 8, 4);
    if (stored_index != -5 || level != 0.5f) return 2;
    unsigned word12, word16, word4;
    std::memcpy(&word12, object + 12, 4);
    std::memcpy(&word16, object + 16, 4);
    std::memcpy(&word4, object + 4, 4);
    if (word12 != 0 || word16 != 0 || word4 != 0xa5a5a5a5u) return 3;  // offset 4 untouched
    if (object[24] != 0xa5) return 4;  // timer not read on failure

    log_count = 0;
    create_result = 3;
    result = Init__7VIFaderP8VIRasterP12VIDictionaryf(object, &raster_dummy, nullptr, 2.0f);
    if (result != 0) return 5;
    const int expected[] = {1, 2, 3, 4, 5};
    if (log_count != 5) return 6;
    for (int i = 0; i < 5; ++i)
        if (log_text[i] != expected[i]) return 7;
    if (material_index_arg != 3 || layer_arg != 0 || mode_arg != 3) return 8;
    long long time;
    std::memcpy(&time, object + 24, 8);
    std::memcpy(&stored_index, object + 20, 4);
    if (time != 0x0000000400000009ll || stored_index != 3) return 9;
    std::memcpy(&level, object + 8, 4);
    if (level != 2.0f) return 10;
    return 0;
}
