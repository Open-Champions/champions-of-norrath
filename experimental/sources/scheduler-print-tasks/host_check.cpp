#include <cstdio>
#include <cstdlib>
#include <cstdint>

extern "C" {
unsigned char nmTasks[4];
unsigned char currentTask[4] = {0x11, 0x22, 0x33, 0x44};
unsigned char taskList[16] = {0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
                              0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a};
unsigned long long norrath_scheduler_host_address_base;
void enginePrintTasks__Fv();
}

namespace {
unsigned ticks;
unsigned failures;

void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
void bind_base() {
    const auto full = reinterpret_cast<unsigned long long>(nmTasks);
    norrath_scheduler_host_address_base = full & ~0xffffffffull;
}
}

extern "C" void norrath_scheduler_host_print_tick() { ++ticks; }

int main() {
    bind_base();
    const unsigned samples[] = {0u, 1u, 2u, 4u, 0x7fffffffu, 0x80000000u, 0xffffffffu};
    const unsigned expected_ticks[] = {0u, 1u, 2u, 4u, 0x7fffffffu, 0u, 0u};
    for (unsigned sample = 0; sample < 7u; ++sample) {
        if (expected_ticks[sample] > 4u) continue;
        write_word(nmTasks, samples[sample]);
        ticks = 0;
        enginePrintTasks__Fv();
        expect(ticks == expected_ticks[sample], "countdown length");
        expect(read_word(nmTasks) == samples[sample], "count unchanged");
        expect(read_word(currentTask) == 0x44332211u, "current untouched");
        expect(taskList[0] == 0x5a && taskList[15] == 0x5a, "list untouched");
    }
    // The two high-bit counts must not be treated as huge unsigned loops.
    for (unsigned sample = 5; sample < 7u; ++sample) {
        write_word(nmTasks, samples[sample]);
        ticks = 0;
        enginePrintTasks__Fv();
        expect(ticks == 0u, "nonpositive count does not count");
        expect(read_word(nmTasks) == samples[sample], "negative count unchanged");
    }
    if (failures != 0) {
        std::fprintf(stderr, "%u failures\n", failures);
        return 1;
    }
    std::printf("print tasks host check passed\n");
    return 0;
}
