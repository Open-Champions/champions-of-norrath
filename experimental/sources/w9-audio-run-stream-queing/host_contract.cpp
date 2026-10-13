#define NORRATH_HOST_CONTRACT 1
#include "run_stream_queing.cpp"
#include <cstdio>
#include <cstring>
unsigned char stream[3 * 452] = {};
unsigned char norrath_sound_rpc_client_view[4] = {}, norrath_sound_rpc_send_view[16] = {}, norrath_sound_rpc_receive_view[64] = {};
struct Event { int command, w0, w1, w2; };
static Event log_[16];
static int events;
static int table_words[12];
extern "C" int norrath_sound_call_rpc(void* client, int command, int mode, void* send, int sb, void* recv, int rb, void* ef, void* ep) {
    if (client != norrath_sound_rpc_client_view || send != norrath_sound_rpc_send_view || recv != norrath_sound_rpc_receive_view || mode || ef || ep || rb != 64 || sb != (command == 80 ? 0 : 16)) return 99;
    int w[3];
    std::memcpy(w, norrath_sound_rpc_send_view, sizeof w);
    log_[events++] = {command, w[0], w[1], w[2]};
    if (command == 80) std::memcpy(norrath_sound_rpc_receive_view, table_words, sizeof table_words);
    return 0;
}
static void put(int stream_number, int offset, int value) { std::memcpy(stream + 452 * stream_number + offset, &value, 4); }
static int get(int stream_number, int offset) { int v; std::memcpy(&v, stream + 452 * stream_number + offset, 4); return v; }
int main() {
    if (runStreamQueing__Fv() != 0 || events != 1 || log_[0].command != 80) return 1;   // all counts zero
    // stream 1 (flag word 0 is the first block's word +44): two queued blocks, flag 0x2 with table word 2 zero, pending word set
    events = 0;
    put(1, 448, 2); put(1, 36, 9);
    std::memcpy(stream + 452 + 64, "first", 6);
    std::memcpy(stream + 452 + 64 + 48, "second", 7);
    put(1, 64 + 16, 0x11); put(1, 64 + 20, 0x22); put(1, 64 + 24, 0x33); put(1, 64 + 28, 0x44);
    put(1, 64 + 32, 0x55); put(1, 64 + 36, 0x66); put(1, 64 + 40, 0x77); put(1, 64 + 44, 2);
    table_words[4] = 0x99;
    int rv = runStreamQueing__Fv(); if (rv != 1) { std::printf("rv %d ev %d\n", rv, events); return 2; }
    if (events != 4 || log_[1].command != 96 || log_[1].w0 != 1) return 3;
    if (log_[2].command != 112 || log_[2].w0 != 1 || log_[2].w1 != 0x3fff) return 4;
    if (log_[3].command != 64 || log_[3].w0 != 1 || log_[3].w1 != 9) return 5;
    if (std::strcmp(reinterpret_cast<char*>(stream + 452), "first") != 0 || get(1, 448) != 1) return 6;
    if (std::memcmp(stream + 452 + 64, "second", 7) != 0) return 7;
    if (get(1, 16) != 0x11 || get(1, 20) != 0x22 || get(1, 24) != 0x33 || get(1, 28) != 0x44) return 8;
    if (get(1, 32) != 2 || get(1, 52) != 0x55 || get(1, 56) != 0x66 || get(1, 60) != 0x77 || get(1, 40) != 0x99) return 9;
    if (get(1, 36) != 0) return 10;
    if (log_[3].w2 != 0x55) return 11;
    // flag 0x2 with table word 2 nonzero sends 112 (stream, 0) and leaves the queue alone
    events = 0; put(1, 448, 1); put(1, 108, 2); table_words[6] = 5;
    if (runStreamQueing__Fv() != 0 || events != 2 || log_[1].command != 112 || log_[1].w0 != 1 || log_[1].w1 != 0 || get(1, 448) != 1) return 12;
    // flag 0x8, and flag 0x4 with word +20 set, skip the record
    events = 0; put(1, 108, 8);
    if (runStreamQueing__Fv() != 0 || events != 1) return 13;
    put(1, 108, 4); put(1, 20, 1);
    events = 0;
    if (runStreamQueing__Fv() != 0 || events != 1) return 14;
    std::puts("run_stream_queing passed");
    return 0;
}
