#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>

extern "C" void* norrath_mem_realloc(void* block, int request);
extern "C" unsigned long long norrath_realloc_host_address_base = 0;

namespace {
unsigned char* arena;
unsigned failures;

int signed_word(unsigned value) {
    return value < 0x80000000u ? static_cast<int>(value) : -1 - static_cast<int>(~value);
}
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
unsigned low_address(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<uintptr_t>(pointer));
}
unsigned char* from_low(unsigned address) {
    return reinterpret_cast<unsigned char*>(norrath_realloc_host_address_base + address);
}

int alloc_calls;
int alloc_request;
int alloc_name_empty;
int free_calls;
int mutate_old_size;
unsigned char* free_pointer;
unsigned copy_count;
unsigned char* copy_destination;
const unsigned char* copy_source;
unsigned char* alloc_result;

void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void* norrath_mem_alloc(int requested_bytes, char* allocation_name) {
    ++alloc_calls;
    alloc_request = requested_bytes;
    alloc_name_empty = allocation_name != nullptr && allocation_name[0] == 0;
    if (mutate_old_size >= 0) write_word(arena + 256 + 4, static_cast<unsigned>(mutate_old_size));
    return alloc_result;
}
extern "C" void norrath_mem_free(void* allocation) {
    ++free_calls;
    free_pointer = static_cast<unsigned char*>(allocation);
}
extern "C" void* norrath_realloc_test_copy(void* destination, const void* source, unsigned count) {
    copy_count = count;
    copy_destination = static_cast<unsigned char*>(destination);
    copy_source = static_cast<const unsigned char*>(source);
    if (destination != nullptr && source != nullptr)
        std::memmove(destination, source, count);
    return nullptr;
}

unsigned char* place_block(unsigned payload_size, unsigned char fill) {
    unsigned char* header = arena + 256;
    unsigned char* payload = header + 28;
    unsigned char* successor = header + payload_size + 28;
    std::memset(arena, 0x5a, 4096);
    std::memset(payload, fill, payload_size);
    write_word(header + 4, payload_size);
    write_word(header + 12, low_address(successor));
    write_word(successor + 16, low_address(header));
    write_word(header + 8, 0x11111111u);
    return payload;
}

void reset_calls() {
    alloc_calls = 0;
    free_calls = 0;
    copy_count = 0;
    alloc_request = 0;
    alloc_name_empty = 0;
    mutate_old_size = -1;
    free_pointer = nullptr;
    copy_destination = nullptr;
    copy_source = nullptr;
}
}

