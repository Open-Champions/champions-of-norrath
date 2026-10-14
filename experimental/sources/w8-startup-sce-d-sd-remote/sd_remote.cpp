/* Observed sceSdRemote at 0x002e0478, 904 bytes. The first argument is a
 * flag, the second a command number, and six more argument words follow in
 * the remaining argument registers. The packet sbuff (0x008e5a00, 64 bytes)
 * is filled: word 0 with its own address and words 1..6 with those six
 * argument words. The flag selects how the RPC is issued. A zero flag uses
 * mode 1 with the end function stored at sceSdEndFunc (0x00446020); any
 * other flag uses mode 0 and no end function. When there is an end function,
 * $gp is swapped for the one stored at sceSdEndGp (0x00446024) around the
 * call and restored after it. The client is sceSd_gCd (0x008e5a40). Every
 * call is sceSifCallRpc with that client, the function number below, the mode,
 * then send pointer and size, receive pointer and size, the end function and
 * an end parameter. A word of the packet used as a pointer is a 32-bit EE
 * address.
 *
 * By command (c is the second argument):
 * - 0x8160: the transfer-interrupt tables are updated first. Packet word 1 zero
 *   selects index 0, otherwise index 1. The old function word is kept, then the
 *   function table[index] = word 2, argument table[index] = word 3 and
 *   gp table[index] = the current $gp. The call is then made as in the default
 *   case, and the old function word is the result. 0x8170 does the same with
 *   index 2 and words 1 and 2 (function and argument), the old function word
 *   again being the result.
 * - 0x8130, 0x81a0, 0x81b0: function number c | word 1, send pointer word 2,
 *   send 64, receive the packet, 64, end parameter the packet. The result is
 *   packet word 0 after the call.
 * - 0x8140: function number word 1 | 0x8140, send and receive pointer word 2
 *   (also the end parameter), 64 bytes each. The result is 0.
 * - 0x81c0, 0x81d0: the halfword at word-1-pointer + 2 receives the low half
 *   of word 2; 0x81d0 also stores word 5 at word-1-pointer + 4. The call uses
 *   function number c, send pointer word 1, send size (word 2 + 1) * 8,
 *   receive pointer word 3 and size word 4 (when word 3 is zero: the local
 *   result word and 4), end parameter the packet. When word 3 is nonzero the
 *   result is the int it points to; otherwise it is the local result word,
 *   which starts at 0 and is the receive area.
 * - 0x9000 through 0x90f0: function number c, send pointer word 1, send size
 *   word 2, receive the packet, 16, end parameter the packet. The result is
 *   packet word 0 after the call.
 * - any other command: send and receive the packet, 64 and 16 bytes, end
 *   parameter the packet. The result is packet word 0 after the call.
 *
 * What the command numbers mean is not established here.
 */
extern "C" {
extern unsigned norrath_sound_remote_packet_view[];
extern unsigned char norrath_sound_remote_rpc_client_view[];
extern unsigned norrath_sound_remote_end_function_word_view[];
extern unsigned norrath_sound_remote_end_gp_word_view[];
extern unsigned norrath_sound_remote_interrupt_functions_view[];
extern unsigned norrath_sound_remote_interrupt_arguments_view[];
extern unsigned norrath_sound_remote_interrupt_gps_view[];

int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
}

