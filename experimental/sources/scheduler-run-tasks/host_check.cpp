#include <cstdio>
#include <cstdint>
#include <cstring>

extern "C" {
unsigned char nmTasks[4];
unsigned char currentTask[4];
unsigned char taskList[8 * 16];
unsigned long long norrath_scheduler_host_address_base;
void engineRunTasks__Fii(int mask, int select);
}

namespace {
unsigned failures;
unsigned calls[4];
unsigned seen_current[4];
unsigned words[4];
void (*fns[4])();

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
void note(unsigned slot) {
    calls[slot] += 1u;
    seen_current[slot] = read_word(currentTask);
}
void fn0() { note(0); }
void fn1() { note(1); }
void fn2() { note(2); }
void fn3() { note(3); }

void reset(unsigned count, unsigned current) {
    std::memset(taskList, 0, sizeof taskList);
    std::memset(calls, 0, sizeof calls);
    write_word(nmTasks, count);
    write_word(currentTask, current);
    void (*local[4])() = {fn0, fn1, fn2, fn3};
    const unsigned flags[4] = {0u, 1u, 5u, 2u};
    for (unsigned index = 0; index < 4u; ++index) {
        fns[index] = local[index];
        words[index] = low_address(local[index]);
        write_word(taskList + index * 16u, words[index]);
        write_word(taskList + index * 16u + 12u, flags[index]);
    }
}
}

extern "C" void norrath_scheduler_host_call(unsigned word) {
    for (unsigned index = 0; index < 4u; ++index) {
        if (words[index] == word) {
            fns[index]();
            return;
        }
    }
    std::fprintf(stderr, "fail: unknown callback %08x\n", word);
    ++failures;
}

int main() {
    bind_base(nmTasks);
    bind_base(currentTask);
    bind_base(taskList);

    reset(0, 9);
    engineRunTasks__Fii(1, -1);
    expect(read_word(currentTask) == 0u, "nonpositive count still clears current");
    expect(calls[0] == 0u, "zero count runs nothing");

    reset(0xffffffffu, 4);
    engineRunTasks__Fii(0, 1);
    expect(read_word(currentTask) == 0u, "negative count clears current");
    expect(calls[0] == 0u, "negative count runs nothing");

    // Observed call from coreGameStep: mask 1, select -1.
    reset(4, 9);
    engineRunTasks__Fii(1, -1);
    expect(calls[0] == 1u && calls[3] == 1u, "mask 1 runs flags without bit 0");
    expect(calls[1] == 0u && calls[2] == 0u, "mask 1 skips bit 0");
    expect(seen_current[0] == 0u, "first callback sees current 0");
    expect(seen_current[3] == 3u, "later callback sees its index");
    expect(read_word(currentTask) == 4u, "walk ends by storing the stopping index");

    // Observed call from gameLoop: mask 0, select 1.
    reset(4, 9);
    engineRunTasks__Fii(0, 1);
    expect(calls[1] == 1u && calls[2] == 1u, "select 1 runs bit 0");
    expect(calls[0] == 0u && calls[3] == 0u, "select 1 skips flags without bit 0");
    expect(seen_current[1] == 1u, "skipped head leaves current at the next index");
    expect(read_word(currentTask) == 4u, "filtered walk still stores the end index");

    reset(4, 0);
    // Replace fn0 with a callback that jumps the cursor and shrinks the count.
    fns[0] = []() {
        note(0);
        write_word(currentTask, 5u);
    };
    engineRunTasks__Fii(1, -1);
    expect(calls[0] == 1u, "mutating callback runs");
    expect(calls[3] == 0u, "current 5 skips index 3");
    expect(read_word(currentTask) == 6u, "stored next index replaces the callback value");

    reset(3, 0);
    fns[0] = []() {
        note(0);
        write_word(currentTask, 9u);
        write_word(nmTasks, 1u);
    };
    engineRunTasks__Fii(1, -1);
    expect(calls[0] == 1u, "shrinking callback runs once");
    expect(read_word(currentTask) == 10u, "final store keeps current+1 after the call");
    expect(read_word(nmTasks) == 1u, "callback count is left stored");

    if (failures != 0) {
        std::fprintf(stderr, "%u failures\n", failures);
        return 1;
    }
    std::printf("run tasks host check passed\n");
    return 0;
}
