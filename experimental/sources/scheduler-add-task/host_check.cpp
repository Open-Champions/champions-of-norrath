#include <cstdio>
#include <cstdint>
#include <cstring>

extern "C" {
unsigned char nmTasks[4];
unsigned char currentTask[4];
unsigned char taskList[8 * 16];
unsigned long long norrath_scheduler_host_address_base;
void engineAddTask__FPFv_viPci(void (*callback)(), int key, char* text, int flags);
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
unsigned low_address(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<uintptr_t>(pointer));
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
void put_record(unsigned index, void (*callback)(), int key, char* text, int flags) {
    unsigned char* row = taskList + index * 16u;
    write_word(row, low_address(callback));
    write_word(row + 4, static_cast<unsigned>(key));
    write_word(row + 8, low_address(text));
    write_word(row + 12, static_cast<unsigned>(flags));
}
bool same_record(unsigned index, void (*callback)(), int key, char* text, int flags) {
    const unsigned char* row = taskList + index * 16u;
    return read_word(row) == low_address(callback) &&
           read_word(row + 4) == static_cast<unsigned>(key) &&
           read_word(row + 8) == low_address(text) &&
           read_word(row + 12) == static_cast<unsigned>(flags);
}
volatile unsigned sink;
void cb_a() { sink = 1u; }
void cb_b() { sink = 2u; }
void cb_c() { sink = 3u; }
void cb_d() { sink = 4u; }
char text_a[] = {'a', 0};
char text_b[] = {'b', 0};
char text_c[] = {'c', 0};
char text_d[] = {'d', 0};
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
    bind_base(text_a);

    reset(0, 4);
    engineAddTask__FPFv_viPci(cb_a, 5, text_a, 7);
    expect(print_calls == 1u, "empty insert calls print");
    expect(count_at_print == 1u, "print sees the stored count");
    expect(current_at_print == 5u, "current advances from 4 at slot 0");
    expect(same_record(0, cb_a, 5, text_a, 7), "first record fields");
    expect(read_word(nmTasks) == 1u, "count is 1");

    reset(3, 2);
    put_record(0, cb_a, 10, text_a, 1);
    put_record(1, cb_b, 20, text_b, 2);
    put_record(2, cb_c, 30, text_c, 3);
    engineAddTask__FPFv_viPci(cb_a, 99, text_d, 9);
    expect(print_calls == 0u, "duplicate callback does not print");
    expect(read_word(nmTasks) == 3u, "duplicate keeps count");
    expect(read_word(currentTask) == 2u, "duplicate keeps current");
    expect(same_record(0, cb_a, 10, text_a, 1), "duplicate keeps slot 0");
    expect(same_record(1, cb_b, 20, text_b, 2), "duplicate keeps slot 1");
    expect(taskList[48] == 0x5a, "duplicate does not write the next slot");

    reset(3, 2);
    put_record(0, cb_a, 10, text_a, 1);
    put_record(1, cb_b, 20, text_b, 2);
    put_record(2, cb_c, 30, text_c, 3);
    // Bytes that must move with the record, not just the four field words.
    taskList[32] = 0x01;
    taskList[33] = 0x02;
    taskList[34] = 0x03;
    taskList[35] = 0x04;
    engineAddTask__FPFv_viPci(cb_d, 15, text_d, 8);
    expect(print_calls == 1u, "ordered insert prints");
    expect(read_word(nmTasks) == 4u, "count becomes 4");
    expect(current_at_print == 3u, "current 2 advances because 2 >= slot 1");
    expect(same_record(0, cb_a, 10, text_a, 1), "lower key stays");
    expect(same_record(1, cb_d, 15, text_d, 8), "new record at slot 1");
    expect(same_record(2, cb_b, 20, text_b, 2), "middle record moved up");
    expect(taskList[48] == 0x01 && taskList[49] == 0x02 &&
           taskList[50] == 0x03 && taskList[51] == 0x04, "moved bytes keep order");
    expect(same_record(3, cb_c, 30, text_c, 3) == false, "slot 3 holds the moved tail");
    expect(read_word(taskList + 52) == static_cast<unsigned>(30), "tail key moved");

    reset(3, 0);
    put_record(0, cb_a, 10, text_a, 1);
    put_record(1, cb_b, 20, text_b, 2);
    put_record(2, cb_c, 30, text_c, 3);
    engineAddTask__FPFv_viPci(cb_d, 15, text_d, 8);
    expect(current_at_print == 0u, "current 0 stays when insertion is slot 1");

    reset(3, 1);
    put_record(0, cb_a, 10, text_a, 1);
    put_record(1, cb_b, 20, text_b, 2);
    put_record(2, cb_c, 30, text_c, 3);
    engineAddTask__FPFv_viPci(cb_d, 20, text_d, 4);
    expect(same_record(0, cb_a, 10, text_a, 1), "equal key leaves earlier records");
    expect(same_record(1, cb_b, 20, text_b, 2), "equal key stays ahead");
    expect(same_record(2, cb_d, 20, text_d, 4), "equal key inserts after equals");
    expect(read_word(taskList + 52) == static_cast<unsigned>(30), "greater key moved up");
    expect(current_at_print == 1u, "current 1 stays below slot 2");

    reset(2, 0);
    put_record(0, cb_a, -5, text_a, 1);
    put_record(1, cb_b, 0, text_b, 2);
    engineAddTask__FPFv_viPci(cb_d, -1, text_d, 6);
    expect(same_record(0, cb_a, -5, text_a, 1), "more-negative key stays first");
    expect(same_record(1, cb_d, -1, text_d, 6), "negative key inserts before zero");
    expect(same_record(2, cb_b, 0, text_b, 2), "zero key moved up");

    reset(1, 0);
    put_record(0, cb_a, 10, text_a, 1);
    engineAddTask__FPFv_viPci(cb_d, 40, text_d, 3);
    expect(same_record(0, cb_a, 10, text_a, 1), "larger key does not shift");
    expect(same_record(1, cb_d, 40, text_d, 3), "larger key appends");
    expect(current_at_print == 0u, "current stays below the new tail");

    if (failures != 0) {
        std::fprintf(stderr, "%u failures\n", failures);
        return 1;
    }
    std::printf("add task host check passed\n");
    return 0;
}
