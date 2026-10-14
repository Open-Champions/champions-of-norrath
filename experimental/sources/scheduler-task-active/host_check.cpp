#include <cstdio>
#include <cstdint>

extern "C" {
unsigned char nmTasks[4];
unsigned char currentTask[4];
unsigned char taskList[8 * 16];
unsigned long long norrath_scheduler_host_address_base;
int engineTaskActive__FPc(char* name);
}

namespace {
unsigned failures;
unsigned strcmp_calls;
unsigned shrink_on_call;

void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
unsigned low_address(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<uintptr_t>(pointer));
}
void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
void bind_base(const void* pointer) {
    const auto full = reinterpret_cast<unsigned long long>(pointer);
    const auto base = full & ~0xffffffffull;
    if (norrath_scheduler_host_address_base == 0ull && base != 0ull)
        norrath_scheduler_host_address_base = base;
    expect((full & ~0xffffffffull) == norrath_scheduler_host_address_base,
           "pointers share one 32-bit window");
}
char labels[4][8] = {"one", "two", "three", "four"};

void set_count(unsigned count) {
    write_word(nmTasks, count);
    for (unsigned index = 0; index < 4u; ++index)
        write_word(taskList + index * 16u + 8u, low_address(labels[index]));
}
}

int compare_text(const char* left, const char* right) {
    while (*left != 0 && *left == *right) {
        ++left;
        ++right;
    }
    return static_cast<unsigned char>(*left) - static_cast<unsigned char>(*right);
}
extern "C" int strcmp(const char* left, const char* right) {
    ++strcmp_calls;
    if (shrink_on_call != 0u && strcmp_calls == shrink_on_call)
        write_word(nmTasks, 1u);
    return compare_text(left, right);
}

int main() {
    bind_base(nmTasks);
    bind_base(taskList);
    bind_base(labels);

    set_count(0);
    strcmp_calls = 0;
    expect(engineTaskActive__FPc(labels[0]) == 0, "zero count returns 0");
    expect(strcmp_calls == 0u, "zero count does not compare");

    set_count(0xffffffffu);
    strcmp_calls = 0;
    expect(engineTaskActive__FPc(labels[0]) == 0, "negative count returns 0");
    expect(strcmp_calls == 0u, "negative count does not compare");

    set_count(3);
    strcmp_calls = 0;
    expect(engineTaskActive__FPc(labels[0]) == 1, "first text matches");
    expect(strcmp_calls == 1u, "first match stops");

    set_count(3);
    strcmp_calls = 0;
    expect(engineTaskActive__FPc(labels[2]) == 1, "last text matches");
    expect(strcmp_calls == 3u, "last match compared each text");

    set_count(3);
    strcmp_calls = 0;
    char missing[] = {'n', 'o', 0};
    expect(engineTaskActive__FPc(missing) == 0, "absent text returns 0");
    expect(strcmp_calls == 3u, "absent text scans the count");
    expect(read_word(nmTasks) == 3u, "scan does not store count");

    set_count(3);
    strcmp_calls = 0;
    shrink_on_call = 1u;
    expect(engineTaskActive__FPc(labels[2]) == 0, "reloaded count stops the scan");
    expect(strcmp_calls == 1u, "reloaded count skips later texts");
    expect(read_word(nmTasks) == 1u, "the compare hook published the new count");
    shrink_on_call = 0u;

    if (failures != 0) {
        std::fprintf(stderr, "%u failures\n", failures);
        return 1;
    }
    std::printf("task active host check passed\n");
    return 0;
}
