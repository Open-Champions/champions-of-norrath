#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" unsigned long long norrath_mallinfo_host_address_base = 0;
extern "C" {
unsigned char _impure_ptr[4];
}
extern "C" void* mallinfo(void* result);

namespace {
unsigned char arena[64];
int calls = 0;
void* seen_result = nullptr;
void* seen_reent = nullptr;

unsigned low_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int fail(int code) {
    std::fprintf(stderr, "mallinfo check failed: %d\n", code);
    return code;
}
}

extern "C" void* _mallinfo_r(void* result, void* reent) {
    ++calls;
    seen_result = result;
    seen_reent = reent;
    return nullptr;
}

int main() {
    norrath_mallinfo_host_address_base =
        reinterpret_cast<std::uintptr_t>(arena) & ~0xffffffffull;
    unsigned char* reent = arena;
    unsigned char* result = arena + 16;
    write_word(_impure_ptr, low_of(reent));
    void* returned = mallinfo(result);
    if (calls != 1 || seen_result != result || seen_reent != reent) return fail(1);
    if (returned != result) return fail(2);
    std::printf("mallinfo checks passed\n");
    return 0;
}
