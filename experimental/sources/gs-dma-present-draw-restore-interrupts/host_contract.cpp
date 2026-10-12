#define NORRATH_HOST_CONTRACT 1
#include "draw_restore_interrupts.cpp"

#include <cstdio>
#include <string>

extern "C" int vif0HID = 0;
extern "C" int vif1HID = 0;
extern "C" int gifHID = 0;

static unsigned channels[3];
static std::string log_text;
static unsigned long long stored64;
static unsigned stored32;
static unsigned stored32_address;

extern "C" unsigned* sceDmaGetChan(int channel) {
    log_text += "G" + std::to_string(channel);
    return &channels[channel];
}
extern "C" void FlushCache(int mode) {
    log_text += "F" + std::to_string(mode);
}
extern "C" int AddDmacHandler(int channel, void (*handler)(int), int next) {
    log_text += "A" + std::to_string(channel);
    if (handler != reloadDMA__Fi || next != 0) log_text += "!";
    return 20 + channel;
}
extern "C" void EnableDmac(int channel) {
    log_text += "E" + std::to_string(channel);
    if (channel == 0 && vif0HID != 20) log_text += "!";
    if (channel == 1 && vif1HID != 21) log_text += "!";
    if (channel == 2 && gifHID != 22) log_text += "!";
}
extern "C" int AddIntcHandler(int cause, int (*handler)(int), int next) {
    log_text += "I" + std::to_string(cause);
    if (handler != gsIntHandler__Fi || next != 0) log_text += "!";
    return 99;
}
extern "C" void EnableIntc(int cause) { log_text += "N" + std::to_string(cause); }
extern "C" void reloadDMA__Fi(int) {}
extern "C" int gsIntHandler__Fi(int) { return 0; }
extern "C" void norrath_store64(unsigned address, unsigned long long value) {
    if (address != 0x12001010u) log_text += "?";
    stored64 = value;
    log_text += "Q";
}
extern "C" void norrath_store32(unsigned address, unsigned value) {
    stored32_address = address;
    stored32 = value;
    log_text += "W";
}

int main() {
    channels[0] = 0x1u;
    channels[1] = 0x100u;
    channels[2] = 0x40u;
    log_text.clear();
    drawRestoreInterrupts__Fv();
    if (channels[0] != 0x41u || channels[1] != 0x140u || channels[2] != 0u) return 1;
    if (norrath_restored_vif0() != &channels[0]) return 2;
    if (norrath_restored_vif1() != &channels[1] || norrath_restored_gif() != &channels[2]) return 3;
    if (vif0HID != 20 || vif1HID != 21 || gifHID != 22) return 4;
    if (log_text != "G0F0G1F0G2F0A0E0A1E1A2E2I0N0QW") return 5;
    if (stored64 != 32000ull || stored32_address != 0x10003010u || stored32 != 4u) return 6;
    std::puts("draw restore interrupts contract passed");
    return 0;
}
