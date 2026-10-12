#define NORRATH_HOST_CONTRACT 1
#include "sce_dma_pause.cpp"

#include <cstdio>
#include <string>
#include <vector>

static int dintr_result;
static int dintr_calls;
static int eintr_calls;
static std::vector<std::string> trace;
static unsigned memory_f520;

extern "C" int DIntr() {
    dintr_calls += 1;
    trace.push_back("D");
    return dintr_result;
}
extern "C" int EIntr() {
    eintr_calls += 1;
    trace.push_back("E");
    return 1;
}
extern "C" unsigned norrath_host_dma_load32(unsigned address) {
    trace.push_back(address == 0x1000f520u ? "L520" : address == 0x1000e000u ? "L000" : "L?");
    if (address == 0x1000f520u) return memory_f520;
    return 0x22u;
}
extern "C" void norrath_host_dma_store32(unsigned address, unsigned value) {
    char text[32];
    std::snprintf(text, sizeof text, "S%x=%x", address, value);
    trace.push_back(text);
}

static bool same(const std::vector<std::string>& expected) {
    return trace == expected;
}

int main() {
    unsigned channel[1] = {0x1ffu};
    dintr_result = 1;
    dintr_calls = 0;
    eintr_calls = 0;
    memory_f520 = 0x11u;
    trace.clear();
    if (sceDmaPause(channel) != 0x1ffu || channel[0] != 0xffu) return 1;
    if (dintr_calls != 1 || eintr_calls != 1) return 2;
    if (!same({"D", "L520", "S1000f590=10011", "L000", "S1000f590=11", "E"})) return 3;

    channel[0] = 0x100u;
    dintr_result = 0;
    memory_f520 = 0x10000u;
    trace.clear();
    if (sceDmaPause(channel) != 0x100u || channel[0] != 0u) return 4;
    if (eintr_calls != 1) return 5;
    if (!same({"D", "L520", "L000", "S1000f590=10000"})) return 6;

    channel[0] = 0x55u;
    dintr_result = 0;
    memory_f520 = 0u;
    trace.clear();
    if (sceDmaPause(channel) != 0x55u || channel[0] != 0x55u) return 7;
    if (!same({"D", "L520", "S1000f590=10000", "L000", "S1000f590=0"})) return 8;
    std::puts("dma pause contract passed");
    return 0;
}
