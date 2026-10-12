#define NORRATH_HOST_CONTRACT 1
#include "sif_init_cmd.cpp"

#include <cstdio>
#include <cstring>

unsigned norrath_sound_command_initialized_word_view[1];
alignas(64) unsigned norrath_sound_command_packet_buffer_view[32];
alignas(64) unsigned norrath_sound_command_send_buffer_view[16];
unsigned norrath_sound_command_initialization_packet_view[5];
unsigned norrath_sound_command_dma_handle_view[1];
unsigned norrath_sound_command_state_view[10];
unsigned norrath_sound_command_system_handlers_view[96];
unsigned norrath_sound_system_register_words_view[32];
volatile unsigned norrath_host_register_1000e010;
volatile unsigned norrath_host_register_1000c000;

static char log[64][32];
static int steps;
static void note(const char* s) { std::snprintf(log[steps++], 32, "%s", s); }
static unsigned long long reg_values[8];   /* sceSifGetReg answers by id */
static int get_reg4_polls_until_ready = 0;
static int get_reg4_calls;
static unsigned set_ids[4], set_vals[4];
static int sets;
static unsigned send_cmd[2];
static void* send_packet[2];
static int sends;
static unsigned packet_at_send[2][5];
static int dma_args[3];

extern "C" {
int DIntr() { note("di"); return 1; }
void EIntr() { note("ei"); }
void FlushCache(int mode) { note("flush"); if (mode != 0) note("flush-bad"); }
void sceSifSetDChain() { note("dchain"); }
int AddDmacHandler(int channel, int (*handler)(), int next) {
    note("adddmac");
    dma_args[0] = channel; dma_args[1] = handler == norrath_sound_command_dma_handler; dma_args[2] = next;
    return 0x1357;
}
int EnableDmac(int channel) { note("enabledmac"); if (channel != 5) note("bad-chan"); return 0; }
unsigned long long sceSifGetReg(unsigned reg) {
    note("getreg");
    if (reg == 0x80000000u) return reg_values[0];
    if (reg == 4) { get_reg4_calls += 1; return get_reg4_calls > get_reg4_polls_until_ready ? 0x20000 : 0; }
    if (reg == 2) return reg_values[2];
    return 0;
}
void sceSifSetReg(unsigned reg, unsigned value) { note("setreg"); set_ids[sets] = reg; set_vals[sets] = value; sets += 1; }
int sceSifSendCmd(unsigned command, void* packet, int packet_size, void* extra, void* destination, int size) {
    note("send");
    if (packet_size != 20 || extra || destination || size) note("send-bad");
    send_cmd[sends] = command; send_packet[sends] = packet;
    std::memcpy(packet_at_send[sends], packet, 20);
    sends += 1;
    return 0;
}
void norrath_sound_command_change_address(void*, void*) {}
void norrath_sound_command_set_system_register(void*, void*) {}
int norrath_sound_command_dma_handler() { return 0; }
}

static unsigned low(const void* p) { return word_of(p); }

