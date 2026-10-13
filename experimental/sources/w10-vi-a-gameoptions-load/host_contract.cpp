#define memset norrath_host_memset
#include "load.cpp"
#undef memset

#include <cstring>

static int order[8];
static int count;
static int read_result;
static int defaults_calls;
static int fill_word0, fill_word255;
static unsigned memset_count;
static const char* name_seen;
static int size_seen;

extern "C" void* norrath_host_memset(void* destination, int value, unsigned size) {
    std::memset(destination, value, size);
    memset_count = size;
    order[count++] = 1;
    return destination;
}
extern "C" int VIMemCardReadFile__FPCcPvi(const char* name, void* buffer, int size) {
    name_seen = name;
    size_seen = size;
    int* words = static_cast<int*>(buffer);
    words[0] = fill_word0;
    words[255] = fill_word255;
    order[count++] = 2;
    return read_result;
}
extern "C" void SetDefaults__13VIGameOptions(void*) {
    ++defaults_calls;
    order[count++] = 3;
}

static int run(int result, int w0, int w255, int* defaults) {
    count = 0;
    defaults_calls = 0;
    read_result = result;
    fill_word0 = w0;
    fill_word255 = w255;
    int object[256];
    for (int& word : object) word = 0x77;
    int value = Load__13VIGameOptions(object);
    *defaults = defaults_calls;
    return value;
}

int main() {
    int defaults;
    int value = run(-5, 0, 0x01234567, &defaults);
    if (value != -5 || defaults != 1 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 1;
    if (memset_count != 1024 || size_seen != 1024 || std::strcmp(name_seen, "game.opt") != 0) return 2;
    value = run(0, 0, 0x01234567, &defaults);
    if (value != -1 || defaults != 1) return 3;
    value = run(1023, 0, 0x01234567, &defaults);
    if (value != -1 || defaults != 1) return 4;
    value = run(1024, 0, 0x01234567, &defaults);
    if (value != 0 || defaults != 0 || count != 2) return 5;
    value = run(1024, 1, 0x01234567, &defaults);
    if (value != 0 || defaults != 1) return 6;
    value = run(1024, 0, 0x01234568, &defaults);
    if (value != 0 || defaults != 1) return 7;
    value = run(2000, 0, 0x01234567, &defaults);
    if (value != 0 || defaults != 0) return 8;
    return 0;
}
