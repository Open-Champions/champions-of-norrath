#include "pad_clear.cpp"

#include <cstdio>
#include <vector>

unsigned rumphole[24];
unsigned char actSendData[8];
signed char tapCount[2];

struct Call {
    int port;
    int slot;
    unsigned char* data;
    bool operator==(const Call& other) const {
        return port == other.port && slot == other.slot && data == other.data;
    }
};
static std::vector<Call> calls;
static unsigned char send_at_call_0;

int scePadSetActDirect(int port, int slot, unsigned char* data) {
    if (calls.empty()) send_at_call_0 = data[0] | data[1];
    calls.push_back({port, slot, data});
    return 1;
}

static int run(signed char first, signed char second, const std::vector<Call>& expected) {
    for (unsigned& word : rumphole) word = 0xa5a5a5a5u;
    for (unsigned char& byte : actSendData) byte = 0xff;
    tapCount[0] = first;
    tapCount[1] = second;
    calls.clear();
    send_at_call_0 = 0;
    padClear__Fv();
    for (int index = 0; index < 24; ++index) {
        const unsigned want = (index % 6 == 3) ? 0xa5a5a5a5u : 0u;
        if (rumphole[index] != want) return 1;
    }
    if (actSendData[0] != 0 || actSendData[1] != 0) return 2;
    for (int index = 2; index < 8; ++index)
        if (actSendData[index] != 0xff) return 3;
    if (calls.size() != expected.size()) return 4;
    for (std::size_t index = 0; index < calls.size(); ++index)
        if (!(calls[index] == expected[index])) return 5;
    if (!calls.empty() && send_at_call_0 != 0) return 6;
    return 0;
}

int main() {
    unsigned char* const data = actSendData;
    int result = run(1, 1, {{0, 0, data}, {1, 0, data}});
    if (result) return 10 + result;
    result = run(4, 0, {{0, 0, data}, {0, 1, data}, {0, 2, data}, {0, 3, data}});
    if (result) return 20 + result;
    result = run(2, 2, {{0, 0, data}, {0, 1, data}, {1, 0, data}, {1, 1, data}});
    if (result) return 30 + result;
    result = run(1, 4, {{0, 0, data}, {1, 0, data}, {1, 1, data}, {1, 2, data}});
    if (result) return 40 + result;
    result = run(0, 0, {});
    if (result) return 50 + result;
    // A tap count byte of 0x80 is negative, so no slot is below it.
    result = run(static_cast<signed char>(0x80), static_cast<signed char>(0x80), {});
    if (result) return 60 + result;
    std::puts("pad clear passed");
    return 0;
}