int main() {
    /* already initialized: only the interrupt pair runs */
    norrath_sound_command_initialized_word_view[0] = 1;
    sceSifInitCmd();
    if (steps != 2 || std::strcmp(log[0], "di") || std::strcmp(log[1], "ei")) return 1;
    if (norrath_sound_command_state_view[0] != 0) return 2;

    /* fresh init, IOP side already answers */
    steps = 0;
    norrath_sound_command_initialized_word_view[0] = 0;
    for (unsigned& w : norrath_sound_command_system_handlers_view) w = 0xdddddddd;
    for (unsigned& w : norrath_sound_system_register_words_view) w = 0xeeeeeeee;
    for (unsigned& w : norrath_sound_command_state_view) w = 0xcccccccc;
    norrath_sound_command_initialization_packet_view[3] = 0x77777777;
    norrath_host_register_1000e010 = 0x20;
    norrath_host_register_1000c000 = 0;
    reg_values[0] = 0x0000000100000000ull; /* nonzero only in the high half: all 64 bits are tested */
    sceSifInitCmd();
    static const char* expect[] = {"di", "ei", "flush", "dchain", "adddmac", "enabledmac"};
    (void)expect;
    if (norrath_sound_command_initialized_word_view[0] != 1) return 3;
    unsigned* st = norrath_sound_command_state_view;
    if (st[0] != (low(norrath_sound_command_packet_buffer_view) | 0x20000000u)) return 4;
    if (st[1] != (low(norrath_sound_command_send_buffer_view) | 0x20000000u)) return 5;
    if (st[2] != 0 || st[4] != 32 || st[5] != 0 || st[6] != 0) return 6; /* low half of the register is stored */
    if (st[3] != low(norrath_sound_command_system_handlers_view) || st[7] != low(norrath_sound_system_register_words_view)) return 7;
    if (st[8] != 0xcccccccc || st[9] != 0xcccccccc) return 8;
    for (int i = 0; i < 32; ++i) {
        unsigned* rec = norrath_sound_command_system_handlers_view + i * 3;
        if (i == 0) {
            if (rec[0] != word_of(reinterpret_cast<const void*>(norrath_sound_command_change_address)) || rec[1] != low(st)) return 9;
        } else if (i == 1) {
            if (rec[0] != word_of(reinterpret_cast<const void*>(norrath_sound_command_set_system_register)) || rec[1] != low(st)) return 10;
        } else if (rec[0] != 0 || rec[1] != 0) return 11;
        if (rec[2] != 0xdddddddd) return 12; /* third word of each record is untouched */
    }
    for (unsigned w : norrath_sound_system_register_words_view) if (w != 0) return 13;
    if (norrath_host_register_1000e010 != 32) return 14;
    if (steps < 7) return 15;
    if (std::strcmp(log[0], "di") || std::strcmp(log[1], "ei") || std::strcmp(log[2], "flush")) return 16; /* enable, flush, then device setup */
    if (norrath_sound_command_dma_handle_view[0] != 0x1357 || dma_args[0] != 5 || !dma_args[1] || dma_args[2] != 0) return 17;
    bool saw_dchain = false;
    for (int i = 0; i < steps; ++i) if (!std::strcmp(log[i], "dchain")) saw_dchain = true;
    if (!saw_dchain) return 18;
    if (sends != 1 || send_cmd[0] != 0x80000000u || send_packet[0] != norrath_sound_command_initialization_packet_view) return 19;
    if (packet_at_send[0][4] != low(norrath_sound_command_packet_buffer_view)) return 20;
    if (packet_at_send[0][3] != 0x77777777) return 21; /* fast path leaves packet word 3 */
    if (sets != 0) return 22;
    if (std::strcmp(log[steps - 1], "send")) return 23;

    /* second path: IOP side not yet ready, status bit 5 clear, DMA control bit 8 set */
    steps = 0; sends = 0; sets = 0; get_reg4_calls = 0; get_reg4_polls_until_ready = 3;
    norrath_sound_command_initialized_word_view[0] = 0;
    norrath_host_register_1000e010 = 0;
    norrath_host_register_1000c000 = 0x100;
    reg_values[0] = 0;
    reg_values[2] = 0x00fe0000ull;
    sceSifInitCmd();
    if (norrath_host_register_1000e010 != 0) return 24;
    for (int i = 0; i < steps; ++i) if (!std::strcmp(log[i], "dchain")) return 25;
    if (get_reg4_calls != 4) return 26; /* polled until bit 17 was set */
    if (st[2] != 0x00fe0000u) return 27;
    if (sets != 2 || set_ids[0] != 0x80000000u || set_vals[0] != 0x00fe0000u || set_ids[1] != 0x80000001u || set_vals[1] != low(st)) return 28;
    if (sends != 1 || send_cmd[0] != 0x80000002u) return 29;
    if (packet_at_send[0][4] != low(norrath_sound_command_packet_buffer_view) || packet_at_send[0][3] != 0) return 30;
    if (std::strcmp(log[steps - 1], "send")) return 31;
    std::puts("sif init cmd contract passed");
    return 0;
}
