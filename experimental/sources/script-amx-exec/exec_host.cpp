#include <cstdio>
#include <cstring>
#include <cstdlib>

extern "C" {
unsigned char norrath_amx_little_endian_word_view[4] = {0, 0, 0, 0};
unsigned* norrath_amx_exec_host_ids = nullptr;
unsigned char* norrath_amx_exec_host_base = nullptr;
unsigned long long (*norrath_amx_exec_host_callback)(void*, int, int*, void*) = nullptr;

unsigned long long norrath_amx_exec(void* amx, int* result, long long public_index,
                                     long long parameter_count, const void* parameter_data);

void* norrath_copy_bytes(void* destination, const void* source, __SIZE_TYPE__ count) {
    return std::memcpy(destination, source, count);
}
int norrath_memory_compare(const void* left, const void* right, __SIZE_TYPE__ count) {
    const auto* a = static_cast<const unsigned char*>(left);
    const auto* b = static_cast<const unsigned char*>(right);
    for (__SIZE_TYPE__ i = 0; i < count; ++i) {
        if (a[i] != b[i]) return int(a[i]) - int(b[i]);
    }
    return 0;
}
}

namespace {
int failures = 0;
void expect(bool ok, const char* name) {
    if (ok) return;
    std::fprintf(stderr, "fail %s\n", name);
    ++failures;
}
unsigned read32(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write32(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}

constexpr unsigned kData = 64;
constexpr unsigned kCode = 1088;
constexpr unsigned kStack = 768;
constexpr unsigned kLimit = 1024;

struct Fixture {
    unsigned char arena[4096]{};
    unsigned char amx[64]{};
    int result = 0x22222222;
    void reset(unsigned flags = 0x8000u) {
        std::memset(arena, 0, sizeof arena);
        std::memset(amx, 0, sizeof amx);
        result = 0x22222222;
        write32(amx + 4, 1);
        write32(amx + 20, 0);
        write32(amx + 24, kStack);
        write32(amx + 28, kLimit);
        write32(amx + 32, flags);
        write32(arena + 12, kCode);
        write32(arena + 16, kData);
        write32(arena + 24, kLimit);
        norrath_amx_exec_host_base = arena;
        for (int i = 0; i < 4; ++i) norrath_amx_little_endian_word_view[i] = 0;
    }
    void code(const unsigned* words, unsigned count) {
        for (unsigned i = 0; i < count; ++i) write32(arena + kCode + i * 4u, words[i]);
    }
    unsigned long long run(long long selector = -1, long long params = 0, const void* data = nullptr) {
        return norrath_amx_exec(amx, &result, selector, params, data);
    }
};

unsigned seen_stack = 0;
unsigned long long add_index(void*, int index, int* pri, void*) {
    *pri += index;
    return 0;
}
unsigned long long watch_stack(void* amx, int, int*, void*) {
    seen_stack = read32(static_cast<unsigned char*>(amx) + 24);
    return 0;
}
unsigned long long fail_callback(void*, int, int*, void*) { return 4; }
unsigned long long unexpected(void*, int, int*, void*) { std::abort(); }
}

int main() {
    norrath_amx_exec_host_callback = unexpected;
    int discarded = 0;
    const unsigned long long null_status = norrath_amx_exec(nullptr, &discarded, 0, 0, nullptr);
    expect(null_status == 0, "null status");
    expect(norrath_amx_exec_host_ids != nullptr, "null ids");
    bool ids_ok = norrath_amx_exec_host_ids != nullptr;
    for (unsigned i = 0; ids_ok && i < 131u; ++i) ids_ok = norrath_amx_exec_host_ids[i] == i;
    expect(ids_ok, "handler ids");

    Fixture fx;
    fx.reset(0xc000u);
    write32(fx.amx + 4, 0);
    expect(fx.run() == 9, "missing callback");
    expect(read32(fx.amx + 32) == 0xc000u, "flags untouched");
    expect(fx.result == 0x22222222, "result untouched");

    fx.reset(0x4000u);
    expect(fx.run() == 22, "flags rejected");
    expect(read32(fx.amx + 32) == 0, "flag bit cleared");
    expect(fx.result == 0x22222222, "reject result");

    fx.reset();
    write32(fx.amx + 20, 0xffffffffu);
    expect(fx.run() == 8, "negative heap");

    fx.reset();
    write32(fx.arena + 24, 100);
    write32(fx.amx + 24, 200);
    expect(fx.run() == 7, "stp below stack");

    fx.reset();
    write32(fx.arena + 28, 0xffffffffu);
    expect(fx.run() == 20, "negative cip");
    expect(fx.result == 0x22222222, "cip result");

    fx.reset();
    fx.arena[32] = 1;
    expect(fx.run(5) == 20, "selector past count");
    expect(fx.run(-3) == 20, "selector rejected");

    fx.reset(0xc000u);
    const unsigned halted[] = {11, 42, 120, 0};
    fx.code(halted, 4);
    expect(fx.run() == 0, "halt status");
    expect(fx.result == 42, "halt value");
    expect(read32(fx.amx + 32) == 0x8000u, "run flags");
    expect(read32(fx.amx + 24) == kStack, "halt restores entry stack");

    fx.reset();
    const unsigned added[] = {11, 20, 12, 22, 78, 120, 0};
    fx.code(added, 7);
    expect(fx.run() == 0 && fx.result == 42, "add");

    fx.reset();
    const unsigned divided[] = {11, 0xfffffff9u, 12, 3, 73, 16, 0, 120, 0};
    fx.code(divided, 9);
    expect(fx.run() == 0 && fx.result == -3, "signed quotient");
    expect(read32(fx.arena + kData) == 2u, "signed remainder");

    fx.reset();
    const unsigned jump[] = {52, 8, 120, 9, 11, 5, 120, 0};
    fx.code(jump, 8);
    expect(fx.run() == 0 && fx.result == 5, "relative jump");

    fx.reset();
    const unsigned widened[] = {11, 0x12345680u, 93, 120, 0};
    fx.code(widened, 5);
    expect(fx.run() == 0 && fx.result == static_cast<int>(0xffffff80u), "sign byte");
    const unsigned narrow[] = {11, 0x1234567fu, 93, 120, 0};
    fx.code(narrow, 5);
    expect(fx.run() == 0 && fx.result == static_cast<int>(0x1234567fu), "sign byte unchanged");

    fx.reset();
    norrath_amx_little_endian_word_view[0] = 1;
    const unsigned aligned[] = {11, 0, 29, 1, 120, 0};
    fx.code(aligned, 6);
    expect(fx.run() == 0 && fx.result == 3, "endian adjust");
    norrath_amx_little_endian_word_view[0] = 0;
    expect(fx.run() == 0 && fx.result == 0, "endian idle");

    fx.reset();
    unsigned char argument[8] = {0x99, 0, 0, 0, 0, 0, 0, 0};
    const unsigned loaded[] = {31, 4, 87, 8, 9, 120, 0};
    fx.code(loaded, 7);
    expect(fx.run(-1, 1, argument) == 0 && fx.result == 0x99, "parameter word");

    fx.reset();
    const unsigned bad[] = {0};
    fx.code(bad, 1);
    expect(fx.run() == 6, "bad opcode");
    expect(read32(fx.amx + 24) == kStack, "bad opcode stack");

    fx.reset();
    fx.arena[32] = 1;
    write32(fx.arena + 36, 40);
    write32(fx.arena + 40, 8);
    const unsigned named[] = {120, 9, 11, 5, 120, 0};
    fx.code(named, 6);
    expect(fx.run(0) == 0 && fx.result == 5, "public entry");

    norrath_amx_exec_host_callback = watch_stack;
    fx.reset();
    seen_stack = 0;
    const unsigned watched[] = {123, 0, 11, 42, 120, 0};
    fx.code(watched, 6);
    expect(fx.run() == 0 && fx.result == 42, "callback continues");
    expect(seen_stack == kStack - 8u, "callback sees adjusted stack");
    expect(read32(fx.amx + 24) == kStack, "halt restores after callback");

    norrath_amx_exec_host_callback = add_index;
    fx.reset();
    const unsigned indexed[] = {11, 7, 123, 3, 120, 0};
    fx.code(indexed, 6);
    expect(fx.run() == 0 && fx.result == 10, "callback index");

    norrath_amx_exec_host_callback = fail_callback;
    fx.reset();
    const unsigned failed[] = {123, 1, 11, 1, 120, 0};
    fx.code(failed, 6);
    expect(fx.run() == 4, "callback status");
    expect(read32(fx.amx + 24) == kStack, "callback restores stack");
    expect(fx.result == 0x22222222, "callback failure result");

    fx.reset();
    const unsigned copied[] = {11, 0x11223344u, 15, 0, 12, 4, 11, 0, 117, 4, 1, 4, 120, 0};
    fx.code(copied, 14);
    expect(fx.run() == 0 && fx.result == static_cast<int>(0x11223344u), "copy bytes");

    if (failures != 0) return 1;
    std::puts("amx exec passed");
    return 0;
}
