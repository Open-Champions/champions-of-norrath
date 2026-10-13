#include "sound_device_flush.cpp"

#include <cstring>
#include <string>
#include <vector>

// Sounds are small integer handles in the table. Each has a scripted type and
// a private block of ints used for both wave and xm views.
static unsigned char device[4600];
static int blocks[16][16];
static int types[16];
static std::vector<std::string> trace;
static int idle_calls;
static int fix_on_begin;  // when set, Begin* calls set the state word so the next walk passes

static int handle_of(const VISound* s) { return static_cast<int>(reinterpret_cast<unsigned long>(s)); }

extern "C" int IdleTransfer__13VISoundDevice(VISoundDevice*) {
    trace.push_back("idle");
    if (++idle_calls == 2) *reinterpret_cast<int*>(device + 3412) = 1;
    return 0;
}
extern "C" void SetIopFunction__FUii(unsigned f, int v) {
    trace.push_back("iop" + std::to_string(f) + "," + std::to_string(v));
}
extern "C" void FlushIopCommand__Fb(bool w) { trace.push_back(std::string("flush") + (w ? "1" : "0")); }
extern "C" int Type__C7VISound(const VISound* s) {
    trace.push_back("T" + std::to_string(handle_of(s)));
    return types[handle_of(s)];
}
extern "C" int* AsWave__7VISound(VISound* s) {
    trace.push_back("W" + std::to_string(handle_of(s)));
    return blocks[handle_of(s)];
}
extern "C" int* AsXm__7VISound(VISound* s) {
    trace.push_back("X" + std::to_string(handle_of(s)));
    return blocks[handle_of(s)];
}

static int block_of(const int* p) {
    for (int i = 0; i < 16; ++i)
        if (p >= blocks[i] && p < blocks[i] + 16) return i;
    return -1;
}
static std::string where(const void* p) {
    const int* ip = static_cast<const int*>(p);
    int b = block_of(ip);
    return std::to_string(b) + "+" + std::to_string(ip - blocks[b]);
}

extern "C" void BeginAdpcmTransfer__13VISoundDevicePPvUiiPi(VISoundDevice* d, void** dst, unsigned size,
                                                           int bank, int* state) {
    trace.push_back("A" + std::string(d == reinterpret_cast<VISoundDevice*>(device) ? "d" : "?") + ":" +
                    where(dst) + ":" + std::to_string(size) + ":" + std::to_string(bank) + ":" + where(state));
    if (fix_on_begin) *state = 1;
}
extern "C" void BeginXmTransfer__13VISoundDevicePPvUiUiiPi(VISoundDevice* d, void** dst, unsigned size,
                                                          unsigned count, int bank, int* state) {
    trace.push_back("X" + std::string(d == reinterpret_cast<VISoundDevice*>(device) ? "d" : "?") + ":" +
                    where(dst) + ":" + std::to_string(size) + ":" + std::to_string(count) + ":" +
                    std::to_string(bank) + ":" + where(state));
    if (fix_on_begin) *state = 1;
}

static void reset() {
    std::memset(device, 0, sizeof device);
    std::memset(blocks, 0, sizeof blocks);
    std::memset(types, 0, sizeof types);
    for (int b = 0; b < 16; ++b)
        for (int i = 0; i < 16; ++i) blocks[b][i] = 100 * b + i;
    trace.clear();
    idle_calls = 0;
    fix_on_begin = 1;
}
static void set_slot(int slot, unsigned handle) { std::memcpy(device + 2584 + 4 * slot, &handle, 4); }

int main() {
    // Empty table with the enable word set: one walk, nothing called.
    reset();
    *reinterpret_cast<int*>(device + 3412) = 1;
    if (Flush__13VISoundDevice(reinterpret_cast<VISoundDevice*>(device)) != 0) return 1;
    if (!trace.empty()) return 2;

    // Enable word clear: the idle sequence repeats until it becomes set.
    reset();
    if (Flush__13VISoundDevice(reinterpret_cast<VISoundDevice*>(device)) != 0) return 3;
    std::vector<std::string> idle = {"idle", "iop3,0", "flush0", "idle", "iop3,0", "flush0"};
    if (trace != idle) return 4;

    // A wave with the state word zero starts an ADPCM transfer, then the walk
    // restarts; the second walk finds the state word set.
    reset();
    *reinterpret_cast<int*>(device + 3412) = 1;
    set_slot(3, 1);
    types[1] = 0;
    blocks[1][10] = 0;
    if (Flush__13VISoundDevice(reinterpret_cast<VISoundDevice*>(device)) != 0) return 5;
    std::vector<std::string> wave = {"T1", "W1", "W1", "Ad:1+13:115:114:1+10", "T1", "W1"};
    if (trace != wave) return 6;

    // Type 1 with word 8 zero: ADPCM transfer from the xm view.
    reset();
    *reinterpret_cast<int*>(device + 3412) = 1;
    set_slot(0, 2);
    types[2] = 1;
    blocks[2][8] = 0;
    if (Flush__13VISoundDevice(reinterpret_cast<VISoundDevice*>(device)) != 0) return 7;
    std::vector<std::string> xm1 = {"T2", "X2", "X2", "Ad:2+11:205:212:2+8", "T2", "X2", "X2"};
    if (trace != xm1) return 8;

    // Type 1 with word 8 set and word 7 zero: XM transfer.
    reset();
    *reinterpret_cast<int*>(device + 3412) = 1;
    set_slot(127, 4);
    types[4] = 1;
    blocks[4][8] = 5;
    blocks[4][7] = 0;
    if (Flush__13VISoundDevice(reinterpret_cast<VISoundDevice*>(device)) != 0) return 9;
    std::vector<std::string> xm2 = {"T4", "X4", "X4", "X4", "Xd:4+9:406:405:410:4+7", "T4", "X4", "X4"};
    if (trace != xm2) return 10;

    // Other types and zero entries are skipped; later slots are still visited.
    reset();
    *reinterpret_cast<int*>(device + 3412) = 1;
    set_slot(1, 5);
    set_slot(2, 6);
    types[5] = 2;
    types[6] = -1;
    if (Flush__13VISoundDevice(reinterpret_cast<VISoundDevice*>(device)) != 0) return 11;
    std::vector<std::string> other = {"T5", "T6"};
    if (trace != other) return 12;

    // Two sounds: the first needs a transfer, restart revisits it, then the second.
    reset();
    *reinterpret_cast<int*>(device + 3412) = 1;
    set_slot(0, 7);
    set_slot(5, 8);
    types[7] = 0;
    types[8] = 0;
    blocks[7][10] = 0;
    blocks[8][10] = 0;
    if (Flush__13VISoundDevice(reinterpret_cast<VISoundDevice*>(device)) != 0) return 13;
    std::vector<std::string> two = {"T7", "W7", "W7", "Ad:7+13:715:714:7+10", "T7", "W7",
                                    "T8", "W8", "W8", "Ad:8+13:815:814:8+10", "T7", "W7", "T8", "W8"};
    if (trace != two) return 14;
    return 0;
}
