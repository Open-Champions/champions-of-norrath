// Host contract for padInit__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" void padInit__Fv();
extern "C" unsigned char pad_dma_buf[2560] = {};
extern "C" unsigned char pad[352] = {};
extern "C" unsigned char stateData[32] = {};

namespace {
int failures = 0;
int events = 0;
char log_names[16][32];
int log_ports[16];
int log_slots[16];
void* log_buffers[16];

void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

void record(const char* name, int port, int slot, void* buffer) {
    expect(events < 16, "event capacity");
    if (events >= 16) return;
    std::snprintf(log_names[events], sizeof log_names[events], "%s", name);
    log_ports[events] = port;
    log_slots[events] = slot;
    log_buffers[events] = buffer;
    ++events;
}
}

extern "C" void scePadInit(int mode) { record("scePadInit", mode, -1, nullptr); }
extern "C" void sceMtapInit() { record("sceMtapInit", -1, -1, nullptr); }
extern "C" void scePadPortOpen(int port, int slot, void* buffer) {
    record("scePadPortOpen", port, slot, buffer);
}
extern "C" void sceMtapPortOpen(int port) { record("sceMtapPortOpen", port, -1, nullptr); }
extern "C" void padInitPadIndexMap__Fv() { record("padInitPadIndexMap", -1, -1, nullptr); }

int main() {
    std::memset(pad, 0xa5, sizeof pad);
    std::memset(stateData, 0x5a, sizeof stateData);
    std::memset(pad_dma_buf, 0x3c, sizeof pad_dma_buf);
    padInit__Fv();

    expect(events == 15, "event count");
    expect(std::strcmp(log_names[0], "scePadInit") == 0 && log_ports[0] == 0, "pad init mode");
    expect(std::strcmp(log_names[1], "sceMtapInit") == 0, "mtap init");
    expect(std::strcmp(log_names[2], "scePadPortOpen") == 0 && log_ports[2] == 0 && log_slots[2] == 0, "port 0 slot 0");
    expect(log_buffers[2] == pad_dma_buf, "port 0 buffer");
    expect(std::strcmp(log_names[3], "sceMtapPortOpen") == 0 && log_ports[3] == 0, "mtap port 0");
    expect(std::strcmp(log_names[4], "scePadPortOpen") == 0 && log_ports[4] == 1 && log_slots[4] == 0, "port 1 slot 0");
    expect(log_buffers[4] == pad_dma_buf + 1280, "port 1 buffer");
    expect(std::strcmp(log_names[5], "sceMtapPortOpen") == 0 && log_ports[5] == 1, "mtap port 1");
    int cursor = 6;
    for (int port = 0; port < 2; ++port) {
        for (int slot = 1; slot < 5; ++slot, ++cursor) {
            expect(std::strcmp(log_names[cursor], "scePadPortOpen") == 0, "slot open name");
            expect(log_ports[cursor] == port && log_slots[cursor] == slot, "slot open index");
            expect(log_buffers[cursor] == pad_dma_buf + port * 1280 + slot * 256, "slot buffer");
        }
    }
    expect(cursor == 14, "slot open count");
    expect(std::strcmp(log_names[14], "padInitPadIndexMap") == 0, "index map last");
    for (unsigned char byte : pad) expect(byte == 0, "pad cleared");
    for (unsigned char byte : stateData) expect(byte == 0, "state cleared");
    for (unsigned char byte : pad_dma_buf) expect(byte == 0x3c, "dma buffer untouched");
    if (failures != 0) {
        std::fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    return 0;
}
