#include "pad_process_data.cpp"

#include <cstdio>
#include <cstring>

int startInhibit;
int padFilter;
int padBackButton;
int padAcceptButton;

struct AnalogCall {
    int x, y;
    float* out_x;
    float* out_y;
};
static AnalogCall analog_calls[4];
static int analog_count;
// Scripted outputs, keyed by the order of the call: stick first, then left.
static float scripted[4];
static float from_bits(unsigned bits) { float v; std::memcpy(&v, &bits, 4); return v; }

void processAnalogInput__FiiPfT2(int x, int y, float* out_x, float* out_y) {
    analog_calls[analog_count] = {x, y, out_x, out_y};
    *out_x = scripted[analog_count * 2];
    *out_y = scripted[analog_count * 2 + 1];
    ++analog_count;
}

struct Fixture {
    alignas(8) unsigned char record[88];
    alignas(2) unsigned char data[8];
    alignas(8) unsigned char state[8];
    unsigned word(int offset) const { unsigned v; std::memcpy(&v, record + offset, 4); return v; }
    float real(int offset) const { float v; std::memcpy(&v, record + offset, 4); return v; }
    unsigned st(int offset) const { unsigned v; std::memcpy(&v, state + offset, 4); return v; }
    void set_state(unsigned prev, unsigned count) {
        std::memcpy(state, &prev, 4);
        std::memcpy(state + 4, &count, 4);
    }
    void set_buttons(unsigned v) { data[0] = v & 0xff; data[1] = (v >> 8) & 0xff; }
};

static void begin(Fixture& f, unsigned buttons) {
    std::memset(f.record, 0xcc, sizeof f.record);
    std::memset(f.data, 0, sizeof f.data);
    f.set_buttons(buttons);
    f.data[2] = 2; f.data[3] = 3; f.data[4] = 4; f.data[5] = 5; f.data[6] = 255; f.data[7] = 51;
    f.set_state(0, 0);
    startInhibit = 0;
    padFilter = 0;
    padBackButton = 0x100;
    padAcceptButton = 0x200;
    analog_count = 0;
    for (float& v : scripted) v = 0.0f;
}

int main() {
    Fixture f;

    // Face buttons: held word, float flags, scaled pressure bytes and calls.
    begin(f, 0x2000 | 0x20 | 0x1);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.word(8) != (0x2000u | 0x20u | 1u)) return 1;
    if (f.real(40) != 1.0f || f.real(44) != 0.0f || f.real(48) != 0.0f || f.real(52) != 0.0f) return 2;
    if (f.real(64) != 1.0f || f.real(68) != 0.0f || f.real(72) != 0.0f || f.real(76) != 0.0f) return 3;
    if (f.real(60) != 1.0f || f.real(84) != 0.0f) return 4;
    if (f.real(56) != 255.0f * from_bits(0x3b808080u) || f.real(80) != 51.0f * from_bits(0x3b808080u)) return 5;
    if (analog_count != 2) return 6;
    if (analog_calls[0].x != 4 || analog_calls[0].y != 5 || analog_calls[0].out_x != (float*)(f.record + 28) ||
        analog_calls[0].out_y != (float*)(f.record + 32)) return 7;
    if (analog_calls[1].x != 2 || analog_calls[1].y != 3 || analog_calls[1].out_x != (float*)(f.record + 20) ||
        analog_calls[1].out_y != (float*)(f.record + 24)) return 8;
    if (f.word(12) != f.word(8) || f.word(16) != f.word(8)) return 9;
    if (f.st(0) != f.word(8) || f.st(4) != 0) return 10;

    // The other flag bits.
    begin(f, 0x8000 | 0x1000 | 0x4000 | 0x80 | 0x10 | 0x40 | 0x2);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.real(40) != 0.0f || f.real(44) != 1.0f || f.real(48) != 1.0f || f.real(52) != 1.0f) return 11;
    if (f.real(64) != 0.0f || f.real(68) != 1.0f || f.real(72) != 1.0f || f.real(76) != 1.0f) return 12;
    if (f.real(60) != 0.0f || f.real(84) != 1.0f) return 13;

    // Start inhibit clears bit 0x800 once and counts down.
    begin(f, 0x800 | 0x4);
    startInhibit = 2;
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.word(8) != 0x4u || startInhibit != 1) return 14;
    begin(f, 0x800);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.word(8) != 0x800u || startInhibit != 0) return 15;

    // Left stick beyond +-0.8 adds the direction bits; the right stick does not.
    begin(f, 0);
    scripted[0] = 1.0f; scripted[1] = -1.0f;   // first call, record+28/+32
    scripted[2] = 0.9f; scripted[3] = -0.9f;   // second call, record+20/+24
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.word(8) != (0x80000u | 0x10000u)) return 16;
    begin(f, 0);
    scripted[2] = -0.9f; scripted[3] = 0.9f;
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.word(8) != (0x40000u | 0x20000u)) return 17;
    // The stored threshold itself does not exceed it; the next float up does.
    begin(f, 0);
    scripted[2] = from_bits(0x3f4ccccc); scripted[3] = from_bits(0xbf4ccccc);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.word(8) != 0u) return 18;
    begin(f, 0);
    scripted[2] = from_bits(0x3f4ccccd); scripted[3] = from_bits(0xbf4ccccd);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.word(8) != (0x80000u | 0x10000u)) return 19;

    // Pressed edge and counter: previous 0x1, now 0x3 gives pressed 0x2 and resets the counter.
    begin(f, 0x3);
    f.set_state(0x1, 9);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.word(12) != 0x2u || f.word(16) != 0x2u || f.st(4) != 0 || f.st(0) != 0x3u) return 20;

    // Same buttons held: counter increments; repeat only from 21 on multiples of 4.
    begin(f, 0x3);
    f.set_state(0x3, 5);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.st(4) != 6 || f.word(16) != 0u || f.word(12) != 0u) return 21;
    begin(f, 0x3);
    f.set_state(0x3, 19);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.st(4) != 20 || f.word(16) != 0u) return 22;   // 20 is a multiple of 4 but below 21
    begin(f, 0x3);
    f.set_state(0x3, 20);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.st(4) != 21 || f.word(16) != 0u) return 23;
    begin(f, 0x3);
    f.set_state(0x3, 23);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.st(4) != 24 || f.word(16) != 0x3u || f.word(12) != 0u) return 24;
    // No buttons: counter zero.
    begin(f, 0);
    f.set_state(0, 30);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.st(4) != 0 || f.word(8) != 0u) return 25;

    // Filter: only back/accept buttons survive and every float is zeroed.
    begin(f, 0x100 | 0x400 | 0x2000);
    padFilter = 1;
    scripted[0] = 0.5f; scripted[1] = 0.5f; scripted[2] = 0.5f; scripted[3] = 0.5f;
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.word(8) != 0x100u || f.word(12) != 0x100u || f.word(16) != 0x100u || f.st(0) != 0x100u) return 26;
    static const int floats[] = {20, 24, 28, 32, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84};
    for (int offset : floats)
        if (f.real(offset) != 0.0f) return 27;
    if (f.word(0) != 0xccccccccu || f.word(4) != 0xccccccccu || f.word(36) != 0xccccccccu) return 28;
    // Without the filter the unaffected fields keep their fill.
    begin(f, 0x1);
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(f.record, f.data, f.state);
    if (f.word(0) != 0xccccccccu || f.word(4) != 0xccccccccu || f.word(36) != 0xccccccccu) return 29;

    std::puts("pad process data passed");
    return 0;
}
