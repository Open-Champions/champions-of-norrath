// Host check (not a plan input). Build:
//   c++ -O1 -c -Dstrlen=w16_strlen -D_malloc_r=w16_malloc_r -Dmemcpy=w16_memcpy -D_strdup_r=w16_strdup_r -o /tmp/w16_sdr_src.o strdup_r.cpp
//   c++ -O1 -o /tmp/w16_strdup_r host_check.cpp /tmp/w16_sdr_src.o && /tmp/w16_strdup_r
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" char* w16_strdup_r(void* reent, const char* text);
static void* malloc_reent;
static unsigned malloc_bytes;
static bool fail;
static int memcpy_calls;
static std::vector<char> pool(256);
extern "C" unsigned w16_strlen(const char* text) { return static_cast<unsigned>(std::strlen(text)); }
extern "C" void* w16_malloc_r(void* reent, unsigned bytes) {
    malloc_reent = reent;
    malloc_bytes = bytes;
    return fail ? nullptr : pool.data();
}
extern "C" void* w16_memcpy(void* destination, const void* source, unsigned count) {
    ++memcpy_calls;
    std::memcpy(destination, source, count);
    return nullptr;
}

int main() {
    int token;
    const char* inputs[] = {"", "a", "hello world"};
    for (const char* input : inputs) {
        std::memset(pool.data(), 0x55, pool.size());
        memcpy_calls = 0;
        fail = false;
        char* result = w16_strdup_r(&token, input);
        if (result != pool.data() || malloc_reent != &token) return 1;
        if (malloc_bytes != std::strlen(input) + 1 || memcpy_calls != 1) return 2;
        if (std::memcmp(result, input, malloc_bytes) != 0) return 3;
        if (pool[malloc_bytes] != 0x55) return 4;
    }
    fail = true;
    memcpy_calls = 0;
    if (w16_strdup_r(&token, "xyz") != nullptr || memcpy_calls != 0 || malloc_bytes != 4) return 5;
    std::printf("strdup_r checks passed\n");
    return 0;
}