#if defined(__mips__) && !defined(NORRATH_HOST_CONTRACT)
static unsigned read_gp() {
    unsigned value;
    __asm__ volatile("move %0,$gp" : "=r"(value));
    return value;
}
static void write_gp(unsigned value) {
    __asm__ volatile("move $gp,%0" : : "r"(value));
}
static void* as_pointer(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}
static unsigned as_word(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
#else
extern "C" unsigned norrath_host_gp;
extern "C" void* norrath_host_pointer(unsigned word);
extern "C" unsigned norrath_host_word(const void* pointer);
static unsigned read_gp() { return norrath_host_gp; }
static void write_gp(unsigned value) { norrath_host_gp = value; }
static void* as_pointer(unsigned word) { return norrath_host_pointer(word); }
static unsigned as_word(const void* pointer) { return norrath_host_word(pointer); }
#endif

extern "C" int sceSdRemote(int flag, unsigned command, ...) {
    __builtin_va_list arguments;
    __builtin_va_start(arguments, command);
    unsigned* const packet = norrath_sound_remote_packet_view;
    packet[0] = as_word(packet);
    for (int index = 1; index <= 6; ++index) packet[index] = __builtin_va_arg(arguments, unsigned);
    __builtin_va_end(arguments);

    unsigned mode = 0;
    unsigned end_word = 0;
    if (flag == 0) {
        mode = 1;
        end_word = norrath_sound_remote_end_function_word_view[0];
    }
    void* const end_function = as_pointer(end_word);
    unsigned previous = 0;
    if (command == 0x8160u) {
        const int index = packet[1] != 0 ? 1 : 0;
        previous = norrath_sound_remote_interrupt_functions_view[index];
        norrath_sound_remote_interrupt_functions_view[index] = packet[2];
        norrath_sound_remote_interrupt_arguments_view[index] = packet[3];
        norrath_sound_remote_interrupt_gps_view[index] = read_gp();
    } else if (command == 0x8170u) {
        previous = norrath_sound_remote_interrupt_functions_view[2];
        norrath_sound_remote_interrupt_functions_view[2] = packet[1];
        norrath_sound_remote_interrupt_arguments_view[2] = packet[2];
        norrath_sound_remote_interrupt_gps_view[2] = read_gp();
    }
    volatile int result = 0;
    unsigned saved_gp = 0;
    void* const client = norrath_sound_remote_rpc_client_view;
    if (command == 0x8130u || command == 0x81a0u || command == 0x81b0u) {
        if (end_word != 0) {
            saved_gp = read_gp();
            write_gp(norrath_sound_remote_end_gp_word_view[0]);
        }
        sceSifCallRpc(client, command | packet[1], mode, as_pointer(packet[2]), 64, packet, 64, end_function, packet);
        if (end_word != 0) write_gp(saved_gp);
        result = static_cast<int>(packet[0]);
    } else if (command == 0x8140u) {
        if (end_word != 0) {
            saved_gp = read_gp();
            write_gp(norrath_sound_remote_end_gp_word_view[0]);
        }
        void* const area = as_pointer(packet[2]);
        sceSifCallRpc(client, packet[1] | 0x8140u, mode, area, 64, area, 64, end_function, area);
        if (end_word != 0) write_gp(saved_gp);
    } else if (command == 0x81c0u || command == 0x81d0u) {
        unsigned short* const target = static_cast<unsigned short*>(as_pointer(packet[1]));
        target[1] = static_cast<unsigned short>(packet[2]);
        if (command == 0x81d0u) *reinterpret_cast<unsigned*>(reinterpret_cast<unsigned char*>(target) + 4) = packet[5];
        void* receive = as_pointer(packet[3]);
        int receive_size = static_cast<int>(packet[4]);
        if (packet[3] == 0) {
            receive = const_cast<int*>(&result);
            receive_size = 4;
        }
        if (end_word != 0) {
            saved_gp = read_gp();
            write_gp(norrath_sound_remote_end_gp_word_view[0]);
        }
        sceSifCallRpc(client, command, mode, as_pointer(packet[1]), static_cast<int>((packet[2] + 1u) << 3), receive,
                      receive_size, end_function, packet);
        if (end_word != 0) write_gp(saved_gp);
        if (packet[3] != 0) result = *static_cast<int*>(as_pointer(packet[3]));
    } else if (command - 0x9000u < 241u) {
        if (end_word != 0) {
            saved_gp = read_gp();
            write_gp(norrath_sound_remote_end_gp_word_view[0]);
        }
        sceSifCallRpc(client, command, mode, as_pointer(packet[1]), static_cast<int>(packet[2]), packet, 16, end_function,
                      packet);
        if (end_word != 0) write_gp(saved_gp);
        result = static_cast<int>(packet[0]);
    } else {
        if (end_word != 0) {
            saved_gp = read_gp();
            write_gp(norrath_sound_remote_end_gp_word_view[0]);
        }
        sceSifCallRpc(client, command, mode, packet, 64, packet, 16, end_function, packet);
        if (end_word != 0) write_gp(saved_gp);
        result = static_cast<int>(packet[0]);
    }
    if (command == 0x8160u || command == 0x8170u) result = static_cast<int>(previous);
    return result;
}
