// Observed: padProcessData__FP3padP18padUnprocessedDataP12padStateData at
// 0x001678f0 is 788 bytes. Arguments: record (an 88-byte pad entry), data (the
// 8-byte output of padGetUnprocessedData) and state (8 bytes of stateData, word
// +0 the previous button word, word +4 a hold counter).
// Sequence: (1) record+8 = half word at data+0; if startInhibit (0x00484050)
// is nonzero, bit 0x800 is cleared from record+8 and startInhibit decreases by
// one. (2) Float flags 1.0 or 0.0 are stored from record+8 bits: +40 from
// 0x2000, +44 from 0x8000, +48 from 0x1000, +52 from 0x4000, +64 from 0x20,
// +68 from 0x80, +72 from 0x10, +76 from 0x40, +60 from bit 0x1 (as the
// converted integer), +84 from 0x2. +56 and +80 hold data[6] and data[7]
// scaled by the single float 0x3b808080 stored at 0x0047ffa4. (3)
// processAnalogInput__FiiPfT2(data[4], data[5], record+28, record+32) and
// (data[2], data[3], record+20, record+24) are called. Against the floats at
// 0x0047ffa8 (bits 0x3f4ccccc, one step below the float nearest 0.8) and
// 0x0047ffac (bits 0xbf4ccccc, its negation), record+8 gains
// 0x80000 when +20 exceeds 0.8, 0x40000 when +20 is below -0.8, 0x20000 when
// +24 exceeds 0.8 and 0x10000 when +24 is below -0.8. (4) record+12 and
// record+16 both receive the buttons that are set now and were not set in
// state word +0. (5) Buttons zero or different from state word +0 reset
// state word +4 to zero. Otherwise it increments; once it reaches 21 and
// its low two bits are zero, record+16 is replaced by the whole button word.
// (6) If padFilter (0x00484074) is nonzero, record+8, +12 and +16 are masked
// with padBackButton (0x00484048) or padAcceptButton (0x0048404c), and every
// float at +20 through +84 that this function writes is set to 0.0. (7) The
// final record+8 is stored to state word +0.
// Inferred: record+8 is the held buttons, +12 newly pressed, +16 repeating;
// the 0x10000-0x80000 bits are the left stick as digital directions.
// Unknown: the pad record fields not named above (+0, +4, +36); the sign and
// range of the analog helper's output, which is not authored here.
extern "C" {
extern int startInhibit;
extern int padFilter;
extern int padBackButton;
extern int padAcceptButton;

void processAnalogInput__FiiPfT2(int x, int y, float* out_x, float* out_y);
}

namespace {
unsigned load_word(const unsigned char* base, int offset) {
    unsigned value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
void store_word(unsigned char* base, int offset, unsigned value) {
    __builtin_memcpy(base + offset, &value, sizeof value);
}
float load_float(const unsigned char* base, int offset) {
    float value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
void store_float(unsigned char* base, int offset, float value) {
    __builtin_memcpy(base + offset, &value, sizeof value);
}
float flag(unsigned buttons, unsigned mask) {
    return (buttons & mask) != 0u ? 1.0f : 0.0f;
}
float bits_to_float(unsigned bits) {
    float value;
    __builtin_memcpy(&value, &bits, sizeof value);
    return value;
}
}  // namespace

extern "C" void padProcessData__FP3padP18padUnprocessedDataP12padStateData(unsigned char* record,
                                                                          unsigned char* data,
                                                                          unsigned char* state) {
    const unsigned previous = load_word(state, 0);
    unsigned buttons = static_cast<unsigned>(data[0]) | (static_cast<unsigned>(data[1]) << 8);
    store_word(record, 8, buttons);
    if (startInhibit != 0) {
        buttons &= 0xf7ffu;
        store_word(record, 8, buttons);
        startInhibit -= 1;
    }
    buttons = load_word(record, 8);

    store_float(record, 40, flag(buttons, 0x2000u));
    store_float(record, 44, flag(buttons, 0x8000u));
    store_float(record, 48, flag(buttons, 0x1000u));
    store_float(record, 52, flag(buttons, 0x4000u));
    store_float(record, 64, flag(buttons, 0x20u));
    store_float(record, 68, flag(buttons, 0x80u));
    store_float(record, 72, flag(buttons, 0x10u));
    store_float(record, 76, flag(buttons, 0x40u));
    const float scale = bits_to_float(0x3b808080u);
    store_float(record, 56, static_cast<float>(data[6]) * scale);
    store_float(record, 60, static_cast<float>(static_cast<int>(buttons & 1u)));
    store_float(record, 80, static_cast<float>(data[7]) * scale);
    store_float(record, 84, flag(buttons, 0x2u));

    processAnalogInput__FiiPfT2(data[4], data[5], reinterpret_cast<float*>(record + 28),
                                reinterpret_cast<float*>(record + 32));
    processAnalogInput__FiiPfT2(data[2], data[3], reinterpret_cast<float*>(record + 20),
                                reinterpret_cast<float*>(record + 24));

    const float high = bits_to_float(0x3f4cccccu);
    const float low = bits_to_float(0xbf4cccccu);
    float axis = load_float(record, 20);
    if (high < axis) store_word(record, 8, load_word(record, 8) | 0x80000u);
    if (axis < low) store_word(record, 8, load_word(record, 8) | 0x40000u);
    axis = load_float(record, 24);
    if (high < axis) store_word(record, 8, load_word(record, 8) | 0x20000u);
    if (axis < low) store_word(record, 8, load_word(record, 8) | 0x10000u);

    buttons = load_word(record, 8);
    const unsigned pressed = (buttons ^ previous) & buttons;
    store_word(record, 12, pressed);
    store_word(record, 16, pressed);
    if (buttons == 0u) {
        store_word(state, 4, 0u);
    } else if (buttons != load_word(state, 0)) {
        store_word(state, 4, 0u);
    } else {
        const int held = static_cast<int>(load_word(state, 4)) + 1;
        store_word(state, 4, static_cast<unsigned>(held));
        if (held >= 21 && (held & 3) == 0) store_word(record, 16, load_word(record, 8));
    }
    buttons = load_word(record, 8);

    if (padFilter != 0) {
        const unsigned mask = static_cast<unsigned>(padAcceptButton) | static_cast<unsigned>(padBackButton);
        buttons &= mask;
        store_word(record, 16, load_word(record, 16) & mask);
        store_word(record, 12, load_word(record, 12) & mask);
        static const int zeroed[] = {40, 24, 20, 32, 28, 84, 60, 80, 56, 76, 72, 68, 64, 52, 48, 44};
        for (int index = 0; index < 16; ++index) store_float(record, zeroed[index], 0.0f);
        store_word(record, 8, buttons);
    }
    store_word(state, 0, buttons);
}
