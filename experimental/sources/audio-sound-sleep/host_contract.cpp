#include "sound_sleep.cpp"

#include <cstdio>
#include <cstring>

static unsigned load_word(const unsigned char* address) {
    return address[0] | (address[1] << 8) | (address[2] << 16) | (address[3] << 24);
}

namespace {
unsigned long freed[4];
int free_count;
int rpc_count;
void* rpc_client;
int rpc_command;
int rpc_mode;
void* rpc_send;
int rpc_send_bytes;
void* rpc_receive;
int rpc_receive_bytes;
void* rpc_end_function;
void* rpc_end_parameter;
unsigned live_at_free[4][3];
}

extern "C" {
unsigned char norrath_sound_stream_heap_words_view[16];
unsigned char norrath_sound_stream_records_view[3 * 452];
unsigned char norrath_sound_rpc_client_view[4];
unsigned char norrath_sound_rpc_send_view[4];
unsigned char norrath_sound_rpc_receive_view[4];
}

extern "C" int sceSifFreeIopHeap(void* block) {
    if (free_count < 4) {
        for (int index = 0; index < 3; ++index)
            live_at_free[free_count][index] = load_word(norrath_sound_stream_heap_words_view + index * 4);
    }
    freed[free_count++] = reinterpret_cast<unsigned long>(block);
    return 7;
}

extern "C" int norrath_sound_call_rpc(void* client, int command, int mode, void* send, int send_length,
                                     void* receive, int receive_length, void* end_function,
                                     void* end_parameter) {
    ++rpc_count;
    rpc_client = client;
    rpc_command = command;
    rpc_mode = mode;
    rpc_send = send;
    rpc_send_bytes = send_length;
    rpc_receive = receive;
    rpc_receive_bytes = receive_length;
    rpc_end_function = end_function;
    rpc_end_parameter = end_parameter;
    return -5;
}

static void store_word(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

int main() {
    store_word(norrath_sound_stream_heap_words_view, 0);
    store_word(norrath_sound_stream_heap_words_view + 4, 0x20);
    store_word(norrath_sound_stream_heap_words_view + 8, 0);
    store_word(norrath_sound_stream_heap_words_view + 12, 0x99);
    std::memset(norrath_sound_stream_records_view, 0xab, sizeof norrath_sound_stream_records_view);
    soundSleep__Fv();
    if (free_count != 1 || freed[0] != 0x20ul) return 1;
    if (live_at_free[0][0] != 0u || live_at_free[0][1] != 0x20u || live_at_free[0][2] != 0u) return 14;
    if (load_word(norrath_sound_stream_heap_words_view) != 0) return 2;
    if (load_word(norrath_sound_stream_heap_words_view + 4) != 0) return 3;
    if (load_word(norrath_sound_stream_heap_words_view + 8) != 0) return 4;
    if (load_word(norrath_sound_stream_heap_words_view + 12) != 0x99u) return 5;
    for (int index = 0; index < 3; ++index) {
        const unsigned char* record = norrath_sound_stream_records_view + index * 452;
        if (load_word(record + 40) != 0xababababu) return 6;
        if (load_word(record + 44) != 0 || load_word(record + 48) != 0) return 7;
        if (load_word(record + 52) != 0xababababu) return 8;
    }
    if (rpc_count != 1 || rpc_command != 160 || rpc_mode != 0 || rpc_send_bytes != 0 ||
        rpc_receive_bytes != 64 || rpc_end_function != nullptr || rpc_end_parameter != nullptr)
        return 9;
    if (rpc_client != norrath_sound_rpc_client_view || rpc_send != norrath_sound_rpc_send_view ||
        rpc_receive != norrath_sound_rpc_receive_view)
        return 10;

    std::memset(norrath_sound_stream_heap_words_view, 0, 12);
    store_word(norrath_sound_stream_heap_words_view, 1);
    store_word(norrath_sound_stream_heap_words_view + 4, 2);
    store_word(norrath_sound_stream_heap_words_view + 8, 3);
    free_count = 0;
    soundSleep__Fv();
    if (free_count != 3 || freed[0] != 1ul || freed[1] != 2ul || freed[2] != 3ul) return 11;
    if (live_at_free[0][0] != 1u || live_at_free[0][1] != 2u || live_at_free[0][2] != 3u) return 12;
    if (live_at_free[1][0] != 0u || live_at_free[1][1] != 2u || live_at_free[1][2] != 3u) return 15;
    if (live_at_free[2][0] != 0u || live_at_free[2][1] != 0u || live_at_free[2][2] != 3u) return 16;
    if (rpc_count != 2) return 13;
    std::puts("sound sleep passed");
    return 0;
}
