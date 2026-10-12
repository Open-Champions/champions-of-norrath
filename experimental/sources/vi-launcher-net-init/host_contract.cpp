#define printf norrath_host_printf
#include "net_init.cpp"
#undef printf

#include <cstring>

static const char* log_text[8];
static int log_count;
static int init_result;
static int list_result = 2;
static void* allocation = reinterpret_cast<void*>(0x100);
static int free_calls;
static void* freed;
static int align_calls;
static unsigned seen_alignment;
static unsigned seen_size;
static VINetworkStatus* seen_object;
static int seen_first;
static int seen_second;
static int seen_third;
static void* seen_buffer;
static void* seen_list;
static int seen_count;

static void note(const char* text) {
    if (log_count < 8) log_text[log_count] = text;
    ++log_count;
}

extern "C" int norrath_host_printf(const char* text, ...) {
    note(text);
    return 0;
}
extern "C" int sceLibnetInitialize(VINetworkStatus* object, int first, int second, int third) {
    seen_object = object;
    seen_first = first;
    seen_second = second;
    seen_third = third;
    note("libnet");
    return init_result;
}
extern "C" void __wrap_free(void* pointer) {
    ++free_calls;
    freed = pointer;
    note("free");
}
extern "C" void* __wrap_memalign(unsigned alignment, unsigned size) {
    ++align_calls;
    seen_alignment = alignment;
    seen_size = size;
    note("memalign");
    return allocation;
}
extern "C" int sceInetGetInterfaceList(VINetworkStatus* object, void* buffer, void* list, int count) {
    if (object != seen_object) return -100;
    seen_buffer = buffer;
    seen_list = list;
    seen_count = count;
    note("list");
    return list_result;
}

static int expect(const char* text) {
    if (log_count < 1 || std::strcmp(log_text[log_count - 1], text) != 0) return 1;
    return 0;
}

int main() {
    unsigned char object[108];
    for (unsigned char& byte : object) byte = 0;
    auto* status = reinterpret_cast<VINetworkStatus*>(object);
    init_result = 3;
    if (Initialize__15VINetworkStatus(status) != -1) return 1;
    if (log_count != 2 || std::strcmp(log_text[0], "aa\n") != 0 || std::strcmp(log_text[1], "libnet") != 0)
        return 2;
    if (seen_first != 2048 || seen_second != 8192 || seen_third != 32 || seen_object != status) return 3;
    if (free_calls != 0 || align_calls != 0) return 4;

    log_count = 0;
    init_result = 0;
    *reinterpret_cast<unsigned*>(object + 60) = 0x55u;
    allocation = nullptr;
    if (Initialize__15VINetworkStatus(status) != -1) return 5;
    if (free_calls != 1 || freed != reinterpret_cast<void*>(0x55)) return 6;
    if (seen_alignment != 64 || seen_size != 2048) return 7;
    if (*reinterpret_cast<unsigned*>(object + 60) != 0) return 8;
    if (std::strcmp(log_text[0], "aa\n") != 0 || std::strcmp(log_text[4], "bb\n") != 0) return 9;
    if (log_count != 5) return 10;

    log_count = 0;
    free_calls = 0;
    allocation = reinterpret_cast<void*>(0x80);
    list_result = -3;
    *reinterpret_cast<unsigned*>(object + 60) = 0;
    if (Initialize__15VINetworkStatus(status) != -1) return 11;
    if (free_calls != 0) return 12;
    if (seen_buffer != allocation || seen_list != object + 64 || seen_count != 10) return 13;
    if (*reinterpret_cast<int*>(object + 104) != 0) return 14;
    if (std::strcmp(log_text[log_count - 1], "dd\n") != 0) return 15;

    log_count = 0;
    list_result = 2;
    if (Initialize__15VINetworkStatus(status) != 0) return 16;
    if (*reinterpret_cast<int*>(object + 104) != 2) return 17;
    if (*reinterpret_cast<unsigned*>(object + 60) != 0x80u) return 18;
    if (expect("dd\n") != 0) return 19;
    return 0;
}
