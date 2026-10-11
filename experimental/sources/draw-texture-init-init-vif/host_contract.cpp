#include <cstdio>
#include <cstring>

extern "C" unsigned* dmaVif0;
extern "C" unsigned* dmaVif1;
extern "C" void initVIF__Fv();

struct Event {
    const char* name;
    unsigned long a;
    unsigned long b;
    unsigned long c;
};
static Event events[24];
static int count;

static void push(const char* name, unsigned long a, unsigned long b, unsigned long c) {
    events[count].name = name;
    events[count].a = a;
    events[count].b = b;
    events[count].c = c;
    ++count;
}

extern "C" void norrath_vif_ctc2(unsigned value) { push("ctc2", value, 0, 0); }
extern "C" void norrath_store32(unsigned address, unsigned value) { push("store", address, value, 0); }
extern "C" void sceDmaSend(unsigned* channel, void* packet) {
    push("send", reinterpret_cast<unsigned long>(channel), reinterpret_cast<unsigned long>(packet), 0);
}
extern "C" void sceDmaSync(unsigned* channel, int mode, int timeout) {
    push("sync", reinterpret_cast<unsigned long>(channel), static_cast<unsigned>(mode), static_cast<unsigned>(timeout));
}
extern "C" void FlushCache(int mode) { push("flush", static_cast<unsigned>(mode), 0, 0); }
extern "C" void sceGsSyncPath(int mode, int timeout) {
    push("path", static_cast<unsigned>(mode), static_cast<unsigned>(timeout), 0);
}
extern "C" void sceGsSyncV(int mode) { push("syncv", static_cast<unsigned>(mode), 0, 0); }

int main() {
    unsigned vif0_channel = 0x11u;
    unsigned vif1_channel = 0x22u;
    dmaVif0 = &vif0_channel;
    dmaVif1 = &vif1_channel;
    count = 0;
    initVIF__Fv();
    const Event expect[] = {
        {"ctc2", 1024u, 0, 0},
        {"send", reinterpret_cast<unsigned long>(&vif1_channel), 0x0031b790u, 0},
        {"sync", reinterpret_cast<unsigned long>(&vif1_channel), 0, 0},
        {"send", reinterpret_cast<unsigned long>(&vif0_channel), 0x0031b510u, 0},
        {"sync", reinterpret_cast<unsigned long>(&vif0_channel), 0, 0},
        {"flush", 0, 0, 0},
        {"path", 0, 0, 0},
        {"store", 0x10003820u, 2u, 0},
        {"store", 0x10003c20u, 2u, 0},
        {"syncv", 0, 0, 0},
        {"syncv", 0, 0, 0},
        {"store", 0x1100ffb4u, 0x30ae4000u, 0},
        {"store", 0x1100ffb8u, 0x30be4000u, 0},
        {"store", 0x1100ff60u, 0x8000u, 0},
        {"store", 0x1100ff64u, 0, 0},
        {"store", 0x1100ff68u, 0, 0},
        {"store", 0x1100ff6cu, 0, 0},
    };
    if (count != 17) return 1;
    for (int index = 0; index < 17; ++index) {
        if (std::strcmp(events[index].name, expect[index].name) != 0) return 10 + index;
        if (events[index].a != expect[index].a || events[index].b != expect[index].b ||
            events[index].c != expect[index].c)
            return 40 + index;
    }
    dmaVif1 = &vif0_channel;
    count = 0;
    initVIF__Fv();
    if (events[1].a != reinterpret_cast<unsigned long>(&vif0_channel)) return 2;
    if (events[2].a != reinterpret_cast<unsigned long>(&vif0_channel)) return 3;
    std::puts("initVIF contract passed");
    return 0;
}
