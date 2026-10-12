/* Observed sceSifInitCmd at 0x002d71a8, 640 bytes. Interrupts are disabled
 * (DIntr) first. When the word _cmd_init_check (0x00444964) is nonzero,
 * interrupts are re-enabled and the function returns. Otherwise:
 *
 * - _cmd_init_check is set to 1 and the command state (0x008dea18) is filled
 *   in: word 0 is the packet buffer address ORed with 0x20000000, word 1 the
 *   send buffer address ORed with 0x20000000, word 2 zero, word 3 the system
 *   handler table, word 4 is 32, words 5 and 6 are zero, and word 7 the
 *   soft-register array.
 * - Words 0 and 1 of each of 32 twelve-byte records of the handler table are
 *   cleared, then 32 soft-register words, highest first. Word 2 of each
 *   record is not written.
 * - Record 0 of the table gets the change-address handler and the state
 *   pointer; record 1 gets the set-system-register handler and the state
 *   pointer. Interrupts are re-enabled and FlushCache(0) runs.
 * - When bit 5 of the word at 0x1000e010 is set, 32 is written back to it.
 *   When bit 8 of the word at 0x1000c000 is clear, sceSifSetDChain runs.
 * - AddDmacHandler(5, command interrupt handler, 0) stores its result in the
 *   handle word (0x008dea14) and EnableDmac(5) follows.
 * - sceSifGetReg(0x80000000) is stored in state word 2. When it is nonzero
 *   (all 64 bits are tested), word 4 of the 20-byte start packet (0x008dea00)
 *   is set to the packet buffer address and sceSifSendCmd(0x80000000,
 *   packet, 20, 0, 0, 0) is the last action. When it is zero, the function
 *   waits until bit 17 of sceSifGetReg(4) is set, stores sceSifGetReg(2) in
 *   state word 2, sets registers 0x80000000 to that value and 0x80000001 to
 *   the state address, sets packet words 4 to the packet buffer address and 3
 *   to zero, and ends with sceSifSendCmd(0x80000002, packet, 20, 0, 0, 0).
 *
 * The original is a tail call into the send function, so its result is what
 * the original leaves in $v0; no result is claimed here. What the registers
 * and packets mean is not established.
 */
extern "C" {
extern unsigned norrath_sound_command_initialized_word_view[];
extern unsigned norrath_sound_command_packet_buffer_view[];
extern unsigned norrath_sound_command_send_buffer_view[];
extern unsigned norrath_sound_command_initialization_packet_view[];
extern unsigned norrath_sound_command_dma_handle_view[];
extern unsigned norrath_sound_command_state_view[];
extern unsigned norrath_sound_command_system_handlers_view[];
extern unsigned norrath_sound_system_register_words_view[];

int DIntr();
void EIntr();
void FlushCache(int mode);
void sceSifSetDChain();
int AddDmacHandler(int channel, int (*handler)(), int next);
int EnableDmac(int channel);
unsigned long long sceSifGetReg(unsigned reg);
void sceSifSetReg(unsigned reg, unsigned value);
int sceSifSendCmd(unsigned command, void* packet, int packet_size, void* extra, void* destination, int size);

void norrath_sound_command_change_address(void* packet, void* parameter);
void norrath_sound_command_set_system_register(void* packet, void* parameter);
int norrath_sound_command_dma_handler();

#if defined(NORRATH_HOST_CONTRACT)
extern volatile unsigned norrath_host_register_1000e010;
extern volatile unsigned norrath_host_register_1000c000;
static volatile unsigned* dma_status_register() { return &norrath_host_register_1000e010; }
static volatile unsigned* dma_control_register() { return &norrath_host_register_1000c000; }
#else
static volatile unsigned* dma_status_register() { return reinterpret_cast<volatile unsigned*>(0x1000e010u); }
static volatile unsigned* dma_control_register() { return reinterpret_cast<volatile unsigned*>(0x1000c000u); }
#endif

static unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

void sceSifInitCmd() {
    DIntr();
    if (norrath_sound_command_initialized_word_view[0] != 0) {
        EIntr();
        return;
    }
    unsigned* const state = norrath_sound_command_state_view;
    unsigned* const handlers = norrath_sound_command_system_handlers_view;
    unsigned* const registers = norrath_sound_system_register_words_view;
    norrath_sound_command_initialized_word_view[0] = 1;
    state[0] = word_of(norrath_sound_command_packet_buffer_view) | 0x20000000u;
    state[7] = word_of(registers);
    state[1] = word_of(norrath_sound_command_send_buffer_view) | 0x20000000u;
    state[4] = 32;
    state[2] = 0;
    state[3] = word_of(handlers);
    state[5] = 0;
    state[6] = 0;
    for (unsigned* record = handlers; record < handlers + 96; record += 3) {
        record[0] = 0;
        record[1] = 0;
    }
    for (int index = 31; index >= 0; --index) registers[index] = 0;
    handlers[0] = word_of(reinterpret_cast<const void*>(norrath_sound_command_change_address));
    handlers[3] = word_of(reinterpret_cast<const void*>(norrath_sound_command_set_system_register));
    handlers[4] = word_of(state);
    handlers[1] = word_of(state);
    EIntr();
    FlushCache(0);
    volatile unsigned* const dma_status = dma_status_register();
    if (*dma_status & 0x20u) *dma_status = 32;
    if ((*dma_control_register() & 0x100u) == 0) sceSifSetDChain();
    norrath_sound_command_dma_handle_view[0] =
        static_cast<unsigned>(AddDmacHandler(5, norrath_sound_command_dma_handler, 0));
    EnableDmac(5);
    unsigned* const packet = norrath_sound_command_initialization_packet_view;
    const unsigned long long remote = sceSifGetReg(0x80000000u);
    state[2] = static_cast<unsigned>(remote);
    if (remote != 0) {
        packet[4] = word_of(norrath_sound_command_packet_buffer_view);
        sceSifSendCmd(0x80000000u, packet, 20, 0, 0, 0);
        return;
    }
    while ((sceSifGetReg(4) & 0x20000u) == 0) {
    }
    const unsigned address = static_cast<unsigned>(sceSifGetReg(2));
    state[2] = address;
    sceSifSetReg(0x80000000u, address);
    sceSifSetReg(0x80000001u, word_of(state));
    packet[4] = word_of(norrath_sound_command_packet_buffer_view);
    packet[3] = 0;
    sceSifSendCmd(0x80000002u, packet, 20, 0, 0, 0);
}
}
