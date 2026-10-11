#include <cstdio>
#include <cstdint>
#include <cstring>

extern "C" {
unsigned char nmTasks[4];
unsigned char currentTask[4];
unsigned char taskList[8 * 16];
unsigned long long norrath_scheduler_host_address_base;
void engineRemoveTask__FPFv_v(void (*callback)());
}

namespace {
unsigned failures;
unsigned print_calls;
unsigned count_at_print;
unsigned current_at_print;

void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
unsigned low_address(void (*pointer)()) {
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
    expect(base == norrath_scheduler_host_address_base, "pointers share one 32-bit window");
}
void reset(unsigned count, unsigned current) {
    std::memset(taskList, 0x5a, sizeof taskList);
    write_word(nmTasks, count);
    write_word(currentTask, current);
    print_calls = 0;
}
void put_callback(unsigned index, void (*callback)(), unsigned mark) {
    unsigned char* row = taskList + index * 16u;
    write_word(row, low_address(callback));
    write_word(row + 4, mark);
    write_word(row + 8, mark ^ 0x11111111u);
    write_word(row + 12, mark ^ 0x22222222u);
}
bool same_callback(unsigned index, void (*callback)(), unsigned mark) {
    const unsigned char* row = taskList + index * 16u;
    return read_word(row) == low_address(callback) &&
           read_word(row + 4) == mark &&
           read_word(row + 8) == (mark ^ 0x11111111u) &&
           read_word(row + 12) == (mark ^ 0x22222222u);
}
volatile unsigned sink;
void cb_a() { sink = 1u; }
void cb_b() { sink = 2u; }
void cb_c() { sink = 3u; }
void cb_missing() { sink = 4u; }
}

extern "C" void enginePrintTasks__Fv() {
    ++print_calls;
    count_at_print = read_word(nmTasks);
    current_at_print = read_word(currentTask);
}

int main() {
    bind_base(nmTasks);
    bind_base(currentTask);
    bind_base(taskList);

    reset(3, 2);
    put_callback(0, cb_a, 0x10);
    put_callback(1, cb_b, 0x20);
    put_callback(2, cb_c, 0x30);
    engineRemoveTask__FPFv_v(cb_b);
    expect(print_calls == 1u, "middle removal prints");
    expect(count_at_print == 2u, "print sees count-1");
    expect(current_at_print == 1u, "current 2 rewinds to 1");
    expect(same_callback(0, cb_a, 0x10), "head stays");
    expect(same_callback(1, cb_c, 0x30), "tail copied down");
    expect(same_callback(2, cb_c, 0x30), "old tail remains");

    reset(3, 0);
    put_callback(0, cb_a, 0x10);
    put_callback(1, cb_b, 0x20);
    put_callback(2, cb_c, 0x30);
    engineRemoveTask__FPFv_v(cb_a);
    expect(current_at_print == 0xffffffffu, "current 0 wraps to all bits");
    expect(same_callback(0, cb_b, 0x20), "second record moved to slot 0");
    expect(same_callback(1, cb_c, 0x30), "third record moved to slot 1");
    expect(read_word(nmTasks) == 2u, "count is 2 after removing the head");

    reset(3, 0);
    put_callback(0, cb_a, 0x10);
    put_callback(1, cb_b, 0x20);
    put_callback(2, cb_c, 0x30);
    engineRemoveTask__FPFv_v(cb_c);
    expect(print_calls == 1u, "tail removal prints");
    expect(current_at_print == 0u, "current below the index stays");
    expect(same_callback(0, cb_a, 0x10), "tail removal keeps slot 0");
    expect(same_callback(1, cb_b, 0x20), "tail removal keeps slot 1");
    expect(same_callback(2, cb_c, 0x30), "removed tail bytes stay in place");
    expect(read_word(nmTasks) == 2u, "tail removal stores count-1");

    reset(3, 7);
    put_callback(0, cb_a, 0x10);
    put_callback(1, cb_b, 0x20);
    put_callback(2, cb_c, 0x30);
    engineRemoveTask__FPFv_v(cb_missing);
    expect(print_calls == 0u, "a miss does not print");
    expect(read_word(nmTasks) == 3u, "a miss keeps count");
    expect(read_word(currentTask) == 7u, "a miss keeps current");
    expect(same_callback(1, cb_b, 0x20), "a miss keeps the list");

    reset(0, 3);
    engineRemoveTask__FPFv_v(cb_a);
    expect(print_calls == 0u, "zero count does not print");
    expect(read_word(nmTasks) == 0u, "zero count stays");
    expect(read_word(currentTask) == 3u, "zero count keeps current");

    reset(1, 0);
    put_callback(0, cb_a, 0x10);
    engineRemoveTask__FPFv_v(cb_a);
    expect(print_calls == 1u, "only element prints");
    expect(read_word(nmTasks) == 0u, "only element clears count");
    expect(current_at_print == 0xffffffffu, "only element rewinds current 0");
    expect(same_callback(0, cb_a, 0x10), "cleared slot is not wiped");

    reset(0xffffffffu, 3);
    put_callback(0, cb_a, 0x10);
    engineRemoveTask__FPFv_v(cb_missing);
    expect(print_calls == 1u, "negative count still publishes");
    expect(read_word(nmTasks) == 0xfffffffeu, "negative count stores count-1");
    expect(current_at_print == 2u, "negative count rewinds a nonnegative current");
    expect(same_callback(0, cb_a, 0x10), "negative count does not compact");

    reset(0xffffffffu, 0xffffffffu);
    engineRemoveTask__FPFv_v(cb_a);
    expect(current_at_print == 0xffffffffu, "current below index 0 stays");
    expect(read_word(nmTasks) == 0xfffffffeu, "already-negative current still drops count");

    if (failures != 0) {
        std::fprintf(stderr, "%u failures\n", failures);
        return 1;
    }
    std::printf("remove task host check passed\n");
    return 0;
}
