/* soundSleep__Fv at 0x0024b9e8 is 180 bytes. It is not norrath_sound_restore.
 *
 * The first loop visits the three words at streamAllocation (0x007dd280).
 * A nonzero word is passed to sceSifFreeIopHeap and then cleared. A zero
 * word is left unchanged. The fourth byte before stream is not accessed.
 *
 * The second loop clears the buffer and length words, at offsets 44 and 48,
 * in each of the three 452-byte stream records.
 *
 * The call is sceSifCallRpc, already authored as norrath_sound_call_rpc:
 * client gCd, command 160, mode 0, send sbuffin, send length 0, receive
 * sbuff, receive length 64, and a null end function and parameter. The
 * return value is ignored. Two nops in the record loop are not reproduced.
 */
extern "C" {
extern unsigned char norrath_sound_stream_heap_words_view[];
extern unsigned char norrath_sound_stream_records_view[];
extern unsigned char norrath_sound_rpc_client_view[];
extern unsigned char norrath_sound_rpc_send_view[];
extern unsigned char norrath_sound_rpc_receive_view[];

int sceSifFreeIopHeap(void* block);
int norrath_sound_call_rpc(void* client, int command, int mode, void* send, int send_bytes,
                           void* receive, int receive_bytes, void* end_function,
                           void* end_parameter);

static unsigned read_word(const volatile unsigned char* address) {
    return unsigned(address[0]) | (unsigned(address[1]) << 8) | (unsigned(address[2]) << 16) |
           (unsigned(address[3]) << 24);
}

static void write_word(volatile unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

void soundSleep__Fv() {
    volatile unsigned char* heap = norrath_sound_stream_heap_words_view;
    for (int index = 0; index < 3; ++index) {
        volatile unsigned char* word = heap + index * 4;
        const unsigned block = read_word(word);
        if (block != 0u) {
            sceSifFreeIopHeap(reinterpret_cast<void*>(static_cast<unsigned long>(block)));
            write_word(word, 0u);
        }
    }

    volatile unsigned char* record = norrath_sound_stream_records_view;
    for (int index = 0; index < 3; ++index) {
        write_word(record + 44, 0u);
        write_word(record + 48, 0u);
        record += 452;
    }

    norrath_sound_call_rpc(norrath_sound_rpc_client_view, 160, 0, norrath_sound_rpc_send_view, 0,
                           norrath_sound_rpc_receive_view, 64, nullptr, nullptr);
}
}