int main() {
    arena = static_cast<unsigned char*>(std::malloc(4096));
    if (arena == nullptr) return 1;
    norrath_realloc_host_address_base =
        reinterpret_cast<uintptr_t>(arena) & ~0xffffffffull;
    expect(from_low(low_address(arena + 256)) == arena + 256, "address round trip");

    alloc_result = arena + 2048;
    std::memset(alloc_result, 0x22, 256);

    {
        reset_calls();
        unsigned char* payload = place_block(200, 0x41);
        void* result = norrath_mem_realloc(payload, 100);
        expect(result == payload, "slack 100 keeps the block");
        expect(alloc_calls == 0 && free_calls == 0 && copy_count == 0, "slack 100 makes no calls");
        expect(read_word(payload - 28 + 4) == 200u, "slack 100 leaves the size word");
    }
    {
        reset_calls();
        unsigned char* payload = place_block(192, 0x42);
        void* result = norrath_mem_realloc(payload, 64);
        expect(result == payload, "slack 128 keeps the block");
        expect(alloc_calls == 0 && free_calls == 0, "slack 128 makes no calls");
        expect(read_word(payload - 28 + 4) == 192u, "slack 128 leaves the size word");
    }
    {
        reset_calls();
        unsigned char* payload = place_block(193, 0x43);
        unsigned char* header = payload - 28;
        unsigned char* successor = header + 193 + 28;
        void* result = norrath_mem_realloc(payload, 64);
        unsigned char* fresh = header + 64 + 28;
        expect(result == payload, "slack 129 returns the original pointer");
        expect(alloc_calls == 0 && free_calls == 1, "slack 129 only frees the tail");
        expect(free_pointer == fresh + 28, "freed pointer is the tail payload");
        expect(read_word(header + 4) == 64u, "retained size is the aligned request");
        expect(read_word(fresh + 4) == 101u, "tail size is old minus aligned minus header");
        expect(read_word(fresh + 12) == low_address(successor), "tail keeps the old successor");
        expect(read_word(fresh + 16) == low_address(header), "tail previous is the retained header");
        expect(read_word(header + 12) == low_address(fresh), "retained block points at the tail");
        expect(read_word(successor + 16) == low_address(fresh), "successor previous becomes the tail");
        expect(fresh[0] == 0xcdu && fresh[1] == 0xabu, "tail magic");
        expect(fresh[2] == 1u && fresh[3] == 0u, "tail is marked allocated before free");
        expect(read_word(fresh + 8) == 0u, "tail name is cleared before free");
    }
    {
        reset_calls();
        unsigned char* payload = place_block(199, 0x44);
        void* result = norrath_mem_realloc(payload, 70);
        unsigned char* header = payload - 28;
        unsigned char* fresh = header + 72 + 28;
        expect(result == payload, "unaligned request still returns the original pointer");
        expect(read_word(header + 4) == 72u, "split size rounds the request up by 8");
        expect(read_word(fresh + 4) == 99u, "tail size uses the rounded request");
        expect(free_pointer == fresh + 28, "free follows the rounded tail");
    }
    {
        reset_calls();
        unsigned char* payload = place_block(80, 0x45);
        std::memset(alloc_result, 0x22, 80);
        void* result = norrath_mem_realloc(payload, 200);
        expect(result == alloc_result, "growth returns the new pointer");
        expect(alloc_calls == 1 && alloc_request == 200 && alloc_name_empty == 1, "growth allocates the clamped request");
        expect(free_calls == 0, "growth does not free the old block");
        expect(copy_count == 80u && copy_source == payload && copy_destination == alloc_result, "growth copies the old size word");
        expect(std::memcmp(alloc_result, payload, 80) == 0, "copied bytes match");
        expect(read_word(payload - 28 + 4) == 80u, "growth leaves the old size word");
    }
    {
        reset_calls();
        unsigned char* payload = place_block(16, 0x46);
        norrath_mem_realloc(payload, 10);
        expect(alloc_calls == 1 && alloc_request == 64, "a request below 64 grows to 64");
        expect(copy_count == 16u, "small growth copies the old size");
    }
    {
        reset_calls();
        unsigned char* payload = place_block(100, 0x47);
        void* result = norrath_mem_realloc(payload, -1);
        expect(result == payload && alloc_calls == 0 && free_calls == 0, "a negative request is clamped to 64 and kept");
    }
    {
        reset_calls();
        unsigned char* payload = place_block(10, 0x48);
        norrath_mem_realloc(payload, -5);
        expect(alloc_calls == 1 && alloc_request == 64, "a negative request below the old size still allocates 64");
    }
    {
        reset_calls();
        mutate_old_size = 24;
        unsigned char* payload = place_block(40, 0x49);
        alloc_result = arena + 3000;
        void* result = norrath_mem_realloc(payload, 90);
        expect(result == alloc_result, "growth returns the allocation, not memcpy's result");
        expect(copy_count == 24u, "copy count is the size word after allocation");
        expect(signed_word(0x80000000u) < 0, "signed size compare treats the high bit as negative");
    }

    std::free(arena);
    if (failures != 0) {
        std::fprintf(stderr, "%u realloc checks failed\n", failures);
        return 1;
    }
    std::printf("realloc host checks passed\n");
    return 0;
}
