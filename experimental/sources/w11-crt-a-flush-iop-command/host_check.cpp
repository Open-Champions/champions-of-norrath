// Host contract for FlushIopCommand__Fb. Not an EE plan input. The two RPC calls are stubs
// that record their arguments and the order they were made in.
#include <cstring>
#include "flush_iop_command.cpp"
struct SifRpcClient { int unused; };
SifRpcClient sSifClient; int sSend[128]; int sRecv[128]; int sFunctionIndex;
static int stat_result, stat_calls, call_calls, seq, stat_seq, call_seq;
static unsigned c_func, c_mode; static void* c_send; static int c_ssize; static void* c_recv; static int c_rsize;
static void* c_efn; static void* c_earg; static int send_word_at_call, index_at_call;
extern "C" int sceSifCheckStatRpc(SifRpcClient* c) { ++stat_calls; stat_seq = ++seq; return c == &sSifClient ? stat_result : 99; }
extern "C" int sceSifCallRpc(SifRpcClient* c, unsigned f, unsigned m, void* s, int ss, void* r, int rs, void (*ef)(void*), void* ea) {
    ++call_calls; call_seq = ++seq; c_func = f; c_mode = m; c_send = s; c_ssize = ss; c_recv = r; c_rsize = rs;
    c_efn = reinterpret_cast<void*>(ef); c_earg = ea; send_word_at_call = sSend[0]; index_at_call = sFunctionIndex;
    return c == &sSifClient ? 0 : -5;
}
int main() {
    // busy
    stat_result = 4; sSend[0] = 3; sFunctionIndex = 9;
    if (FlushIopCommand__Fb(true) != -1 || call_calls || sSend[0] != 3 || sFunctionIndex != 9) return 1;
    // idle but empty
    stat_result = 0; sSend[0] = 0;
    if (FlushIopCommand__Fb(true) != -2 || call_calls || sFunctionIndex != 9) return 2;
    // idle with commands, non-blocking: returns the old index and clears both words after the call
    sSend[0] = 5; sFunctionIndex = 17;
    if (FlushIopCommand__Fb(false) != 17) return 3;
    if (call_calls != 1 || c_func != 0x80f0 || c_mode != 0 || c_send != sSend || c_ssize != 512 || c_recv != sRecv || c_rsize != 512) return 4;
    if (c_efn != 0 || c_earg != 0 || stat_seq >= call_seq) return 5;
    if (send_word_at_call != 5 || index_at_call != 17 || sSend[0] != 0 || sFunctionIndex != 0) return 6;
    // blocking flag, any nonzero bool
    sSend[0] = -1; sFunctionIndex = 0;
    if (FlushIopCommand__Fb(true) != 0 || c_mode != 1 || call_calls != 2 || sSend[0] != 0) return 7;
    return 0;
}
