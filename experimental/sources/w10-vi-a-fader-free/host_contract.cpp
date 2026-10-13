#include "free.cpp"

static void* seen_raster;
static int seen_material;
static void* seen_dictionary;
static int calls;

extern "C" void ReleaseMaterial__8VIRasteriR12VIDictionary(void* raster, int material, void* dictionary) {
    seen_raster = raster;
    seen_material = material;
    seen_dictionary = dictionary;
    ++calls;
}

int main() {
    // Pointer words are 32-bit on the target; fixtures use host-sized storage
    // only for the calls, so the pointer words are kept to values < 2^32.
    int object[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    // No raster: no release, state cleared.
    object[0] = 0; object[5] = 7;
    Free__7VIFader(object);
    if (calls != 0 || object[0] != 0 || object[5] != -1) return 1;

    // Raster set, negative material: no release, still cleared.
    object[0] = 0x1000; object[1] = 0x2000; object[5] = -3;
    Free__7VIFader(object);
    if (calls != 0 || object[0] != 0 || object[5] != -1) return 2;

    // Raster set, material >= 0: one release with the three stored values.
    object[0] = 0x1000; object[1] = 0x2000; object[5] = 0;
    Free__7VIFader(object);
    if (calls != 1 || seen_raster != reinterpret_cast<void*>(0x1000) || seen_material != 0 ||
        seen_dictionary != reinterpret_cast<void*>(0x2000) || object[0] != 0 || object[5] != -1) return 3;

    object[0] = 0x1000; object[5] = 41;
    Free__7VIFader(object);
    if (calls != 2 || seen_material != 41) return 4;
    // Other words untouched.
    if (object[1] != 0x2000) return 5;
    return 0;
}
