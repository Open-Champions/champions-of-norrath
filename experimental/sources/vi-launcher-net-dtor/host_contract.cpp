#define printf norrath_host_printf
#define memset norrath_host_memset
#include "net_dtor.cpp"
#undef printf
#undef memset

#include <cstring>

static const char* log_text[8];
static int log_count;
static void* freed;
static int free_calls;
static VINetworkStatus* terminated;
static void* deleted;
static int delete_calls;

static void note(const char* text) {
    if (log_count < 8) log_text[log_count] = text;
    ++log_count;
}

extern "C" int norrath_host_printf(const char* text, ...) {
    note(text);
    return 0;
}
extern "C" void* norrath_host_memset(void* destination, int value, unsigned count) {
    unsigned char* bytes = static_cast<unsigned char*>(destination);
    for (unsigned index = 0; index < count; ++index) bytes[index] = static_cast<unsigned char>(value);
    note("memset");
    return destination;
}
extern "C" void __wrap_free(void* pointer) {
    ++free_calls;
    freed = pointer;
    note("free");
}
extern "C" void sceLibnetTerminate(VINetworkStatus* object) {
    terminated = object;
    note("terminate");
}
extern "C" void __builtin_delete(void* pointer) {
    ++delete_calls;
    deleted = pointer;
}

int main() {
    unsigned char object[108];
    for (unsigned char& byte : object) byte = 0xa5;
    *reinterpret_cast<unsigned*>(object + 60) = 0;
    *reinterpret_cast<int*>(object + 104) = 9;
    auto* status = reinterpret_cast<VINetworkStatus*>(object);
    _$_15VINetworkStatus(status, 2);
    if (free_calls != 0 || terminated != nullptr || delete_calls != 0) return 1;
    for (unsigned char byte : object) {
        if (byte != 0) return 2;
    }

    for (unsigned char& byte : object) byte = 0xa5;
    *reinterpret_cast<unsigned*>(object + 60) = 0x70u;
    *reinterpret_cast<int*>(object + 104) = 4;
    log_count = 0;
    _$_15VINetworkStatus(status, 1);
    if (free_calls != 1 || freed != reinterpret_cast<void*>(0x70u)) return 3;
    if (terminated != status || delete_calls != 1 || deleted != object) return 4;
    if (log_count != 6) return 5;
    if (std::strcmp(log_text[1], "a1\n") != 0 || std::strcmp(log_text[2], "terminate") != 0) return 6;
    if (std::strcmp(log_text[3], "a2\n") != 0) return 7;
    for (unsigned index = 0; index < 108; ++index) {
        if (object[index] != 0) return 8;
    }
    return 0;
}
