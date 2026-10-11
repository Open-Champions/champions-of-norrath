#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" unsigned char pad[352] = {};
extern "C" unsigned char stateData[32] = {};
extern "C" unsigned char tapCount[2] = {};
extern "C" unsigned char padIndexMap[4] = {};
extern "C" unsigned char netLocalPad[352] = {};
extern "C" unsigned char netNetworkMode = 0;
extern "C" unsigned pad_frame asm("padFrame.756") = 0;

static int slot_max[2];
static int slot_calls;
static int data_index[4];
static void* data_state[4];
static int data_result = 1;
static int process_calls;
static char names[4][16];

extern "C" int scePadGetSlotMax(int port) {
    if (port < 0 || port > 1) std::abort();
    slot_calls += 1;
    return slot_max[port];
}
extern "C" int sprintf(char* destination, const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    const int wrote = std::vsnprintf(destination, 16, format, args);
    va_end(args);
    return wrote;
}
extern "C" int padGetUnprocessedData__FiP18padUnprocessedData(int, void*) { return data_result; }
extern "C" void padProcessData__FP3padP18padUnprocessedDataP12padStateData(void* record, void*,
                                                                            void* state) {
    const int index = process_calls;
    data_index[index] = static_cast<int>(static_cast<unsigned char*>(record) - pad) / 88;
    data_state[index] = state;
    std::memcpy(names[index], record, 8);
    process_calls += 1;
}
extern "C" void padProcess__Fv();

static int fail(int code) {
    std::fprintf(stderr, "pad process failed %d\n", code);
    return code;
}

int main() {
    std::memset(pad, 0, sizeof pad);
    std::memset(netLocalPad, 0xab, sizeof netLocalPad);
    std::memset(stateData, 0, sizeof stateData);
    padIndexMap[0] = 0;
    padIndexMap[1] = 1;
    padIndexMap[2] = 2;
    padIndexMap[3] = 3;
    pad_frame = 0;
    slot_max[0] = 4;
    slot_max[1] = 4;
    netNetworkMode = 0;
    pad[4] = 2;
    padProcess__Fv();
    if (pad_frame != 1 || slot_calls != 2) return fail(1);
    if (tapCount[0] != 4 || tapCount[1] != 0) return fail(2);
    if (std::strcmp(reinterpret_cast<char*>(pad), "1-A") != 0) return fail(3);
    if (std::strcmp(reinterpret_cast<char*>(pad + 88), "1-B") != 0) return fail(4);
    if (std::strcmp(reinterpret_cast<char*>(pad + 176), "1-C") != 0) return fail(5);
    if (std::strcmp(reinterpret_cast<char*>(pad + 264), "1-D") != 0) return fail(6);
    if (pad[4] != 30 || process_calls != 4) return fail(7);
    if (data_index[3] != 3 || data_state[3] != stateData + 24) return fail(8);
    if (std::memcmp(netLocalPad, pad, 88) != 0) return fail(9);

    const unsigned char saved = pad[0];
    pad_frame = 1;
    slot_calls = 0;
    process_calls = 0;
    data_result = 0;
    pad[4] = 1;
    netNetworkMode = 1;
    std::memset(netLocalPad, 0xab, sizeof netLocalPad);
    padProcess__Fv();
    if (slot_calls != 0 || pad_frame != 2) return fail(10);
    if (pad[0] != saved) return fail(11);
    if (pad[4] != 0) return fail(12);
    if (netLocalPad[0] != 0xab) return fail(13);

    pad_frame = 180;
    slot_max[0] = 1;
    slot_max[1] = 1;
    padIndexMap[0] = 5;
    padIndexMap[1] = 6;
    process_calls = 0;
    padProcess__Fv();
    if (tapCount[0] != 1 || tapCount[1] != 1) return fail(14);
    if (std::strcmp(reinterpret_cast<char*>(pad), "6") != 0) return fail(15);
    if (std::strcmp(reinterpret_cast<char*>(pad + 88), "7") != 0) return fail(16);
    std::puts("pad process contract passed");
    return 0;
}
