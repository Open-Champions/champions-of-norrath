#include "get_stream_amount_played.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char stream[3 * 452] = {};
extern "C" unsigned char norrath_sound_rpc_client_view[96] = {};
extern "C" unsigned char norrath_sound_rpc_send_view[64] = {};
extern "C" unsigned char norrath_sound_rpc_receive_view[64] = {};
static int calls = 0;
static void* a_client; static int a_cmd, a_mode, a_slen, a_rlen;
static void *a_send, *a_recv, *a_end, *a_par;
extern "C" int norrath_sound_call_rpc(void* c, int cmd, int mode, void* s, int sl, void* r, int rl, void* e, void* p) {
    ++calls; a_client = c; a_cmd = cmd; a_mode = mode; a_send = s; a_slen = sl; a_recv = r; a_rlen = rl; a_end = e; a_par = p;
    for (int i = 0; i < 4; ++i) { int v = 1000 + i; std::memcpy(norrath_sound_rpc_receive_view + i * 16 + 12, &v, 4); }
    return 0;
}
int main() {
    if (soundGetStreamAmountPlayed__Fi(1) != 0 || calls != 0) return 1;   // inactive stream
    stream[452] = 1;
    if (soundGetStreamAmountPlayed__Fi(1) != 1001 || calls != 1) return 2;
    if (a_client != norrath_sound_rpc_client_view || a_cmd != 80 || a_mode != 0) return 3;
    if (a_send != norrath_sound_rpc_send_view || a_slen != 0) return 4;
    if (a_recv != norrath_sound_rpc_receive_view || a_rlen != 64) return 5;
    if (a_end != nullptr || a_par != nullptr) return 6;
    stream[0] = 0x80;                                // any nonzero byte counts
    if (soundGetStreamAmountPlayed__Fi(0) != 1000) return 7;
    stream[2 * 452] = 1;
    if (soundGetStreamAmountPlayed__Fi(2) != 1002 || calls != 3) return 8;
    std::puts("get_stream_amount_played passed");
    return 0;
}
