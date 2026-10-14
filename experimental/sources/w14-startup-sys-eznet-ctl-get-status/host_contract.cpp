// Host contract for ezNetCtlGetStatus. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// printf and sceSifCallRpc are scripted. Checks the not-running path (message, -1, no RPC), the RPC arguments,
// that a zero status word returns 0 without printing, and that a nonzero word prints the two lines and is returned.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
extern "C" { int m_rpcRunning; unsigned char m_cd[64]; }
static std::string out;
static int calls, fn_seen, mode_seen, send_len, recv_len, set_status;
static void *client_seen, *send_seen, *recv_seen, *end_seen, *arg_seen;
extern "C" int printf(const char* f, ...) {
    char buf[256]; va_list ap; va_start(ap, f); std::vsnprintf(buf, sizeof buf, f, ap); va_end(ap);
    out += buf; return 0;
}
extern "C" int sceSifCallRpc(void* c, unsigned f, unsigned m, void* s, int sl, void* r, int rl, void* e, void* a) {
    ++calls; client_seen = c; fn_seen = f; mode_seen = m; send_seen = s; send_len = sl; recv_seen = r; recv_len = rl; end_seen = e; arg_seen = a;
    *static_cast<int*>(r) = set_status; return -7;
}
#include "get_status.cpp"
int main() {
    int bad = 0;
    int rec[68]; std::memset(rec, 0x11, sizeof rec);
    m_rpcRunning = 0; calls = 0; out.clear();
    if (ezNetCtlGetStatus(rec) != -1 || calls != 0 || out != "**** eznetctl RPC server not currently running\n" || rec[0] != 0x11111111) { std::puts("not running"); ++bad; }
    m_rpcRunning = 1; out.clear(); set_status = 0;
    if (ezNetCtlGetStatus(rec) != 0 || !out.empty() || calls != 1) { std::puts("zero status"); ++bad; }
    if (client_seen != m_cd || fn_seen != 3 || mode_seen != 0 || send_seen != rec || recv_seen != rec || send_len != 272 || recv_len != 272 || end_seen || arg_seen) { std::puts("rpc arguments"); ++bad; }
    out.clear(); set_status = -12;
    if (ezNetCtlGetStatus(rec) != -12 || out != "../eznetcnf/libezctl.c> ezNetCtlGetStatus returned -12\n") { std::puts("nonzero status"); ++bad; }
    out.clear(); set_status = 5;
    if (ezNetCtlGetStatus(rec) != 5 || out != "../eznetcnf/libezctl.c> ezNetCtlGetStatus returned 5\n") { std::puts("positive status"); ++bad; }
    return bad;
}
