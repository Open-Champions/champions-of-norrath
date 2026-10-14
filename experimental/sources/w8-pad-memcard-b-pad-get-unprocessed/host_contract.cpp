#include "pad_get_unprocessed.cpp"

#include <cstdio>
#include <cstring>
#include <string>

signed char tapCount[2];
unsigned char rdata[128];
signed char needActAlign[4];
unsigned char actAlignData[6] = {0, 1, 0xff, 0xff, 0xff, 0xff};

static std::string log_text;
static int read_result;
static int press_result;
static unsigned char packet[32];

static void note(const char* name, int port, int slot, int extra = -1) {
    log_text += name;
    log_text += '(' + std::to_string(port) + ',' + std::to_string(slot);
    if (extra >= 0) log_text += ',' + std::to_string(extra);
    log_text += ')';
}

int scePadRead(int port, int slot, unsigned char* data) {
    note("read", port, slot, static_cast<int>((data - rdata) / 32));
    std::memcpy(data, packet, 32);
    return read_result;
}
int scePadSetMainMode(int port, int slot, int offs, int lock) {
    note("main", port, slot, offs * 10 + lock);
    return 1;
}
int scePadSetActAlign(int port, int slot, unsigned char* data) {
    note("align", port, slot, data == actAlignData ? 1 : 0);
    return 1;
}
int scePadInfoPressMode(int port, int slot) {
    note("info", port, slot);
    return press_result;
}
int scePadEnterPressMode(int port, int slot) {
    note("enter", port, slot);
    return 1;
}
void* memset(void* destination, int value, unsigned long size) {
    unsigned char* bytes = static_cast<unsigned char*>(destination);
    for (unsigned long index = 0; index < size; ++index) bytes[index] = static_cast<unsigned char>(value);
    return destination;
}

static void setup(unsigned char status) {
    tapCount[0] = 1;
    tapCount[1] = 2;
    for (int index = 0; index < 4; ++index) needActAlign[index] = 0;
    for (int index = 0; index < 32; ++index) packet[index] = static_cast<unsigned char>(0x40 + index);
    packet[1] = status;
    packet[2] = 0xf0;
    packet[3] = 0x0f;
    read_result = 1;
    press_result = 0;
    log_text.clear();
}
static bool default_out(const unsigned char* out) {
    return out[0] == 0 && out[1] == 0 && out[2] == 0x80 && out[3] == 0x80 && out[4] == 0x80 &&
           out[5] == 0x80 && out[6] == 0 && out[7] == 0;
}

int main() {
    alignas(2) unsigned char out[8];

    // Mode 0x79 data path, second pad on port 1 slot 1 (index 2).
    setup(0x79);
    std::memset(out, 0xee, 8);
    if (padGetUnprocessedData__FiP18padUnprocessedData(2, out) != 1) return 1;
    if (log_text != "read(1,1,2)") return 2;
    // Buttons are the complement of byte2<<8|byte3 = ~0xf00f = 0x0ff0.
    if (out[0] != 0xf0 || out[1] != 0x0f) return 3;
    if (out[4] != 0x44 || out[5] != 0x45 || out[2] != 0x46 || out[3] != 0x47) return 4;
    if (out[6] != 0x50 || out[7] != 0x51) return 5;

    // Index 0 is port 0 slot 0 and reads the first buffer.
    setup(0x79);
    if (padGetUnprocessedData__FiP18padUnprocessedData(0, out) != 1 || log_text != "read(0,0,0)") return 6;

    // Slot at or beyond tapCount[port]: no read, default output.
    setup(0x79);
    if (padGetUnprocessedData__FiP18padUnprocessedData(3, out) != 0) return 7;
    if (!log_text.empty() || !default_out(out)) return 8;

    // Failed read: default output, no further calls.
    setup(0x79);
    read_result = 0;
    if (padGetUnprocessedData__FiP18padUnprocessedData(0, out) != 0 || !default_out(out)) return 9;
    if (log_text != "read(0,0,0)") return 10;
    setup(0x79);
    read_result = -1;
    if (padGetUnprocessedData__FiP18padUnprocessedData(0, out) != 0 || !default_out(out)) return 11;

    // Wrong device mode: set main mode, request alignment, return 0.
    setup(0x41);
    if (padGetUnprocessedData__FiP18padUnprocessedData(1, out) != 0) return 12;
    if (log_text != "read(1,0,1)main(1,0,13)" || needActAlign[1] != 1 || !default_out(out)) return 13;

    // Pending alignment: send the actuator data once and clear the flag.
    setup(0x73);
    needActAlign[1] = 1;
    if (padGetUnprocessedData__FiP18padUnprocessedData(1, out) != 0) return 14;
    if (log_text != "read(1,0,1)align(1,0,1)" || needActAlign[1] != 0 || !default_out(out)) return 15;
    needActAlign[1] = static_cast<signed char>(0x80);
    log_text.clear();
    if (padGetUnprocessedData__FiP18padUnprocessedData(1, out) != 0 || needActAlign[1] != 0) return 16;

    // Status other than 0x79 with press mode info 1 enters press mode.
    setup(0x73);
    press_result = 1;
    if (padGetUnprocessedData__FiP18padUnprocessedData(0, out) != 0) return 17;
    if (log_text != "read(0,0,0)info(0,0)enter(0,0)" || !default_out(out)) return 18;

    // Any other press mode result continues to the data path.
    setup(0x73);
    press_result = 0;
    if (padGetUnprocessedData__FiP18padUnprocessedData(0, out) != 1) return 19;
    if (log_text != "read(0,0,0)info(0,0)" || out[0] != 0xf0) return 20;
    setup(0x73);
    press_result = 2;
    if (padGetUnprocessedData__FiP18padUnprocessedData(0, out) != 1) return 21;

    std::puts("pad get unprocessed passed");
    return 0;
}
