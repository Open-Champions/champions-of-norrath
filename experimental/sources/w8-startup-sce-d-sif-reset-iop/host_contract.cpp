#include "sif_reset_iop.cpp"

#include <cstdio>
#include <cstring>

static char order[64][24];
static int steps;
static void note(const char* what) { std::snprintf(order[steps++], 24, "%s", what); }
static unsigned reg_values[16], reg_ids[16];
static int regs;
static int set_dma_result = 7;
static SifDmaData seen_transfer;
static unsigned char seen_packet[128];
static unsigned writeback_size;
static void* writeback_address;
static unsigned reg_value_80000000 = 0x01234560u;
static int notify_modes[2];

extern "C" void sceSifExecNotifyHandler(int use_system_table, unsigned argument) {
    if (argument != 0) note("notify-bad-arg");
    notify_modes[steps < 2 ? steps : 0] = use_system_table;
    note(use_system_table ? "notify1" : "notify0");
}
extern "C" void sceSifStopDma() { note("stopdma"); }
extern "C" unsigned sceSifGetReg(unsigned reg) { note("getreg"); return reg == 0x80000000u ? reg_value_80000000 : 0; }
extern "C" void sceSifSetReg(unsigned reg, unsigned value) {
    note("setreg");
    reg_ids[regs] = reg; reg_values[regs] = value; regs += 1;
}
extern "C" void sceSifWriteBackDCache(void* address, int size) {
    note("writeback");
    writeback_address = address; writeback_size = static_cast<unsigned>(size);
    std::memcpy(seen_packet, address, 128);
}
extern "C" int sceSifSetDma(SifDmaData* data, int count) {
    note("setdma");
    seen_transfer = *data;
    if (count != 1) note("bad-count");
    return set_dma_result;
}

int main() {
    std::memset(packet, 0xee, 8); /* leftover in word 0 and word 1 must not survive */
    packet[3] = 0xabcdef01u;      /* word 3 is not written */
    if (sceSifResetIop("rom0:UDNL cdrom0:\\X.IMG;1", 0x55) != 1) return 1;
    static const char* expect[] = {"notify0", "notify1", "stopdma", "getreg", "writeback", "setreg", "setdma",
                                   "setreg", "setreg", "setreg", "setreg"};
    if (steps != 11) return 2;
    for (int i = 0; i < 11; ++i)
        if (std::strcmp(order[i], expect[i]) != 0) return 3;
    const unsigned want_ids[] = {4, 4, 4, 0x80000002u, 0x80000000u};
    const unsigned want_vals[] = {0x40000, 0x10000, 0x20000, 0, 0};
    if (regs != 5) return 4;
    for (int i = 0; i < 5; ++i)
        if (reg_ids[i] != want_ids[i] || reg_values[i] != want_vals[i]) return 5;
    if (writeback_address != packet || writeback_size != 104) return 6;
    if (seen_transfer.source != packet || seen_transfer.destination != reg_value_80000000 ||
        seen_transfer.size != 104 || seen_transfer.attribute != 68)
        return 7;
    const char text[] = "rom0:UDNL cdrom0:\\X.IMG;1";
    unsigned word[6];
    std::memcpy(word, seen_packet, 24);
    if (word[0] != 104 || word[1] != 0 || word[2] != 0x80000003u || word[3] != 0xabcdef01u) return 8;
    if (word[4] != sizeof text - 1 || word[5] != 0x55) return 9;
    if (std::memcmp(seen_packet + 24, text, sizeof text - 1) != 0) return 10; /* no terminator copied */
    if (seen_packet[24 + sizeof text - 1] != 0) return 11;
    /* failed transfer: stops after the first register write */
    steps = 0; regs = 0; set_dma_result = 0;
    if (sceSifResetIop("", -1) != 0) return 12;
    if (regs != 1 || steps != 7) return 13;
    std::memcpy(word, seen_packet, 24);
    if (word[4] != 0 || word[5] != 0xffffffffu) return 14;
    std::puts("sif reset iop contract passed");
    return 0;
}
